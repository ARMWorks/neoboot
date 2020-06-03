include(drivers/sunxi_ddr3/CMakeLists.txt)
include(drivers/sunxi_uart/CMakeLists.txt)
list(APPEND DEFINES
    -DPLATFORM_SUNXI_H3
    -DCONFIG_DRAM_CLK=408
)