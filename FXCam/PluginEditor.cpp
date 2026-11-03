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

    view->setHandPositionCallback([this](float x, float y, bool, bool)
    {
        auto &parameters = processorRef.getModel().getParameters();
        const auto activeEffect = static_cast<int>(parameters.getRawParameterValue("activeEffect")->load());

        auto setParameter = [&parameters](const juce::String &parameterId, float value)
        {
            if (auto *parameter = dynamic_cast<juce::RangedAudioParameter *>(
                    parameters.getParameter(parameterId)))
            {
                parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
            }
        };

        if (activeEffect == 0)
        {
            setParameter("delayTime", juce::jmap(x, 1.0f, 1000.0f));
            setParameter("dryWet", y);
        }
        else if (activeEffect == 1)
        {
            setParameter("roomSize", x);
            setParameter("reverbDryWet", y);
        }
    });

    setSize(800, 800);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() = default;

void AudioPluginAudioProcessorEditor::resized()
{
    view->setBounds(getLocalBounds());
}
 