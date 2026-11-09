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
python3 -m pip install mediapipe opencv-python
```

The plugin starts the first available macOS interpreter from Homebrew
(`/opt/homebrew/bin/python3` or `/usr/local/bin/python3`), the standard
Python framework, and then `python3`. Install the packages with the same
interpreter that will be used by the plugin. If Python is installed elsewhere,
set `SYNTHCAM_PYTHON` to its full executable path before starting the host:

```shell
export SYNTHCAM_PYTHON="$HOME/.venv/bin/python"
"$SYNTHCAM_PYTHON" -m pip install mediapipe opencv-python
```
