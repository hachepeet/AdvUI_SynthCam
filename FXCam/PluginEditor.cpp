#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    view = std::make_unique<FXView>();
    addAndMakeVisible(*view);

    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "delayTime", view->getDelaySlider());
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "dryWet", view->getDryWetSlider());
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "roomSize", view->getRoomSizeSlider());
    reverbDryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "reverbDryWet", view->getReverbDryWetSlider());
    effectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processorRef.getModel().getParameters(), "activeEffect", view->getEffectSelector());

    setSize(800, 800);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() = default;

void AudioPluginAudioProcessorEditor::resized()
{
    view->setBounds(getLocalBounds());
}
 