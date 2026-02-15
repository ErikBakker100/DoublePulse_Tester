cmake_minimum_required(VERSION 3.20)


# ----------------------
# Copy either kernel8.img or kernel.img to the SD-card output directory, depending on the selected ARCH
# kernel.img	ARMv6 (32-bit)	Pi 1, Zero, Zero W
# kernel7.img	ARMv7 (32-bit)	Pi 2, Pi 3
# kernel7l.img	ARMv7+LPAE (32-bit)	Pi 4, Pi 400 (De 'l' staat voor Large Physical Address Extension)
# kernel8.img	ARMv8 (64-bit)	Pi 3, Zero 2 W, Pi 4, Pi 5
# ----------------------

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# ----------------------
# User options
# ----------------------
include(${CMAKE_SOURCE_DIR}/CMakeFiles/utils.cmake)

set(ARCH "arm64" CACHE STRING "Target architecture")
set_property(CACHE ARCH PROPERTY STRINGS arm32 arm64)
validate_choice(ARCH arm32 arm64)

set(DUALCORE "on" CACHE STRING "Dual core support")
set_property(CACHE DUALCORE PROPERTY STRINGS on off)
validate_choice(DUALCORE on off)

print_choice(ARCH
    arm32 "arm32 build"
    arm64 "arm64 build"
)

print_choice(DUALCORE
    on  "Dual-core build"
    off "Single-core build"
)

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
    set(LINKER_LOAD_ADDR 0x80000)
    set(ARM_64BIT 1)
    set(IMAGEFILE kernel8.img)
    # Extra namen waar de firmware naar zoekt op 64-bit systemen
    set(ALT_IMAGEFILES kernel8-71.img kernel8-2712.img)
else() # 32-bit
    set(LINKER_LOAD_ADDR 0x8000)
    set(ARM_64BIT 0)
    # Voor 32-bit build genereren we alle varianten
    set(IMAGEFILE kernel.img)
    set(ALT_IMAGEFILES kernel7.img kernel7l.img)
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

set(BOOTFILES_DIR ${CMAKE_SOURCE_DIR}/CMakeFiles/boot/PI)
set(MODEL_CONFIG "[pi4]\narm_boost=1\nenable_jtag_gpio=1\n")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_BINARY_DIR}/bin/${IMAGEFILE}
    # Maak kopieën voor de alternatieve namen (voor automatische detectie firmware)
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/bin/ 
            ${CMAKE_COMMAND} -E create_symlink ${IMAGEFILE} kernel7.img
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/bin/ 
            ${CMAKE_COMMAND} -E create_symlink ${IMAGEFILE} kernel7l.img
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/bin/ 
            ${CMAKE_COMMAND} -E create_symlink ${IMAGEFILE} kernel8.img            

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

# Kopieer ALLE gegenereerde kernel bestanden naar de SD kaart
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/kernel.img ${SDCARD_DIR}/ 2>NUL || ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/kernel7.img ${SDCARD_DIR}/ 2>NUL || ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/kernel7l.img ${SDCARD_DIR}/ 2>NUL || ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/kernel8.img ${SDCARD_DIR}/ 2>NUL || ${CMAKE_COMMAND} -E echo ""
    
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/config.txt ${SDCARD_DIR}
)
