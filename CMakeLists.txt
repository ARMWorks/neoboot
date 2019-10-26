cmake_minimum_required(VERSION 2.8)

option(PLATFORM "Platform to build bootloader for." "")
if(NOT IS_DIRECTORY "${CMAKE_SOURCE_DIR}/src/platform/${PLATFORM}/")
    message(FATAL_ERROR "Platform undefined or unsupported.")
endif()

include(src/platform/${PLATFORM}/CMakeLists.txt)

project(neoboot C ASM)

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

add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET} ${PROJECT_NAME}-${PLATFORM}.bin
)