#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    view = std::make_unique<FXView>();
    addAndMakeVisible(*view);

    // fqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "frequency", view->getFrequencySlider());
    volAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "volume", view->getVolumeSlider());

    setSize(400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() = default;

void AudioPluginAudioProcessorEditor::resized()
{
    view->setBounds(getLocalBounds());
}
