cmake_minimum_required(VERSION 3.15)

project(audio-plugin)

enable_testing()

add_executable("audio-plugin"
    source/AudioProcessorTest.cpp )

target_include_directories("audio-plugin"
    PRIVATE
        ${GOOGLETEST_SOURCE_DIR}/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../plugin/include
)        
target_link_libraries("audio-plugin"
    PRIVATE
        audio-plugin
        Gtest::gtest_main)