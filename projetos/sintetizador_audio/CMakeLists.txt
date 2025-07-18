# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(sintetizador_audio C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(sintetizador_audio
    gravador.c
    drivers/ssd1306_i2c.c
)

pico_set_program_name(sintetizador_audio "sintetizador_audio")
pico_set_program_version(sintetizador_audio "0.1")

# no_flash means the target is to run from RAM
pico_set_binary_type(sintetizador_audio no_flash)

# Generate PIO header
#pico_generate_pio_header(sintetizador_audio ${CMAKE_CURRENT_LIST_DIR}/drivers/ws2818b.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(sintetizador_audio 1)
pico_enable_stdio_usb(sintetizador_audio 1)

# Add the standard include files to the build
target_include_directories(sintetizador_audio PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add the standard library to the build
target_link_libraries(sintetizador_audio
    pico_stdlib
    hardware_adc
    hardware_dma
    hardware_pwm
    hardware_gpio
    hardware_i2c
    pico_multicore
)

pico_add_extra_outputs(sintetizador_audio)

