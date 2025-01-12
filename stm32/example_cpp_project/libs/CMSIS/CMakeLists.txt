cmake_minimum_required(VERSION 3.9)

set(lib_name cmsis_lib)
project(${lib_name})

set(source ${PROJECT_SOURCE_DIR}/src/system_stm32f1xx.c)
set(include_locations ${PROJECT_SOURCE_DIR}/include)

add_library(${lib_name} STATIC ${source})
target_include_directories(${lib_name} PUBLIC ${include_locations})
