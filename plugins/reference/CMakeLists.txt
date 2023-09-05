cmake_minimum_required(
    VERSION
    3.23.0
)

project(
    "audio-plugin"
    VERSION
    0.1.0
    LANGUAGES
    CXX
)

juce_add_plugin("audio-plugin"
    COMPANY_NAME
    "newDNA"
    COMPANY_COPYRIGHT
    "newDNA"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    FORMATS VST3
    )
target_sources("audio-plugin"
    PRIVATE
        source/PluginProcessor.cpp
        source/PluginEditor.cpp
    )
target_include_directories("audio-plugin"
    PRIVATE
        include/reverb
    )
target_link_directories("audio-plugin"
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
    )

    target_compile_definitions("audio-plugin"
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        )

