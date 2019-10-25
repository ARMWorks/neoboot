cmake_minimum_required(VERSION 2.8)

option(PLATFORM "Platform to build bootloader for." "")
if(NOT IS_DIRECTORY "${CMAKE_SOURCE_DIR}/platform/${PLATFORM}/")
    message(FATAL_ERROR "Platform undefined or unsupported.")
endif()

list(APPEND SOURCES
    main.c
)

include(platform/${PLATFORM}/CMakeLists.txt)
project(neoboot C ASM)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(IS_RELEASE_VERSION FALSE)

if(IS_RELEASE_VERSION)
    set(PROJECT_VERSION_TWEAK 0)
    set(VERSION_REVISION "")
elseif (EXISTS "${CMAKE_SOURCE_DIR}/.git")
    find_package(Git)
    if(Git_FOUND)
        execute_process(
            COMMAND git rev-list --count HEAD ^tags/v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE TWEAK_RESULT
            OUTPUT_VARIABLE PROJECT_VERSION_TWEAK
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND git rev-parse --short --verify HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE VERSION_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND git diff-index --quiet HEAD --
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE VERSION_COMMIT_DIRTY
        )
        if(VERSION_COMMIT_DIRTY)
            set(DIRTY_POSTFIX "-dirty")
        else()
            set(DIRTY_POSTFIX "")
        endif()
        if(NOT TWEAK_RESULT EQUAL 0)
            # Happens on a shallow git clone, like Travis does. Append date to version ref.
            set(PROJECT_VERSION_TWEAK 0)
            string(TIMESTAMP TODAY "%Y%m%d")
            set(VERSION_REVISION "-${TODAY}-${VERSION_COMMIT_HASH}${DIRTY_POSTFIX}")
        else()
            set(VERSION_REVISION ".${PROJECT_VERSION_TWEAK}-${VERSION_COMMIT_HASH}${DIRTY_POSTFIX}")
        endif()
    else()
        set(PROJECT_VERSION_TWEAK 0)
        set(VERSION_REVISION " unknown development version")
    endif()
else()
    set(PROJECT_VERSION_TWEAK 0)
    set(VERSION_REVISION " unknown development version")
endif()

add_executable(${PROJECT_NAME}-${PLATFORM}.elf ${SOURCES})
set_target_properties(${PROJECT_NAME}-${PLATFORM}.elf PROPERTIES
    LINK_FLAGS "-Wl,-T${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PLATFORM}.lds,-M=${PROJECT_NAME}-${PLATFORM}.map,--gc-sections")

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PLATFORM}.lds
    MAIN_DEPENDENCY ${LINKER_SCRIPT_SOURCE}
    COMMAND ${CMAKE_C_COMPILER}
        -E ${LINKER_SCRIPT_SOURCE} -P
        -o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PLATFORM}.lds
        -I include
        VERBATIM)

add_custom_target(linkerscript
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PLATFORM}.lds
    VERBATIM)

add_dependencies(${PROJECT_NAME}-${PLATFORM}.elf linkerscript)

add_custom_command(
    TARGET ${PROJECT_NAME}-${PLATFORM}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}-${PLATFORM}.elf ${PROJECT_NAME}-${PLATFORM}.bin
)