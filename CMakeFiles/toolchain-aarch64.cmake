set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER aarch64-none-elf-gcc)
set(CMAKE_ASM_COMPILER aarch64-none-elf-gcc)

set(CMAKE_C_FLAGS "-Wall -nostdlib -nostartfiles -ffreestanding" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_DEBUG "-Wall -nostdlib -nostartfiles -ffreestanding -g3 -Og" CACHE STRING "" FORCE)

#set(CMAKE_ASM_FLAGS "--warn --fatal-warnings" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp -Wall -Werror" CACHE STRING "" FORCE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
