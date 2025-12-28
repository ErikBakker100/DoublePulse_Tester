cmake_minimum_required(VERSION 3.17)

# ----------------------
# Gebruikersopties
# ----------------------
set(ARCH "arm64" CACHE STRING "Target architecture: arm32 or arm64")
set_property(CACHE ARCH PROPERTY STRINGS arm32 arm64)

set(RPI_MODEL "Z2" CACHE STRING "Raspberry Pi model: Z1, Z2, 4B")
set_property(CACHE RPI_MODEL PROPERTY STRINGS Z1 Z2 4B)

set(DUALCORE "on" CACHE STRING "Dual core support: on or off")
set_property(CACHE DUALCORE PROPERTY STRINGS on off)

set(SDCARD_DIR ${CMAKE_SOURCE_DIR}/sdcard)

if(ARCH STREQUAL "arm64")
    message(STATUS "arm64 build enabled")
else()
    message(STATUS "arm32 build enabled")
endif()

if(RPI_MODEL STREQUAL "Z2")
    message(STATUS "Building for Raspberry Pi Zero 2(W)")
elseif(RPI_MODEL STREQUAL "Z1")
    message(STATUS "Building for Raspberry Pi Zero 1")
elseif(RPI_MODEL STREQUAL "4B")
    message(STATUS "Building for Raspberry Pi 4B")
endif()

if(DUALCORE STREQUAL "on")
    message(STATUS "Dual-core build enabled")
else()
    message(STATUS "Single-core build")
endif()

if(NOT (ARCH STREQUAL "arm32" OR ARCH STREQUAL "arm64"))
    message(FATAL_ERROR "Ongeldige waarde voor ARCH: ${ARCH}. Kies uit: arm32, arm64")
endif()

if(NOT (RPI_MODEL STREQUAL "Z1" OR RPI_MODEL STREQUAL "Z2" OR RPI_MODEL STREQUAL "4B"))
    message(FATAL_ERROR "Ongeldige waarde voor RPI_MODEL: ${RPI_MODEL}. Kies uit: Z1, Z2, 4B")
endif()

if(NOT (DUALCORE STREQUAL "on" OR DUALCORE STREQUAL "off"))
    message(FATAL_ERROR "Ongeldige waarde voor DUALCORE: ${DUALCORE}. Kies uit: on, off")
endif()

# ----------------------
# Combinaties valideren
# ----------------------
if(RPI_MODEL STREQUAL "Z1" AND NOT ARCH STREQUAL "arm32")
    message(FATAL_ERROR "Raspberry Pi Zero 1 (Z1) ondersteunt alleen ARCH=arm32")
endif()

if(RPI_MODEL STREQUAL "Z1" AND NOT DUALCORE STREQUAL "off")
    message(FATAL_ERROR "Raspberry Pi Zero 1 (Z1) is een single core device, alleen DUALCORE=off is mogelijk.")
endif()

# ----------------------
# Toolchain automatisch kiezen, en Startup & linker script
# ----------------------
if(ARCH STREQUAL "arm32")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/CMakeFiles/toolchain-arm-eabi.cmake"
        CACHE FILEPATH "Toolchain file" FORCE)
    set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/src/start32.s)
elseif(ARCH STREQUAL "arm64")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/CMakeFiles/toolchain-aarch64.cmake"
        CACHE FILEPATH "Toolchain file" FORCE)
    set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/src/start64.s)
else()
    message(FATAL_ERROR "ARCH moet arm32 of arm64 zijn")
endif()

# ----------------------
# Project setup
# ----------------------
project(doublepulse
    VERSION 1.0
    LANGUAGES C ASM)

# ----------------------
# Bronbestanden
# ----------------------
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/lib/*.c
    ${STARTUP_FILE}
)

set(LINKER_SCRIPT ${CMAKE_BINARY_DIR}/bin/linker.ld)

# ----------------------
# Executable
# ----------------------
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# ----------------------
# Compiler instellingen
# ----------------------
# Kies variabele op basis van RPI_MODEL
if(RPI_MODEL STREQUAL "Z1")
    set(RPI_DEFINE raspberry_zero_1w)
    set(CORE_FREQ 250)
elseif(RPI_MODEL STREQUAL "Z2")
    set(RPI_DEFINE raspberry_zero_2w)
    set(CORE_FREQ 500)
elseif(RPI_MODEL STREQUAL "4B")
    set(RPI_DEFINE raspberry_pi_4b)
    set(CORE_FREQ 500)
else()
    message(FATAL_ERROR "Onbekend RPI_MODEL: ${RPI_MODEL}")
endif()

# Definities doorgeven aan de compiler (alleen voor C, niet voor ASM)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${RPI_DEFINE}>
    $<$<COMPILE_LANGUAGE:C>:CORE_FREQ=${CORE_FREQ}>
)

# ----------------------
# Model-specifieke compileropties
# ----------------------
set(ARCH_FLAGS "")

if(RPI_MODEL STREQUAL "Z1" AND ARCH STREQUAL "arm32")
	set(ARCH_FLAGS -mcpu=arm1176jzf-s -marm) #-marm dwingt de compiler om ARM 32-bit (A32) instructies te genereren.
elseif(RPI_MODEL STREQUAL "Z2" AND ARCH STREQUAL "arm32")
    set(ARCH_FLAGS -mcpu=cortex-a53 -marm)
elseif(RPI_MODEL STREQUAL "Z2" AND ARCH STREQUAL "arm64")
    set(ARCH_FLAGS -march=armv8-a)
elseif(RPI_MODEL STREQUAL "4B" AND ARCH STREQUAL "arm32")
    set(ARCH_FLAGS -mcpu=cortex-a72 -marm)
elseif(RPI_MODEL STREQUAL "4B" AND ARCH STREQUAL "arm64")
    set(ARCH_FLAGS -march=armv8-a)	
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -fno-builtin
	${ARCH_FLAGS}
)

target_link_options(${PROJECT_NAME} PRIVATE
    "-nostartfiles"
    "-nostdlib"
    "-T${LINKER_SCRIPT}"
    "-Wl,-Map=${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.map"
)

# ----------------------
# Post-build acties
# ----------------------

# 64bit versions require kernel8.img, whereby 32bit versions uses kernel.img
if(ARCH STREQUAL "arm64") 
    set(IMAGEFILE kernel8.img)
    set(ARM_64BIT 1)
    set(KERNEL ${IMAGEFILE})
    set(LINKER_LOAD_ADDR 0x80000)
else() 
    set(IMAGEFILE kernel.img)
    set(ARM_64BIT 0)
    set(KERNEL ${IMAGEFILE})
    set(LINKER_LOAD_ADDR 0x8000)
    endif()

configure_file(
    ${CMAKE_SOURCE_DIR}/src/linker.ld.in
    ${CMAKE_BINARY_DIR}/bin/linker.ld
    @ONLY
)

# Decide which image files the Pi expects, and set Model-specific options
if(RPI_MODEL STREQUAL "Z1")
    set(BOOTFILES_DIR ${CMAKE_SOURCE_DIR}/CMakeFiles/boot/Z1)
    set(MODEL_CONFIG "")
elseif(RPI_MODEL STREQUAL "Z2")
    set(BOOTFILES_DIR ${CMAKE_SOURCE_DIR}/CMakeFiles/boot/Z2)
    set(MODEL_CONFIG "")
elseif(RPI_MODEL STREQUAL "4B")
    set(BOOTFILES_DIR ${CMAKE_SOURCE_DIR}/CMakeFiles/boot/4B)
    set(MODEL_CONFIG "[pi4]\narm_boost=1\nenable_jtag_gpio=1\n")
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_BINARY_DIR}/bin/${IMAGEFILE}
    COMMAND ${CMAKE_OBJCOPY} --srec-forceS3
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.srec
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -S $<TARGET_FILE:${PROJECT_NAME}> > ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.lst
    )
endif()

# Zorg dat de kernel altijd gebouwd wordt als je 'make' doet
add_custom_target(kernel ALL
    DEPENDS ${PROJECT_NAME}
)

# ----------------------
# Configuratiebestand voor de SD-kaart
# ----------------------
configure_file(
    ${CMAKE_SOURCE_DIR}/CMakeFiles/boot/config.txt.in
    ${CMAKE_BINARY_DIR}/bin/config.txt
    @ONLY
)

# Copy model-specific boot files
add_custom_target(copy_boot_files ALL
    # verwijder bestaande sdcard dir (indien aanwezig)
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${SDCARD_DIR} 
    # maak hem opnieuw aan
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SDCARD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${BOOTFILES_DIR} ${SDCARD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/${IMAGEFILE} ${SDCARD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/config.txt ${SDCARD_DIR}
    COMMAND ${CMAKE_COMMAND} -E echo "Copying boot files for ${RPI_MODEL}/${ARCH} to ${SDCARD_DIR}"
)
