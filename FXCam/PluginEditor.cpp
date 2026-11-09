#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    view = std::make_unique<FX>();
    addAndMakeVisible(*view);

    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getParameters(), "delayTime", view->getDelaySlider());
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getParameters(), "dryWet", view->getDryWetSlider());
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getParameters(), "roomSize", view->getRoomSizeSlider());
    reverbDryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getParameters(), "reverbDryWet", view->getReverbDryWetSlider());
    effectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processorRef.getParameters(), "activeEffect", view->getEffectSelector());

    view->setHandPositionCallback([this](float x, float y, bool, bool)
    {
        auto &parameters = processorRef.getParameters();
        const auto activeEffect = static_cast<int>(parameters.getRawParameterValue("activeEffect")->load());
        const auto invertedY = 1.0f - y;

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
            setParameter("dryWet", invertedY);
        }
        else if (activeEffect == 1)
        {
            setParameter("roomSize", x);
            setParameter("reverbDryWet", invertedY);
        }
    });

    setSize(800, 700);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() = default;

void AudioPluginAudioProcessorEditor::resized()
{
    view->setBounds(getLocalBounds());
}
 