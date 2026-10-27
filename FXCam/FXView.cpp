#include "FXView.h"
#include "Theme.h"

FXView::FXView()
{
    addAndMakeVisible(cameraView);
    addAndMakeVisible(delaySlider);
    addAndMakeVisible(dryWetSlider);

    // for (auto *slider : {&frequencySlider, &volumeSlider})
    // {
    //     slider->setSliderStyle(juce::Slider::LinearHorizontal);
    //     slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    // }

    // frequencySlider.setRange(50.0, 5000.0);
    // frequencySlider.setSkewFactorFromMidPoint(500.0);
    // volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    // volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    // volumeSlider.setRange(0.0, 1.0);

    delaySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delaySlider.setRange(1.0, 1000.0, 1.0);
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);

    dryWetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dryWetSlider.setRange(0.0, 1.0, 0.0);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
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
}

// juce::Slider &FXView::getFrequencySlider() noexcept
// {
//     return frequencySlider;
// }

// juce::Slider &FXView::getVolumeSlider() noexcept
// {
//     return volumeSlider;
// }

juce::Slider &FXView::getDelaySlider() noexcept
{
    return delaySlider;
}

juce::Slider &FXView::getDryWetSlider() noexcept
{
    return dryWetSlider;
}