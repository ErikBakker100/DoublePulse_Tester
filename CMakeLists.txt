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

set(RPI_DEFINE "RPIZ2W" CACHE STRING "Raspberry Pi model")
set_property(CACHE RPI_DEFINE PROPERTY STRINGS RPIA RPIB RPIA_PLUS RPIB_PLUS RPI2B RPIALPHA RPICM1 RPI3B_V11 RPIZ1 RPICM3 RPIZ1W RPI3B RPI3A_PLUS RPICM3_PLUS RPI4B RPI3B_PLUS RPI400 RPICM4 RPIZ2W RPI5 RPICM5 RPI500 RPICM5_LITE RPICM0)
validate_choice(RPI_DEFINE RPIA RPIB RPIA_PLUS RPIB_PLUS RPI2B RPIALPHA RPICM1 RPI3B_V11 RPIZ1 RPICM3 RPIZ1W RPI3B RPI3A_PLUS RPICM3_PLUS RPI4B RPI3B_PLUS RPI400 RPICM4 RPIZ2W RPI5 RPICM5 RPI500 RPICM5_LITE RPICM0)

set(DUALCORE "on" CACHE STRING "Dual core support")
set_property(CACHE DUALCORE PROPERTY STRINGS on off)
validate_choice(DUALCORE on off)

print_choice(ARCH
    arm32 "arm32 build"
    arm64 "arm64 build"
)

print_choice(RPI_DEFINE
    RPIA            " "
    RPIB            " "
    RPIA_PLUS       " "
    RPIB_PLUS       " "
    RPI2B           " "
    RPIALPHA        " "
    RPICM1          " "
    RPI3B_V11       "Building for Raspberry Pi 3B"
    RPIZ1           "Building for Raspberry Pi Zero 1"
    RPICM3          " "
    RPIZ1W          " "
    RPI3B           "Building for Raspberry Pi 3B"
    RPI3A_PLUS      "Building for Raspberry Pi 3A+"
    RPICM3_PLUS     " "
    RPI4B           "Building for Raspberry Pi 4B"
    RPI3B_PLUS      " "
    RPI400          " "
    RPICM4          " "
    RPIZ2W          "Building for Raspberry Pi Zero 2(W)"
    RPI5            "Building for Raspberry Pi 5"
    RPICM5          " "
    RPI500          " "
    RPICM5_LITE     " "
    RPICM0          " "
)

print_choice(DUALCORE
    on  "Dual-core build"
    off "Single-core build"
)

# ----------------------
# Validating user choices
# ----------------------
if(RPI_DEFINE STREQUAL "RPIA" OR RPI_DEFINE STREQUAL "RPIB" OR RPI_DEFINE STREQUAL "RPIA_PLUS" OR RPI_DEFINE STREQUAL "RPIB_PLUS" OR RPI_DEFINE STREQUAL "RPIALPHA" OR RPI_DEFINE STREQUAL "RPICM1" OR RPI_DEFINE STREQUAL "RPIZ1" OR RPI_DEFINE STREQUAL "RPIZ1W")
    if(ARCH STREQUAL "arm64")
        message(FATAL_ERROR "Model does not support arm64")
    endif()
    if(DUALCORE STREQUAL "on")
        message(FATAL_ERROR "Model does not support dual-core")
    endif()
endif()

# ----------------------
# Raspberry Pi CPU map
# ----------------------

if(RPI_DEFINE STREQUAL "RPIZ1")
    # Raspberry Pi Zero / Zero W
    set(RPI_CPU         arm1176jzf-s)
    set(RPI_ARCH        armv6)
    set(RPI_ARM_BITS    arm32)
    set(CORE_FREQ       250)
    set(ARM_FREQ        250)
    set(OUTPUT          18)
    set(IMAGEFILE kernel.img)

elseif(RPI_DEFINE STREQUAL "RPIZ2W")
    # Raspberry Pi Zero 2 W
    set(RPI_CPU         cortex-a53)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64
    set(CORE_FREQ       500)
    set(ARM_FREQ        600)
    set(OUTPUT          18)
    set(IMAGEFILE kernel8.img)

elseif(RPI_DEFINE STREQUAL "RPI3A")
    # Raspberry Pi 3A
    set(RPI_CPU         cortex-a53)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64
    set(CORE_FREQ       500)
    set(ARM_FREQ        250)
    set(OUTPUT          18)
    set(IMAGEFILE kernel8.img)

elseif(RPI_DEFINE STREQUAL "RPI3B")
    # Raspberry Pi 3B
    set(RPI_CPU         cortex-a53)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64
    set(CORE_FREQ       500)
    set(ARM_FREQ        250)
    set(OUTPUT          18)
    set(IMAGEFILE kernel8.img)

elseif(RPI_DEFINE STREQUAL "RPI3B_PLUS")
    # Raspberry Pi 3B+
    set(RPI_CPU         cortex-a53)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64
    set(CORE_FREQ       500)
    set(ARM_FREQ        250)
    set(OUTPUT          18)
    set(IMAGEFILE kernel8.img)

elseif(RPI_DEFINE STREQUAL "RPI4B")
    # Raspberry Pi 4B
    set(RPI_CPU         cortex-a72)
    set(RPI_ARCH        armv8-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64
    set(CORE_FREQ       500)
    set(ARM_FREQ        250)
    set(OUTPUT          18)
    set(IMAGEFILE kernel8.img)

elseif(RPI_DEFINE STREQUAL "RPI5")
    # Raspberry Pi 5
    set(RPI_CPU         cortex-a76)
    set(RPI_ARCH        armv8.2-a)
    set(RPI_ARM_BITS    ${ARCH})   # arm32 / arm64
    set(CORE_FREQ       500)
    set(ARM_FREQ        250)
    set(OUTPUT          18)
    set(IMAGEFILE kernel8.img)

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
    set(ARM_64BIT 1)
else() # 32-bit
    set(LINKER_LOAD_ADDR 0x8000)
    set(ARM_64BIT 0)
#RPIA RPIB RPIA_PLUS RPIB_PLUS RPI2B RPIALPHA RPICM1 RPI3B_V11 RPIZ1 RPICM3 RPIZ1W RPI3B RPI3A_PLUS RPICM3_PLUS RPI4B RPI3B_PLUS RPI400 RPICM4 RPIZ2W RPI5 RPICM5 RPI500 RPICM5_LITE RPICM0
    if(RPI_DEFINE MATCHES "^(RPI3A|RPI3B|RPI3B_PLUS)$")
        set(KERNEL kernel4.img)
    elseif(RPI_DEFINE MATCHES "^(RPIZ1|RPIZ1W)$")
        set(KERNEL kernel.img)
    elseif(RPI_DEFINE STREQUAL "RPIZ2W")
        set(KERNEL kernel.img)   # Zero 2 gebruikt oude naam
    elseif(RPI_DEFINE MATCHES "^(RPI4B)$")
        set(KERNEL kernel7l.img)
    else()
        message(FATAL_ERROR "Unknown RPI model for 32-bit kernel")
    endif()
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
    $<$<COMPILE_LANGUAGE:C>:RPI_DEFINE="${RPI_DEFINE}">
    $<$<COMPILE_LANGUAGE:C>:CORE_FREQ=${CORE_FREQ}>
    $<$<COMPILE_LANGUAGE:C>:ARM_FREQ=${ARM_FREQ}>
    $<$<COMPILE_LANGUAGE:C>:OUTPUT=${OUTPUT}>
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
if(RPI_DEFINE STREQUAL "RPIZ1")
    set(BOOTFILES_DIR ${CMAKE_SOURCE_DIR}/CMakeFiles/boot/Z1)
    set(MODEL_CONFIG "")
elseif(RPI_DEFINE STREQUAL "RPIZ2W")
    set(BOOTFILES_DIR ${CMAKE_SOURCE_DIR}/CMakeFiles/boot/Z2W)
    set(MODEL_CONFIG "")
elseif(RPI_DEFINE STREQUAL "RPI4B")
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
    set(JTAG_ENABLE "gpio=22-27=a4")
else()
    set(JTAG_ENABLE "")
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
