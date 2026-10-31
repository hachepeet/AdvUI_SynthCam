# AdvUI_SynthCam

## Build VST3

Look for them in `AdvUI_SynthCam\build`

## Install Mediapipe and OpenCV
Install the worker dependencies in the Python environment that is on PATH:
```powershell
python -m pip install "mediapipe==0.10.14" opencv-python
```


## TODO
- To discuss: CameraView is doing all the processing, but there is no model like the rest of the interface. May need some refactoring. 