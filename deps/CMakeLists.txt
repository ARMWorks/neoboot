# libc
list(APPEND SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/libc/printf/printf.c
    ${CMAKE_CURRENT_LIST_DIR}/libc/src/stdio/putchar.c
    ${CMAKE_CURRENT_LIST_DIR}/libc/src/stdio/puts.c
)

list(APPEND INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/libc/include
    ${CMAKE_CURRENT_LIST_DIR}/libc/printf
    ${CMAKE_CURRENT_LIST_DIR}/libc/arch/${CMAKE_SYSTEM_PROCESSOR}/include
)

# libmemory
list(APPEND SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/libmemory/src/aligned_malloc.c
    ${CMAKE_CURRENT_LIST_DIR}/libmemory/src/malloc_freelist.c
    ${CMAKE_CURRENT_LIST_DIR}/libmemory/src/posix_memalign.c
)

list(APPEND INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/libmemory/include
    ${CMAKE_CURRENT_LIST_DIR}/libmemory/dependencies/lib
)