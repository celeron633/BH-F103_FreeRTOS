cmake_minimum_required(VERSION 3.22)

project(freertos_kernel)
add_library(freertos_kernel INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(freertos_kernel INTERFACE 
	USE_HAL_DRIVER 
	STM32F103xE
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(freertos_kernel INTERFACE
    ../../FreeRTOS/Inc
    ../../FreeRTOS/Config
)

target_sources(freertos_kernel INTERFACE
    ../../FreeRTOS/Src/croutine.c
    ../../FreeRTOS/Src/event_groups.c
    ../../FreeRTOS/Src/heap_4.c
    ../../FreeRTOS/Src/list.c
    ../../FreeRTOS/Src/port.c
    ../../FreeRTOS/Src/queue.c
    ../../FreeRTOS/Src/stream_buffer.c
    ../../FreeRTOS/Src/tasks.c
    ../../FreeRTOS/Src/timers.c
)

target_link_directories(freertos_kernel INTERFACE
)

target_link_libraries(freertos_kernel INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


