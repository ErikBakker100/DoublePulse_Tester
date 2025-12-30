cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# ----------------------
# User options
# ----------------------
include(${CMAKE_SOURCE_DIR}/CMakeFiles/utils.cmake)

set(ARCH "arm64" CACHE STRING "Target architecture")
set_property(CACHE ARCH PROPERTY STRINGS arm32 arm64)
validate_choice(ARCH arm32 arm64)

set(RPI_MODEL "Z2" CACHE STRING "Raspberry Pi model")
set_property(CACHE RPI_MODEL PROPERTY STRINGS Z1 Z2 3A 3B 3B+ 4B 5)
validate_choice(RPI_MODEL Z1 Z2 3A 3B 3B+ 4B 5)

set(DUALCORE "on" CACHE STRING "Dual core support")
set_property(CACHE DUALCORE PROPERTY STRINGS on off)
validate_choice(DUALCORE on off)

print_choice(ARCH
    arm32 "arm32 build"
    arm64 "arm64 build"
)

print_choice(RPI_MODEL
    Z1 "Building for Raspberry Pi Zero 1"
    Z2 "Building for Raspberry Pi Zero 2(W)"
    3A "Building for Raspberry Pi 3A"
    3B "Building for Raspberry Pi 3B"
    3B+ "Building for Raspberry Pi 3B+"
    4B "Building for Raspberry Pi 4B"
    5 "Building for Raspberry Pi 5"
)

print_choice(DUALCORE
    on  "Dual-core build"
    off "Single-core build"
)

# ----------------------
# Validating user choices
# ----------------------
if(RPI_MODEL STREQUAL "Z1")
    if(ARCH STREQUAL "arm64")
        message(FATAL_ERROR "Raspberry Pi Zero 1 does not support arm64")
    endif()
    if(DUALCORE STREQUAL "on")
        message(FATAL_ERROR "Raspberry Pi Zero 1 does not support dual-core")
    endif()
endif()

# ----------------------
# Raspberry Pi CPU map
# ----------------------

if(RPI_MODEL STREQUAL "Z1")
    # Raspberry Pi Zero / Zero W
    set(RPI_DEFINE      RPIZ1)
    set(RPI_CPU         arm1176jzf-s)
    set(RPI_ARCH        armv6)
    set(RPI_ARM_BITS    arm32)

elseif(RPI_MODEL STREQUAL "Z2")
    # Raspberry Pi Zero 2 W
    set(RPI_DEFINE      RPIZ2)
    set(RPI_CPU         cortex-a53)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64

elseif(RPI_MODEL STREQUAL "3A")
    # Raspberry Pi 3A
    set(RPI_DEFINE      RPI3A)
    set(RPI_CPU         cortex-a53)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64

elseif(RPI_MODEL STREQUAL "3B")
    # Raspberry Pi 3B
    set(RPI_DEFINE      RPI3B)
    set(RPI_CPU         cortex-a53)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64

elseif(RPI_MODEL STREQUAL "3B+")
    # Raspberry Pi 3B+
    set(RPI_DEFINE      RPI3B+)
    set(RPI_CPU         cortex-a53)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64

elseif(RPI_MODEL STREQUAL "4B")
    # Raspberry Pi 4B
    set(RPI_DEFINE      RPI4B)
    set(RPI_CPU         cortex-a72)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64

elseif(RPI_MODEL STREQUAL "5")
    # Raspberry Pi 5
    set(RPI_DEFINE      RPI5)
    set(RPI_CPU         cortex-a76)
    set(RPI_ARCH        armv8.2-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64

else()
    message(FATAL_ERROR "Onbekend RPI_MODEL: ${RPI_MODEL}")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Build type (Debug, Release)" FORCE)
endif()

# ----------------------
# Auto select toolchains and Startup files for selected ARCH
# ----------------------
if(ARCH STREQUAL "arm32")
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
    set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/src/start32.s)
elseif(ARCH STREQUAL "arm64")
    set(CMAKE_C_COMPILER aarch64-none-elf-gcc)
    set(CMAKE_ASM_COMPILER aarch64-none-elf-gcc)
    set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/src/start64.s)
else()
    message(FATAL_ERROR "-DARCH= has to be arm32 or arm64")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ----------------------
# Linker script defines
# ----------------------
set(LINKER_DEFINES "")

if(DUALCORE STREQUAL "on")
    list(APPEND LINKER_DEFINES -DDUALCORE)
endif()

# ----------------------
# Set output directory for SD-Card
# ----------------------
set(SDCARD_DIR ${CMAKE_SOURCE_DIR}/sdcard)

# ----------------------
# Bronbestanden
# ----------------------
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/lib/*.c
    ${STARTUP_FILE}
)

# ----------------------
# Project setup
# ----------------------
project(doublepulse
    VERSION 1.0
    LANGUAGES C ASM)

# ----------------------
# Explicit Debug / Release flags
# ----------------------
set(CMAKE_C_FLAGS_DEBUG "-Og -g3" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE STRING "" FORCE)

set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE STRING "" FORCE)

# ----------------------
# Kernel image & linker load address
# ----------------------
if(ARCH STREQUAL "arm64") 
    set(IMAGEFILE kernel8.img)
    set(LINKER_LOAD_ADDR 0x80000)
else() 
    set(IMAGEFILE kernel.img)
    set(LINKER_LOAD_ADDR 0x8000)
endif()
# ----------------------
# Linker script generation
# ----------------------
set(LINKER_PP     ${CMAKE_BINARY_DIR}/bin/linker.ld.pp)
set(LINKER_SCRIPT ${CMAKE_BINARY_DIR}/bin/linker.ld)

# Stap 1: CMake-variabelen vervangen (@VAR@)
configure_file(
    ${CMAKE_SOURCE_DIR}/src/linker.ld.S
    ${LINKER_PP}
    @ONLY
)

# ----------------------
# Executable
# ----------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# Forceer dat ASM geen C compile flags meekrijgt bij linking
set_source_files_properties(${STARTUP_FILE} PROPERTIES
     COMPILE_FLAGS "-x assembler-with-cpp"
)

add_custom_command(
    OUTPUT ${LINKER_SCRIPT}
    COMMAND ${CMAKE_C_COMPILER} -E -P -x c
            ${LINKER_DEFINES}
            ${LINKER_PP}
            -o ${LINKER_SCRIPT}
    DEPENDS ${LINKER_PP}
    COMMENT "Preprocessing linker script"
)

add_custom_target(generate_linker_script DEPENDS ${LINKER_SCRIPT})
add_dependencies(${PROJECT_NAME} generate_linker_script)

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY
    LINK_DEPENDS ${LINKER_SCRIPT}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# ----------------------
# Compiler settings
# ----------------------

# Passing definitions onto the compiler (for C and ASM)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${RPI_DEFINE}>
)

if(DUALCORE STREQUAL "on")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
    DUALCORE)
endif()

if(ARCH STREQUAL "arm64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM>:ARCH_ARM64>)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM>:ARCH_ARM32>)
endif()

# Compiler flags
set(ARCH_FLAGS "")

if(RPI_ARM_BITS STREQUAL "arm32")
    set(ARCH_FLAGS
        -mcpu=${RPI_CPU}
        -marm
    )
elseif(RPI_ARM_BITS STREQUAL "arm64")
    set(ARCH_FLAGS
        -march=${RPI_ARCH}
    )
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:C>: -Wall -fno-builtin>
	${ARCH_FLAGS}
)

# ----------------------
# Linker flags
# ----------------------

target_link_options(${PROJECT_NAME} PRIVATE
    -nostartfiles
    -nostdlib
    -ffreestanding
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.map
)

# ----------------------
# Post-build acties
# ----------------------

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
