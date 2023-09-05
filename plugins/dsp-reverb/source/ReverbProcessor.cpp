
#include "ReverbProcessor.h"
#include "ReverbEditor.h"
#include "ParamIDs.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto percentFormat = [](float value, int /*maximumStringLength*/)
    {
        if (value < 10.0f)
            return juce::String (value, 2) + " %";
        else if (value < 100.0f)
            return juce::String (value, 1) + " %";
        else
            return juce::String (value, 0) + " %";
    };

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::size, 1 },
                                                             ParamIDs::size,
                                                             juce::NormalisableRange<float> { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::damp, 1 },
                                                             ParamIDs::damp,
                                                             juce::NormalisableRange<float> { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::width, 1 },
                                                             ParamIDs::width,
                                                             juce::NormalisableRange<float> { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::mix, 1 },
                                                             ParamIDs::mix,
                                                             juce::NormalisableRange<float> { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterBool> (juce::ParameterID { ParamIDs::freeze, 1 }, 
                                                            ParamIDs::freeze, 
                                                            false));

    return layout;
}

ReverbAudioProcessor::ReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
       apvts (*this, &undoManager, "Parameters", createParameterLayout())  
{
    auto storeFloatParam = [&apvts = this->apvts](auto& param, const auto& paramID)
    {
        param = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter (paramID));
        jassert (param != nullptr);
    };

    storeFloatParam (size,  ParamIDs::size); 
    storeFloatParam (damp,  ParamIDs::damp); 
    storeFloatParam (width, ParamIDs::width); 
    storeFloatParam (mix,   ParamIDs::mix); 

    auto storeBoolParam = [&apvts = this->apvts](auto& param, const auto& paramID)
    {
        param = dynamic_cast<juce::AudioParameterBool*> (apvts.getParameter (paramID));
        jassert (param != nullptr);
    };

    storeBoolParam (freeze, ParamIDs::freeze); 
}

ReverbAudioProcessor::~ReverbAudioProcessor()
{
}

const juce::String ReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String ReverbAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void ReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void ReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //newDNA::dsp::machineConfig machine_config;

    //machine_config.sampleRate       = sampleRate;
    //machine_config.maximumBlockSize = static_cast<uint32> (samplesPerBlock);
    //machine_config.numChannels      = static_cast<uint32> (getTotalNumOutputChannels());
    juce::ignoreUnused(samplesPerBlock);
    reverb.setSampleRate (sampleRate);
}

void ReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ReverbAudioProcessor::updateReverbParams()
{
    params.roomSize   = size->get() * 0.01f;
    params.damping    = damp->get() * 0.01f;
    params.width      = width->get() * 0.01f;
    params.wetLevel   = mix->get() * 0.01f;
    params.dryLevel   = 1.0f - mix->get() * 0.01f;
    params.freezeMode = freeze->get();

    reverb.setParameters (params);
}

void ReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    updateReverbParams();

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);

    // juce specific audio buffer formatting
    const auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();
    const auto numInChannels = inputBlock.getNumChannels();
    const auto numOutChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    jassert (inputBlock.getNumSamples() == numSamples);

    outputBlock.copyFrom (inputBlock);

    if (/*! enabled || */ context.isBypassed)
        return;

    if (numInChannels == 1 && numOutChannels == 1)
    {
        reverb.processMono (outputBlock.getChannelPointer (0), (int) numSamples);
    }
    else if (numInChannels == 2 && numOutChannels == 2)
    {
        reverb.processStereo (outputBlock.getChannelPointer (0),
                              outputBlock.getChannelPointer (1),
                              (int) numSamples);
    }
    else
    {
        jassertfalse;   // invalid channel configuration
    }
}

bool ReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbAudioProcessor::createEditor()
{
    return new ReverbAudioProcessorEditor (*this, undoManager);
}

void ReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}

void ReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data,
                                               static_cast<size_t> (sizeInBytes));

    if (tree.isValid())
        apvts.replaceState (tree);
}

juce::AudioProcessorValueTreeState& ReverbAudioProcessor::getPluginState() { return apvts; }

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAudioProcessor();
}

