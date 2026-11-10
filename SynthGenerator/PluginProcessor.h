// Declares the synthesizer audio processor.
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "SynthModel.h"
#include <atomic>

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState &getParameters() noexcept { return parameters; }
    void setHandClosed(bool shouldMute) noexcept { handClosed.store(shouldMute); }

private:
    static constexpr double attackTimeSeconds = 0.03;
    static constexpr double releaseTimeSeconds = 0.03;

    juce::AudioProcessorValueTreeState parameters;
    SynthModel model;
    std::atomic<bool> handClosed{true};
    bool wasHandClosed = true;
    int attackSamplesRemaining = 0;
    int attackSamplesTotal = 0;
    int releaseSamplesRemaining = 0;
    int releaseSamplesTotal = 0;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
