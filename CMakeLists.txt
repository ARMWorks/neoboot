cmake_minimum_required(VERSION 2.8)

option(PLATFORM "Platform to build bootloader for." "")
if(PLATFORM STREQUAL "sunxi-h5")
    set(CPU "armv8")
else()
    message(FATAL_ERROR "Platform undefined or unsupported.")
endif()

set(CMAKE_TOOLCHAIN_FILE "cpu/${CPU}/toolchain.cmake" CACHE PATH "toolchain file")
project(neoboot C ASM)

include(cpu/${CPU}/CMakeLists.txt)
include(platform/${PLATFORM}/CMakeLists.txt)
include(deps/CMakeLists.txt)
include(src/CMakeLists.txt)
list(APPEND INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/include
)

set(TARGET ${PROJECT_NAME}-${PLATFORM}.elf)
add_executable(${TARGET} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${INCLUDES})
set_target_properties(${TARGET} PROPERTIES
    LINK_FLAGS "-Wl,-T${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PLATFORM}.lds,-M=${PROJECT_NAME}-${PLATFORM}.map,--gc-sections"
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PLATFORM}.lds
    MAIN_DEPENDENCY ${LINKER_SCRIPT_SOURCE}
    COMMAND ${CMAKE_C_COMPILER}
        -E ${LINKER_SCRIPT_SOURCE} -P
        -o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PLATFORM}.lds
        -I include
        VERBATIM)

add_custom_target(linkerscript
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PLATFORM}.lds
    VERBATIM
)

add_dependencies(${TARGET} linkerscript)

find_package(PythonInterp REQUIRED)
add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET} ${PROJECT_NAME}-${PLATFORM}.bin
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/mksunxiboot.py ${PROJECT_NAME}-${PLATFORM}.bin
    COMMENT "-- Generating ${PROJECT_NAME}-${PLATFORM}.bin"
)