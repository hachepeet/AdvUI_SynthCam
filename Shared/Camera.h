#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <functional>

class CameraView;

class Camera final : public juce::Component
{
public:
    Camera();
    ~Camera() override;

    void setHandPositionCallback(std::function<void(float, float, bool, bool)> callback);

    void resized() override;

private:
    std::unique_ptr<CameraView> view;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Camera)
};
