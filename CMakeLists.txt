cmake_minimum_required(VERSION 4.0)
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/pico-sdk)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
project(pico_nvme C CXX)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()
add_executable(pico_nvme main.c)
target_link_libraries(pico_nvme
        pico_stdlib
        hardware_gpio
        hardware_adc
)
pico_enable_stdio_usb(pico_nvme 1)
pico_enable_stdio_uart(pico_nvme 0)
pico_add_extra_outputs(pico_nvme)