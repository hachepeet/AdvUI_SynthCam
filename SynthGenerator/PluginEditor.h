// Declares the synthesizer plugin editor.
#pragma once

#include "PluginProcessor.h"
#include "Synth.h"

class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    void resized() override;

private:
    // The editor coordinates the model and view; the view owns presentation.
    AudioPluginAudioProcessor &processorRef;
    std::unique_ptr<Synth> view;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> waveAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
