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

project(teste_unitario_temp C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(teste_unitario_temp 
        app/main.c
        hal/hal_temp.c
        drivers/adc_temp.c
)

pico_set_program_name(teste_unitario_temp "teste_unitario_temp")
pico_set_program_version(teste_unitario_temp "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(teste_unitario_temp 0)
pico_enable_stdio_usb(teste_unitario_temp 0)

# Add the standard library to the build
target_link_libraries(teste_unitario_temp
        pico_stdlib 
        hardware_adc
        )

# Add the standard include files to the build
target_include_directories(teste_unitario_temp PRIVATE 
        include
        lib/unity
)
pico_add_extra_outputs(teste_unitario_temp)

# --- TESTE UNITÁRIO COM UNITY ---
add_executable(test_adc
    tests/test_adc.c
    drivers/adc_temp.c
    lib/unity/unity.c
)
target_include_directories(test_adc PRIVATE include lib/unity)
target_link_libraries(test_adc pico_stdlib)
pico_add_extra_outputs(test_adc)


