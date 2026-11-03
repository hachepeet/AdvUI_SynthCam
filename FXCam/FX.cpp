#include "FX.h"
#include "FXView.h"

FX::FX()
{
    view = std::make_unique<FXView>();
    addAndMakeVisible(*view);
}

FX::~FX() = default;

juce::Slider &FX::getDelaySlider() noexcept
{
    return view->getDelaySlider();
}

juce::Slider &FX::getDryWetSlider() noexcept
{
    return view->getDryWetSlider();
}

juce::Slider &FX::getRoomSizeSlider() noexcept
{
    return view->getRoomSizeSlider();
}

juce::Slider &FX::getReverbDryWetSlider() noexcept
{
    return view->getReverbDryWetSlider();
}

juce::ComboBox &FX::getEffectSelector() noexcept
{
    return view->getEffectSelector();
}

void FX::setHandPositionCallback(std::function<void(float, float, bool, bool)> callback)
{
    view->setHandPositionCallback(std::move(callback));
}

void FX::resized()
{
    view->setBounds(getLocalBounds());
}
