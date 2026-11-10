This project was created for the Advanced Programming of Interactive Systems
course. It explores interactive audio and visual processing through SynthCam,
combining camera-based interaction with synthesizer and effects plugins.

# AdvUI_SynthCam

## Build plugins

The project builds VST3 and Standalone plugins on Windows, macOS, and Linux.
macOS also builds an AU plugin. The default export directory is
`<build-directory>/SynthCam_VST`; it can be overridden with
`-DSYNTHCAM_EXPORT_ROOT=<directory>`.

### Windows

```powershell
cmake -S . -B build-export -DSYNTHCAM_EXPORT_ROOT=C:/SynthCam_VST
cmake --build build-export --config Release
```

With the command above, the built VST3 plugins are copied to
`C:\SynthCam_VST\VST3`, and `hand_tracker.py` is copied to
`C:\SynthCam_VST\hand_tracker.py`. Without an explicit export path, they are
placed under `build-export/SynthCam_VST`.

### macOS

The following steps build and install the VST3 files into
`~/SynthCam_VST/VST3`.

1. Install Xcode Command Line Tools, CMake, and Python 3 if they are not
   already installed. Make sure to use Python 3.12 as newer versions of Python 
   can be incompatible with the necessary version of MediaPipe. On macOs with Homebrew:

``` shell
brew install python@3.12
```

2. Open Terminal and go to the project directory:

```shell
cd /path/to/AdvUI_SynthCam
```

Replace `/path/to/AdvUI_SynthCam` with the actual location of the cloned
project.

3. Create a virtual environment and install tracker dependencies. Make sure to install
   version 0.10.21 of MediaPipe as newer versions do not support the solutions module
   used for hand tracking. 

``` shell
/opt/homebrew/bin/python3.12 -m venv ~/SynthCam_Venv
~/SynthCam_Venv/bin/python -m pip install --upgrade pip 
~/SynthCam_Venv/bin/python -m pip install mediapipe==0.10.21 opencv-python
```

Note: the homebrew path above is for Apple Silicon Macs, for Intel Macs homebrew is
typically installed at /usr/local or /usr/local/bin/brew instead of opt/homebrew.

4. Configure the Python executable and build the Release plugins:

```shell
launchctl setenv SYNTHCAM_PYTHON "$HOME/SynthCam_Venv/bin/python"
cmake -S . -B build-export -DSYNTHCAM_EXPORT_ROOT="$HOME/SynthCam_VST"
cmake --build build-export --config Release
```

Note: Setting SYNTHCAM_PYTHON to the virtual environment's Python executable should
make it so that the host can be opened from the general applications library instead of
having to be opened from the same terminal session. However the host can also be 
opened from the same terminal session as an added precaution. 

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

If the plugin reports a missing Python module, make sure that you have installed 
the packages with the same interpreter used by the plugin:

```shell
SYNTHCAM_PYTHON="$(command -v python3)"
"$SYNTHCAM_PYTHON" -m pip install mediapipe opencv-python
```

### Linux (has not been tested)

Install CMake, a C++ compiler, Python 3, and the development packages required
by JUCE and the camera backend. Then install the tracker dependencies and
build:

```shell
python3 -m pip install mediapipe opencv-python
cmake -S . -B build-export -DSYNTHCAM_EXPORT_ROOT="$HOME/SynthCam_VST"
cmake --build build-export --config Release
```

Copy the VST3 bundles from `$HOME/SynthCam_VST/VST3` to the host's VST3 scan
folder, or configure that directory as a custom scan folder. Set
`SYNTHCAM_PYTHON` to the Python executable containing the tracker dependencies
when it is not available as `python3` on `PATH`.
