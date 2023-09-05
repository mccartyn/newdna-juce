cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "newDNA-reverb")
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for macOS")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum macOS version required")

project("${PROJECT_NAME}" VERSION 1.0.0)

set(formats "VST3")

if(APPLE)
    list(APPEND formats "AU")
endif()

juce_add_plugin("${PROJECT_NAME}"
    VERSION "1.0.0"
    COMPANY_NAME "newDNA"
    COMPANY_WEBSITE "https://newDNA.net"
    COMPANY_EMAIL "mccartyn@gmail.com"
    PLUGIN_MANUFACTURER_CODE "newD"
    PLUGIN_CODE "newR"
    FORMATS ${formats}
    COPY_PLUGIN_AFTER_BUILD TRUE
    IS_SYNTH FALSE     
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    PRODUCT_NAME "${PROJECT_NAME}"
    PLUGIN_NAME "newDNA Reverb")

juce_generate_juce_header("${PROJECT_NAME}")

target_sources("${PROJECT_NAME}"
    PRIVATE
        source/ReverbProcessor.cpp
        source/ReverbEditor.cpp
        source/EditorContent.cpp
        source/Dial.cpp
        source/EditorContent.cpp
        source/EditorLnf.cpp
        source/FreezeButton.cpp
    )
target_include_directories("${PROJECT_NAME}"
    PRIVATE
        include
        "../../libs/newDNA/newdna_fx"
    )

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_20)

target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_sources("${PROJECT_NAME}"
    PRIVATE
        Resources/FuturaMedium.cpp)

juce_add_binary_data(BinaryData
    SOURCES
        Resources/FreezeIcon.svg)

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
        BinaryData
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
