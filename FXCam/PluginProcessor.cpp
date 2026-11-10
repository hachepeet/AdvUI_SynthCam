// Implements the effects audio processor.
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
                                                         AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                                                                            .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                                                                            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                                                                            ),

#endif
                                                         parameters(*this, nullptr, juce::Identifier("Sine Synth"),
                                                                    {std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("delayTime", 1), "DelayTime", 1.0f, 1000.0f, 250.0f),
                                                                    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("dryWet", 1), "DryWet", 0.0f, 1.0f, 0.0f),
                                                                    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("roomSize",1), "RoomSize", 0.0f, 1.0f, 0.5f),
                                                                    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("reverbDryWet",1), "ReverbDryWet", 0.0f, 1.0f, 0.5f),
                                                                    std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("activeEffect",1), "ActiveEffect", juce::StringArray{"Delay","Reverb"}, 0)}),
                                                         model(parameters)
{
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return "AudioPluginExample";
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; 
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
// Pre-playback initialization
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    model.prepareToPlay(sampleRate, getTotalNumOutputChannels());
}

void AudioPluginAudioProcessor::releaseResources()
{
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

// Main site of audio processing
void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);
    model.render(buffer);
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; 
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    const auto state = parameters.copyState();
    const std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    const std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

//==============================================================================
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
 