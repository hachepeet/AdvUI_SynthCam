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
                                                                    {// The frequency range follows the logarithmic hand and slider controls.
                                                                     std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("frequency", 1), "Frequency", 220.0f, 880.0f, 440.0f),
                                                                     std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("waveform", 1), "Waveform", 0.0f, 4.0f, 0.0f)}),
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
    return releaseTimeSeconds;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
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
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(samplesPerBlock);
    attackSamplesTotal = juce::jmax(1, juce::roundToInt(sampleRate * attackTimeSeconds));
    releaseSamplesTotal = juce::jmax(1, juce::roundToInt(sampleRate * releaseTimeSeconds));
    attackSamplesRemaining = 0;
    releaseSamplesRemaining = 0;
    wasHandClosed = handClosed.load();
    auto frequency = parameters.getRawParameterValue("frequency")->load();
    auto waveform = parameters.getRawParameterValue("waveform")->load();
    model.prepareToPlay(sampleRate, frequency, waveform);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);
    const auto isHandClosed = handClosed.load();

    if (!isHandClosed)
    {
        if (wasHandClosed)
            attackSamplesRemaining = attackSamplesTotal;

        wasHandClosed = false;
        releaseSamplesRemaining = 0;
        auto frequency = parameters.getRawParameterValue("frequency")->load();
        auto waveform = parameters.getRawParameterValue("waveform")->load();
        if (attackSamplesRemaining <= 0)
        {
            model.render(buffer, frequency, waveform);
            return;
        }

        const auto samplesToAttack = juce::jmin(attackSamplesRemaining, buffer.getNumSamples());
        const auto gainStart = 1.0f
                             - static_cast<float>(attackSamplesRemaining)
                                   / static_cast<float>(attackSamplesTotal);
        const auto gainEnd = 1.0f
                           - static_cast<float>(attackSamplesRemaining - samplesToAttack)
                                 / static_cast<float>(attackSamplesTotal);
        model.render(buffer, frequency, waveform, gainStart, gainEnd, samplesToAttack);
        attackSamplesRemaining -= samplesToAttack;
        return;
    }

    if (!wasHandClosed)
    {
        attackSamplesRemaining = 0;
        releaseSamplesRemaining = releaseSamplesTotal;
        wasHandClosed = true;
    }

    if (releaseSamplesRemaining <= 0)
    {
        buffer.clear();
        return;
    }

    auto frequency = parameters.getRawParameterValue("frequency")->load();
    auto waveform = parameters.getRawParameterValue("waveform")->load();
    const auto samplesToRelease = juce::jmin(releaseSamplesRemaining, buffer.getNumSamples());
    const auto gainStart = static_cast<float>(releaseSamplesRemaining)
                         / static_cast<float>(releaseSamplesTotal);
    const auto gainEnd = static_cast<float>(releaseSamplesRemaining - samplesToRelease)
                       / static_cast<float>(releaseSamplesTotal);
    model.render(buffer, frequency, waveform, gainStart, gainEnd, samplesToRelease);
    releaseSamplesRemaining -= samplesToRelease;
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
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
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
