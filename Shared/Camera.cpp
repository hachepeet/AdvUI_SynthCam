// Implements camera capture and hand tracking support.
#include "Camera.h"
#include "CameraView.h"

Camera::Camera()
{
    view = std::make_unique<CameraView>();
    addAndMakeVisible(*view);
}

Camera::~Camera() = default;

void Camera::setHandPositionCallback(std::function<void(float, float, bool, bool)> callback)
{
    view->setHandPositionCallback(std::move(callback));
}

void Camera::resized()
{
    view->setBounds(getLocalBounds());
}
