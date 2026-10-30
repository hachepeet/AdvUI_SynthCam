#include "FXView.h"
#include "Theme.h"

FXView::FXView()
{
    addAndMakeVisible(effectSelector);
    effectSelector.addItem("Delay",1);
    effectSelector.addItem("Reverb",2);
    effectSelector.setSelectedId(1);

    effectSelector.onChange = [this]
    {
        delaySelected = effectSelector.getSelectedId() == 1;
        delaySlider.setVisible(delaySelected);
        dryWetSlider.setVisible(delaySelected);
        roomSizeSlider.setVisible(!delaySelected);
        reverbDryWetSlider.setVisible(!delaySelected);
    };

    addAndMakeVisible(cameraView);

    addAndMakeVisible(delaySlider);
    addAndMakeVisible(dryWetSlider);
    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(reverbDryWetSlider);

    for (auto *slider : {&delaySlider, &dryWetSlider, &roomSizeSlider, &reverbDryWetSlider})
    {
        slider->setSliderStyle(juce::Slider::LinearHorizontal);
        slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    }

    delaySlider.setRange(1.0, 1000.0, 1.0);
    dryWetSlider.setRange(0.0, 1.0, 0.0);
    roomSizeSlider.setRange(0.0, 1.0, 0.0);
    reverbDryWetSlider.setRange(0.0, 1.0, 0.0);

}

void FXView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(SynthCamTheme::backgroundColour);
}

void FXView::resized()
{
    auto area = getLocalBounds().reduced(20);
    effectSelector.setBounds(area.removeFromTop(40).removeFromLeft(180));
    cameraView.setBounds(area.removeFromTop(220));
    delaySlider.setBounds(area.removeFromTop(100));
    dryWetSlider.setBounds(area.removeFromTop(100));
    roomSizeSlider.setBounds(area.removeFromTop(100));
    reverbDryWetSlider.setBounds(area.removeFromTop(100));
}

juce::Slider &FXView::getDelaySlider() noexcept
{
    return delaySlider;
}

juce::Slider &FXView::getDryWetSlider() noexcept
{
    return dryWetSlider;
}

juce::Slider &FXView::getRoomSizeSlider() noexcept
{
    return roomSizeSlider;
}

juce::Slider &FXView::getReverbDryWetSlider() noexcept
{
    return reverbDryWetSlider;
}

juce::ComboBox &FXView::getEffectSelector() noexcept
{
    return effectSelector;
}
