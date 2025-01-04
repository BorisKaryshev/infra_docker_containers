cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(project_name corutine_test)

if(NOT DEFINED BUILD_MODE)
    set(BUILD_MODE "DEBUG")
endif()

project(${project_name})

set(Boost_USE_STATIC_LIBS   ON)
set(libs
    Boost::program_options
    Boost::fiber
)

find_package(Boost 1.85.0 REQUIRED COMPONENTS fiber program_options HINTS /usr/lib/boost)

set(source
    ${PROJECT_SOURCE_DIR}/src/coruteine_test.cpp
)

set(include_locations
    ${PROJECT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

set(includes
    ${PROJECT_SOURCE_DIR}/include/corutine_test.hpp
)

add_custom_target(
    format
    COMMAND clang-format
    -i
    --style=file:${PROJECT_SOURCE_DIR}/configs/.clang-format
    ${includes} ${source} ${PROJECT_SOURCE_DIR}/src/main.cpp
)

add_library(${project_name}_lib STATIC ${source})
target_include_directories(${project_name}_lib PUBLIC ${include_locations})
target_link_libraries(${project_name}_lib ${libs})

add_executable(${project_name} ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_dependencies(${project_name} format)
target_include_directories(${project_name} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${project_name} ${project_name}_lib)
add_custom_target(
    exe
    DEPENDS ${project_name}
    COMMAND ${CMAKE_BINARY_DIR}/${project_name}
)

target_compile_options(${project_name} PRIVATE -Wall -Wextra -Werror -Wpedantic -pthread)
target_compile_options(${project_name}_lib PRIVATE  -Wall -Wextra -Werror -Wpedantic -pthread)

target_compile_options(${project_name} PUBLIC -pthread)
target_compile_options(${project_name}_lib PUBLIC -pthread)

if(${BUILD_MODE} STREQUAL "DEBUG")
    target_compile_options(${project_name} PRIVATE  -g3 -O0)
    target_compile_options(${project_name}_lib PRIVATE  -g3 -O0)
elseif(${BUILD_MODE} STREQUAL "RELEASE")
    target_compile_options(${project_name} PRIVATE  -O2)
    target_compile_options(${project_name}_lib PRIVATE  -O2)
else()
    message(FATAL_ERROR "Unknow build mode")
endif()
