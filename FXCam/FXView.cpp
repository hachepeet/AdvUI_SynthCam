// Implements the effects interface.
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
        delayMinLabel.setVisible(delaySelected);
        delayMaxLabel.setVisible(delaySelected);
        delayAxisLabel.setVisible(delaySelected);
        roomSizeSlider.setVisible(!delaySelected);
        reverbDryWetSlider.setVisible(!delaySelected);
        roomMinLabel.setVisible(!delaySelected);
        roomMaxLabel.setVisible(!delaySelected);
        roomAxisLabel.setVisible(!delaySelected);
    };

    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(camera);

    addAndMakeVisible(delaySlider);
    addAndMakeVisible(dryWetSlider);
    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(reverbDryWetSlider);

    addAndMakeVisible(delayMinLabel);
    addAndMakeVisible(delayMaxLabel);
    addAndMakeVisible(delayAxisLabel);
    addAndMakeVisible(dryWetMinLabel);
    addAndMakeVisible(dryWetMaxLabel);
    addAndMakeVisible(dryWetAxisLabel);

    addAndMakeVisible(roomMinLabel);
    addAndMakeVisible(roomMaxLabel);
    addAndMakeVisible(roomAxisLabel);
    addAndMakeVisible(revDWMinLabel);
    addAndMakeVisible(revDWMaxLabel);
    addAndMakeVisible(revDWAxisLabel);

    pluginTitle.setText("FXCam", juce::dontSendNotification);
    pluginTitle.setFont(juce::Font(24.0f));
    pluginTitle.setJustificationType(juce::Justification::centred);

    for (auto *slider : {&delaySlider, &roomSizeSlider})
    {
        slider->setSliderStyle(juce::Slider::LinearHorizontal);
        slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    }

    for (auto *slider : {&dryWetSlider, &reverbDryWetSlider})
    {
        slider->setSliderStyle(juce::Slider::LinearVertical);
        slider->setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    }

    delaySlider.setRange(1.0, 1000.0, 1.0);
    dryWetSlider.setRange(0.0, 1.0, 0.0);
    roomSizeSlider.setRange(0.0, 1.0, 0.0);
    reverbDryWetSlider.setRange(0.0, 1.0, 0.0);

    delayMinLabel.setText(juce::String(delaySlider.getMinimum()), juce::dontSendNotification);
    delayMaxLabel.setText(juce::String(delaySlider.getMaximum()), juce::dontSendNotification);
    delayAxisLabel.setText("Delay Time (ms)", juce::dontSendNotification);
    dryWetMinLabel.setText(juce::String(dryWetSlider.getMinimum()), juce::dontSendNotification);
    dryWetMaxLabel.setText(juce::String(dryWetSlider.getMaximum()), juce::dontSendNotification);
    dryWetAxisLabel.setText("Dry / Wet", juce::dontSendNotification);

    delayMinLabel.setJustificationType(juce::Justification::centredLeft);
    delayMaxLabel.setJustificationType(juce::Justification::centredRight);
    dryWetMinLabel.setJustificationType(juce::Justification::centredLeft);
    dryWetMaxLabel.setJustificationType(juce::Justification::centredLeft);

    roomMinLabel.setText(juce::String(roomSizeSlider.getMinimum()), juce::dontSendNotification);
    roomMaxLabel.setText(juce::String(roomSizeSlider.getMaximum()), juce::dontSendNotification);
    roomAxisLabel.setText("Room Size", juce::dontSendNotification);
    revDWMinLabel.setText(juce::String(reverbDryWetSlider.getMinimum()), juce::dontSendNotification);
    revDWMaxLabel.setText(juce::String(reverbDryWetSlider.getMaximum()), juce::dontSendNotification);
    revDWAxisLabel.setText("Dry / Wet", juce::dontSendNotification);

    roomMinLabel.setJustificationType(juce::Justification::centredLeft);
    roomMaxLabel.setJustificationType(juce::Justification::centredRight);
    revDWMinLabel.setJustificationType(juce::Justification::centredLeft);
    revDWMaxLabel.setJustificationType(juce::Justification::centredLeft);

}

void FXView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(SynthCamTheme::backgroundColour);
}

void FXView::resized()
{
    auto area = getLocalBounds().reduced(20);
    pluginTitle.setBounds(area.removeFromTop(30));
    effectSelector.setBounds(area.removeFromTop(40).removeFromLeft(180));
    camera.setBounds(area.removeFromTop(220));

    int verticalSliderWidth = 40;
    int verticalSliderHeight = 240;
    int horizontalSliderWidth = 300;
    int horizontalSliderHeight = 40;
    auto sliderBounds = area.withSizeKeepingCentre(verticalSliderWidth + horizontalSliderWidth, verticalSliderHeight + horizontalSliderHeight);

    dryWetSlider.setBounds(sliderBounds.getX(), sliderBounds.getY(), verticalSliderWidth, verticalSliderHeight);
    dryWetMaxLabel.setBounds(dryWetSlider.getX() - 55, dryWetSlider.getY(), 50, 20);
    dryWetMinLabel.setBounds(dryWetSlider.getX() - 55, dryWetSlider.getBottom() - 20, 50, 20);
    dryWetAxisLabel.setBounds(dryWetSlider.getX() - 110, dryWetSlider.getY() + dryWetSlider.getHeight() / 2 - 10, 100, 20);
    
    delaySlider.setBounds(sliderBounds.getX() + verticalSliderWidth, sliderBounds.getBottom() - horizontalSliderHeight, horizontalSliderWidth, horizontalSliderHeight);
    delayMinLabel.setBounds(delaySlider.getX(), delaySlider.getBottom(), 50, 20);
    delayMaxLabel.setBounds(delaySlider.getRight() - 50, delaySlider.getBottom() + 5, 50, 20);
    delayAxisLabel.setBounds(delaySlider.getX(), delaySlider.getBottom() + 30, delaySlider.getWidth(), 20);

    reverbDryWetSlider.setBounds(sliderBounds.getX(), sliderBounds.getY(), verticalSliderWidth, verticalSliderHeight);
    revDWMaxLabel.setBounds(reverbDryWetSlider.getX() - 55, reverbDryWetSlider.getY(), 50, 20);
    revDWMinLabel.setBounds(reverbDryWetSlider.getX() - 55, reverbDryWetSlider.getBottom() - 20, 50, 20);
    revDWAxisLabel.setBounds(reverbDryWetSlider.getX() - 110, reverbDryWetSlider.getY() + dryWetSlider.getHeight() / 2 - 10, 100, 20);

    roomSizeSlider.setBounds(sliderBounds.getX() + verticalSliderWidth, sliderBounds.getBottom() - horizontalSliderHeight, horizontalSliderWidth, horizontalSliderHeight);
    roomMinLabel.setBounds(roomSizeSlider.getX(), roomSizeSlider.getBottom(), 50, 20);
    roomMaxLabel.setBounds(roomSizeSlider.getRight() - 50, roomSizeSlider.getBottom() + 5, 50, 20);
    roomAxisLabel.setBounds(roomSizeSlider.getX(), roomSizeSlider.getBottom() + 30, roomSizeSlider.getWidth(), 20);

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

void FXView::setHandPositionCallback(std::function<void(float, float, bool, bool)> callback)
{
    camera.setHandPositionCallback(std::move(callback));
}
