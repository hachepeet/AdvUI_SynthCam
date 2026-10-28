#include "FXView.h"
#include "Theme.h"

FXView::FXView()
{
    addAndMakeVisible(cameraView);
    addAndMakeVisible(delaySlider);
    addAndMakeVisible(dryWetSlider);
    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(reverbDryWetSlider);

    // for (auto *slider : {&delaySlider, &dryWetSlider, &roomSizeSlider, &reverbDryWetSlider})
    // {
    //     slider->setSliderStyle(juce::Slider::LinearHorizontal);
    //     slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    // }

    delaySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delaySlider.setRange(1.0, 1000.0, 1.0);
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);

    dryWetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dryWetSlider.setRange(0.0, 1.0, 0.0);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);

    roomSizeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    roomSizeSlider.setRange(0.0, 1.0, 0.0);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);

    reverbDryWetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    reverbDryWetSlider.setRange(0.0, 1.0, 0.0);
    reverbDryWetSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
}

void FXView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(SynthCamTheme::backgroundColour);
}

void FXView::resized()
{
    auto area = getLocalBounds().reduced(20);
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