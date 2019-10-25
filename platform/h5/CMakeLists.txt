include(cpu/armv8a/CMakeLists.txt)
include(driver/sunxi_uart/CMakeLists.txt)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake" CACHE PATH "toolchain file")