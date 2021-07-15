cmake_minimum_required(VERSION 3.16)
project(Robot-Sorting-Algorithm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB TARGET_SRC "./src/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

set(EXECUTABLE_NAME "Robot-Sorting")
include_directories(${CMAKE_HOME_DIRECTORY}/include)
add_executable(${EXECUTABLE_NAME} ${TARGET_SRC})

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic)