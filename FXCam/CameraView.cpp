#include "CameraView.h"

#include "Theme.h"

// Inspired by https://github.com/juce-framework/JUCE/blob/29396c22c93392d6738e021b83196283d6e4d850/examples/GUI/CameraDemo.h

CameraView::CameraView()
{
    addAndMakeVisible(cameraSelector);
    addAndMakeVisible(snapshotButton);
    addAndMakeVisible(lastSnapshot);
    lastSnapshot.setVisible(false);

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
    cameraPreview.reset();
    cameraDevice.reset();
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
    if (cameraPreview != nullptr)
        cameraPreview->setBounds(previewArea);

    lastSnapshot.setBounds(previewArea);
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
    cameraPreview.reset();
    cameraDevice.reset();
    lastSnapshot.setVisible(false);
    snapshotButton.setEnabled(false);
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
    snapshotButton.setEnabled(true);
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
                safeThis->imageReceived(image);
        });
}

void CameraView::imageReceived(const juce::Image &image)
{
    if (image.isValid())
    {
        lastSnapshot.setImage(image);
        lastSnapshot.setVisible(true);
        lastSnapshot.toFront(false);
    }
}
