#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Shared/Camera.h"
#include <functional>

class FXView;

class FX final : public juce::Component
{
public:
    FX();
    ~FX() override;

    juce::Slider &getDelaySlider() noexcept;
    juce::Slider &getDryWetSlider() noexcept;
    juce::Slider &getRoomSizeSlider() noexcept;
    juce::Slider &getReverbDryWetSlider() noexcept;
    juce::ComboBox &getEffectSelector() noexcept;
    void setHandPositionCallback(std::function<void(float, float, bool, bool)> callback);

    void resized() override;

private:
    std::unique_ptr<FXView> view;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FX)
};
