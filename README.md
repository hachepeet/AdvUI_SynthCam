# AdvUI_SynthCam

## Build VST3

### Windows

```powershell
cmake -S . -B build-export -DSYNTHCAM_EXPORT_ROOT=C:/SynthCam_VST
cmake --build build-export --config Release
```

The built VST3 plugins are copied to `C:\SynthCam_VST\VST3`, and
`hand_tracker.py` is copied to `C:\SynthCam_VST\hand_tracker.py`.

### macOS

The following steps build and install the VST3 files into
`~/SynthCam_VST/VST3`.

1. Install Xcode Command Line Tools, CMake, and Python 3 if they are not
   already installed.
2. Open Terminal and go to the project directory:

```shell
cd /path/to/AdvUI_SynthCam
```

Replace `/path/to/AdvUI_SynthCam` with the actual location of the cloned
project.

3. Install the tracker dependencies. A virtual environment is not required:

```shell
python3 -m pip install mediapipe opencv-python
```

4. Configure and build the Release plugins:

```shell
cmake -S . -B build-export -DSYNTHCAM_EXPORT_ROOT="$HOME/SynthCam_VST"
cmake --build build-export --config Release
```

After the build, these files should exist:

```text
~/SynthCam_VST/VST3/SynthCam - SynthGenerator.vst3
~/SynthCam_VST/VST3/SynthCam - FXCam.vst3
~/SynthCam_VST/hand_tracker.py
```

5. Copy the `.vst3` folders from `~/SynthCam_VST/VST3` to the host's VST3
   plugin folder, or add `~/SynthCam_VST/VST3` as a custom VST3 scan folder in
   the host. Rescan plugins in the host, then load either SynthGenerator or
   FXCam.
6. Allow camera access when macOS asks. The hand tracker starts when the
   plugin opens its camera view.

If the plugin reports a missing Python module, install the packages with the
same interpreter used by the plugin:

```shell
SYNTHCAM_PYTHON="$(command -v python3)"
"$SYNTHCAM_PYTHON" -m pip install mediapipe opencv-python
```

For a virtual environment, replace the path with its Python executable and
start the audio host from the same Terminal session so it receives
`SYNTHCAM_PYTHON`.
