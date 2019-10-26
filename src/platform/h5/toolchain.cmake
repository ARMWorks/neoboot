# The Generic system name is used for embedded targets (targets without OS) in
# CMake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Set a toolchain path. You only need to set this if the toolchain isn't in
# your system path. Don't forget a trailing path separator!
set(TC_PATH "$ENV{HOME}/x-tools/aarch64-unknown-elf/bin/")

# The toolchain prefix for all toolchain executables
set(CROSS_COMPILE aarch64-unknown-elf-)

set(CMAKE_C_COMPILER ${TC_PATH}${CROSS_COMPILE}gcc)
set(CMAKE_ASM_COMPILER ${TC_PATH}${CROSS_COMPILE}gcc)
set(CMAKE_C_COMPILER_WORKS 1)

# We must set the OBJCOPY setting into cache so that it's available to the
# whole project. Otherwise, this does not get set into the CACHE and therefore
# the build doesn't know what the OBJCOPY filepath is
set(CMAKE_OBJCOPY ${TC_PATH}${CROSS_COMPILE}objcopy
    CACHE FILEPATH "The toolchain objcopy command " FORCE)

# Set the CMAKE C flags (which should also be used by the assembler!
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -march=armv8-a+crc+crypto -mtune=cortex-a53")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -nostartfiles -nostdlib")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "")
