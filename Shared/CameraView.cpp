// Implements the shared camera interface.
#include "CameraView.h"

#include "Theme.h"

namespace
{
constexpr auto cameraAspectRatio = 16.0f / 9.0f;
}

// Inspired by https://github.com/juce-framework/JUCE/blob/29396c22c93392d6738e021b83196283d6e4d850/examples/GUI/CameraDemo.h

CameraView::CameraView()
{
    addAndMakeVisible(cameraSelector);
    addAndMakeVisible(snapshotButton);
    addAndMakeVisible(lastSnapshot);
    addAndMakeVisible(handPositionLabel);
    addAndMakeVisible(handPositionMarker);
    handPositionMarker.setVisible(false);
    lastSnapshot.setVisible(false);
    handPositionLabel.setJustificationType(juce::Justification::centredLeft);
    handPositionLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    handPositionLabel.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.75f));
    handPositionLabel.setAlwaysOnTop(true);
    handPositionLabel.setText("Right hand: not detected", juce::dontSendNotification);

    snapshotButton.setEnabled(false);
    snapshotButton.onClick = [this]
    { takeSnapshot(); };
    cameraSelector.onChange = [this]
    { cameraChanged(); };

    updateCameraList();
    cameraSelector.setSelectedId(1);

    const auto cameras = juce::CameraDevice::getAvailableDevices();
    if (cameras.size() > 0)
    {
        cameraSelector.setSelectedId(2);
        statusText = "Opening camera...";
        openCamera();
    }
    else
    {
        statusText = "No camera available";
    }
}

CameraView::~CameraView()
{
    stopTimer();
    if (cameraDevice != nullptr)
        cameraDevice->removeListener(this);
    cameraPreview.reset();
    cameraDevice.reset();
}

void CameraView::setHandPositionCallback(std::function<void(float, float, bool, bool)> callback)
{
    handPositionCallback = std::move(callback);
}

void CameraView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(SynthCamTheme::backgroundColour);

    auto previewArea = getLocalBounds().withTrimmedTop(32);
    graphics.setColour(juce::Colours::black);
    graphics.fillRect(previewArea);

    graphics.setColour(juce::Colours::white);
    graphics.setFont(juce::FontOptions(16.0f));
    if (cameraPreview == nullptr)
        graphics.drawFittedText(statusText, previewArea.reduced(12),
                                juce::Justification::centred, 2);
}

void CameraView::resized()
{
    auto controls = getLocalBounds().removeFromTop(28);
    cameraSelector.setBounds(controls.removeFromLeft(180));
    controls.removeFromLeft(8);
    snapshotButton.setBounds(controls.removeFromLeft(120));

    auto previewArea = getLocalBounds().reduced(1);
    previewArea.removeFromTop(31);
    auto cameraArea = previewArea.toFloat();
    if (cameraArea.getAspectRatio() > cameraAspectRatio)
        cameraArea.setWidth(cameraArea.getHeight() * cameraAspectRatio);
    else
        cameraArea.setHeight(cameraArea.getWidth() / cameraAspectRatio);
    cameraArea.setCentre(previewArea.getCentre().toFloat());
    const auto cameraBounds = cameraArea.toNearestInt();

    if (cameraPreview != nullptr)
    {
        cameraPreview->setBounds(previewArea);
        cameraPreview->setTransform(juce::AffineTransform::scale(
            -1.0f, 1.0f,
            cameraPreview->getWidth() * 0.5f,
            cameraPreview->getHeight() * 0.5f));
    }

    constexpr auto markerSize = 16;
    const auto markerX = cameraBounds.getX() + juce::roundToInt(handPosition.x * cameraBounds.getWidth()) - markerSize / 2;
    const auto markerY = cameraBounds.getY() + juce::roundToInt(handPosition.y * cameraBounds.getHeight()) - markerSize / 2;
    handPositionMarker.setBounds(markerX, markerY, markerSize, markerSize);
    handPositionMarker.toFront(false);

    lastSnapshot.setBounds(previewArea);
    lastSnapshot.setTransform(juce::AffineTransform::scale(
        -1.0f, 1.0f,
        lastSnapshot.getWidth() * 0.5f,
        lastSnapshot.getHeight() * 0.5f));
    handPositionLabel.setBounds(previewArea.removeFromTop(28).reduced(4, 2));
}

void CameraView::updateCameraList()
{
    cameraSelector.clear();
    cameraSelector.addItem("No camera", 1);
    cameraSelector.addSeparator();

    const auto cameras = juce::CameraDevice::getAvailableDevices();
    for (int i = 0; i < cameras.size(); ++i)
        cameraSelector.addItem(cameras[i], i + 2);
}

void CameraView::cameraChanged()
{
    stopTimer();
    if (cameraDevice != nullptr)
        cameraDevice->removeListener(this);
    cameraModel.stopHandTracker();
    cameraPreview.reset();
    cameraDevice.reset();
    lastSnapshot.setVisible(false);
    snapshotButton.setEnabled(false);
    handPositionLabel.setText("Right hand: not detected", juce::dontSendNotification);
    handPositionMarker.setVisible(false);
    statusText = "No camera selected";

    if (cameraSelector.getSelectedId() > 1)
        openCamera();

    resized();
    repaint();
}

void CameraView::openCamera()
{
    const auto selectedIndex = cameraSelector.getSelectedId() - 2;
    if (selectedIndex < 0)
        return;

#if JUCE_ANDROID || JUCE_IOS
    juce::Component::SafePointer<CameraView> safeThis(this);
    juce::CameraDevice::openDeviceAsync(selectedIndex,
                                        [safeThis](juce::CameraDevice *device,
                                                   const juce::String &error)
                                        {
                                            if (safeThis != nullptr)
                                                safeThis->cameraDeviceOpenResult(device, error);
                                        });
#else
    cameraDeviceOpenResult(juce::CameraDevice::openDevice(selectedIndex), {});
#endif
}

void CameraView::cameraDeviceOpenResult(juce::CameraDevice *device,
                                        const juce::String &error)
{
    cameraDevice.reset(device);

    if (cameraDevice == nullptr)
    {
        statusText = error.isNotEmpty() ? "Camera open failed: " + error
                                        : "Camera open failed";
        repaint();
        return;
    }

    cameraPreview.reset(cameraDevice->createViewerComponent());
    addAndMakeVisible(*cameraPreview);
    cameraDevice->addListener(this);
    handPositionLabel.toFront(true);
    snapshotButton.setEnabled(true);
    if (!cameraModel.startHandTracker())
    {
        handPositionLabel.setText("Right hand: tracker unavailable", juce::dontSendNotification);
        return;
    }

    startTimer(100);
    statusText.clear();
    resized();
    repaint();
}

void CameraView::takeSnapshot()
{
    if (cameraDevice == nullptr)
        return;

    juce::Component::SafePointer<CameraView> safeThis(this);
    cameraDevice->takeStillPicture(
        [safeThis](const juce::Image &image)
        {
            if (safeThis != nullptr)
                safeThis->snapshotReceived(image);
        });
}

void CameraView::snapshotReceived(const juce::Image &image)
{
    if (image.isValid())
    {
        lastSnapshot.setImage(image);
        lastSnapshot.setVisible(true);
        lastSnapshot.toFront(false);
        handPositionLabel.toFront(false);
    }
}

void CameraView::timerCallback()
{
    updateHandPosition();
}

void CameraView::imageReceived(const juce::Image &image)
{
    cameraModel.submitFrame(image);
}

void CameraView::updateHandPosition()
{
    const auto state = cameraModel.pollHandState();
    using Status = CameraModel::HandState::Status;

    if (state.status == Status::unavailable)
    {
        handPositionLabel.setText(state.diagnostic.isNotEmpty()
                                      ? state.diagnostic
                                      : "Right hand: tracker unavailable",
                                  juce::dontSendNotification);
        handPositionMarker.setVisible(false);
        return;
    }

    if (state.status == Status::stopped)
    {
        handPositionLabel.setText("Right hand: tracker stopped", juce::dontSendNotification);
        handPositionMarker.setVisible(false);
        return;
    }

    if (state.status == Status::waiting)
    {
        handPositionLabel.setText("Right hand: waiting (callbacks " + juce::String(state.receivedFrames)
                                      + ", writes " + juce::String(state.writtenFrames)
                                      + ", failures " + juce::String(state.failedFrames) + ")",
                                  juce::dontSendNotification);
        handPositionMarker.setVisible(false);
        return;
    }

    if (state.status == Status::notDetected)
    {
        handPositionLabel.setText("Right hand: not detected"
                                      + (state.diagnostic.isNotEmpty() ? " (" + state.diagnostic + ")" : ""),
                                  juce::dontSendNotification);
        handPositionMarker.setVisible(false);
        if (handPositionCallback != nullptr)
            handPositionCallback(handPosition.x, handPosition.y, false, false);
        return;
    }

    handPosition = state.position;
    handPositionLabel.setText("Right hand: X " + juce::String(handPosition.x, 2)
                                  + "  Y " + juce::String(handPosition.y, 2),
                              juce::dontSendNotification);
    handPositionMarker.setVisible(true);
    resized();
    if (handPositionCallback != nullptr)
        handPositionCallback(handPosition.x, handPosition.y, state.isClosed, true);
}
