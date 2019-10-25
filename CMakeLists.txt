cmake_minimum_required(VERSION 2.8)

option(PLATFORM "Platform to build bootloader for." "")
if(NOT IS_DIRECTORY "${CMAKE_SOURCE_DIR}/platform/${PLATFORM}/")
    message(FATAL_ERROR "Platform undefined or unsupported.")
endif()

include(platform/${PLATFORM}/CMakeLists.txt)
list(APPEND SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.c
)
list(APPEND INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/include
)

project(neoboot C ASM)

add_executable(${PROJECT_NAME}-${PLATFORM}.elf ${SOURCES})
set_target_properties(${PROJECT_NAME}-${PLATFORM}.elf PROPERTIES
    INCLUDE_DIRECTORIES ${INCLUDES}
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

add_dependencies(${PROJECT_NAME}-${PLATFORM}.elf linkerscript)

add_custom_command(
    TARGET ${PROJECT_NAME}-${PLATFORM}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}-${PLATFORM}.elf ${PROJECT_NAME}-${PLATFORM}.bin
)