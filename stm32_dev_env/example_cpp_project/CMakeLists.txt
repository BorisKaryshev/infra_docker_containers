cmake_minimum_required(VERSION 3.9)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m3)


add_definitions(-DSTM32F103x6 -DSTM32 -DSTM32F1)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb")
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp -mcpu=cortex-m3 -MMD -MP --specs=nano.specs -mfloat-abi=soft -mthumb")
set(CMAKE_C_FLAGS "-ffunction-sections -mcpu=cortex-m3 -fdata-sections -Wall -fstack-usage -MMD -MP --specs=nosys.specs --specs=nano.specs -mfloat-abi=soft")
set(CMAKE_CXX_FLAGS "-ffunction-sections -fno-exceptions -mcpu=cortex-m3 -fdata-sections -Wall -fstack-usage -MMD -MP --specs=nosys.specs --specs=nano.specs -mfloat-abi=soft")
set(LINK_FLAGS "--specs=nosys.specs -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group")

set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(project_name stm32_project)

if(NOT DEFINED BUILD_MODE)
    set(BUILD_MODE "DEBUG")
endif()

project(${project_name} CXX C ASM)

set(source
    ${PROJECT_SOURCE_DIR}/src/base_functions.cpp
    ${PROJECT_SOURCE_DIR}/src/syscalls.c
    ${PROJECT_SOURCE_DIR}/src/sysmem.c
)

set(include_locations
    ${PROJECT_SOURCE_DIR}/include
    /usr/include/c++/12
    /usr/include/c++/13
)

set(includes
    ${PROJECT_SOURCE_DIR}/include/base_functions.hpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/CMSIS)

add_custom_target(
    format
    COMMAND clang-format
    -i
    --style=file:${PROJECT_SOURCE_DIR}/configs/.clang-format
    ${includes} ${source} ${PROJECT_SOURCE_DIR}/src/main.cpp
)
add_library(${project_name}_lib STATIC ${source} ${PROJECT_SOURCE_DIR}/src/startup/startup_stm32f103rbtx.s)
target_include_directories(${project_name}_lib PUBLIC ${include_locations})
target_link_libraries(${project_name}_lib cmsis_lib)

add_executable(${project_name} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${project_name} -T${CMAKE_SOURCE_DIR}/LINKER_SCRIPT.ld)

target_link_libraries(${project_name} ${project_name}_lib)
add_custom_target(
    exe
    DEPENDS ${project_name}
)

if(${BUILD_MODE} STREQUAL "DEBUG")
    target_compile_options(${project_name} PRIVATE -g3 -Os)
    target_compile_options(${project_name}_lib PRIVATE -g3 -Os)
    add_definitions(-DDEBUG)
elseif(${BUILD_MODE} STREQUAL "RELEASE")
    target_compile_options(${project_name} PRIVATE -Os)
    target_compile_options(${project_name}_lib PRIVATE -Os)
    add_definitions(-DRELEASE)
else()
    message(FATAL_ERROR "Unknow build mode")
endif()
