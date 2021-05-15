cmake_minimum_required(VERSION 3.16)
project(AAL_CMYK)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB TARGET_SRC "./src/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

set(EXECUTABLE_NAME "aal_cmyk")
include_directories(${CMAKE_HOME_DIRECTORY}/include)
add_executable(${EXECUTABLE_NAME} ${TARGET_SRC})

if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4)
else()
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

#Add tests -> building program for testing with boost
set(BUILD_TESTS True) #Set to "True" for test for building tests
if(BUILD_TESTS)
    message("Building tests.")
    list(FILTER TARGET_SRC EXCLUDE REGEX /*/main.cpp)
    if (UNIX)
        find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    endif ()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)

    add_executable(tests ${CMAKE_HOME_DIRECTORY}/tests/UnitTests.cpp ${TARGET_SRC} tests/UnitTests.cpp)

    if (UNIX)
        target_link_libraries(tests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    endif ()

    add_test(UnitTest tests)
    enable_testing()
endif()

# set (BUILD_DOCS True) #set TRUE if you want to build docs
# if(BUILD_DOCS)
#     find_package(Doxygen)
#     option(BUILD_DOCUMENTATION "Create and install the HTML based API
#     documentation (requires Doxygen)" ${DOXYGEN_FOUND})

#     if(NOT DOXYGEN_FOUND)
#         message(FATAL_ERROR "Doxygen is needed to build the documentation.")
#     endif()

#     set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
#     set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

#     configure_file(${doxyfile_in} ${doxyfile} @ONLY)

#     message("Doxygen build started.")

#     add_custom_target(docs
#             COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_in}
#             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
#             COMMENT "Generating API documentation with Doxygen"
#             VERBATIM)
# endif()