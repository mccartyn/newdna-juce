cmake_minimum_required(VERSION 3.27)

project(audio-plugins)

set(CMAKE_CXX_STANDARD 17)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

include(cmake/cpm.cmake)

CPMAddPackage(
        NAME juce
        GITHUB_REPOSITORY juce-framework/JUCE
        GIT_TAG 7.0.7
        SOURCE_DIR ${LIB_DIR}/juce
        OPTIONS
        "JUCE_BUILD_EXAMPLES OFF"
        "JUCE_BUILD_EXTRAS OFF"
        "JUCE_BUILD_TESTS OFF")

CPMAddPackage(
    NAME include(GoogleTest)
    GITHUB_REPOSITORY google/googletest
    VERSION 1.13.0
    SOURCE_DIR ${LIB_DIR}/googletest
    OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"
)
if (APPLE)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory(plugins)