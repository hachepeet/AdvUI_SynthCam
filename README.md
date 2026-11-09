# AdvUI_SynthCam

## Build VST3

Choose the export directory when configuring CMake. For example, on Windows:

```powershell
cmake -S . -B build-export -DSYNTHCAM_EXPORT_ROOT=C:/SynthCam_VST
cmake --build build-export --config Release
```

On Windows, the built VST3 plugins are copied to `C:\SynthCam_VST\VST3`, and
`hand_tracker.py` is copied to `C:\SynthCam_VST\hand_tracker.py`.

On macOS, choose a macOS path instead, for example:

```shell
cmake -S . -B build-export -DSYNTHCAM_EXPORT_ROOT="$HOME/SynthCam_VST"
cmake --build build-export --config Release
```

## Install Mediapipe and OpenCV
On Windows:
```powershell
python -m pip install "mediapipe==0.10.14" opencv-python
```
On macOS:
```shell
python3 -m pip install "mediapipe==0.10.14" opencv-python
```