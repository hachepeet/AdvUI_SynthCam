"""
Read camera frames written by JUCE and report the right-hand position.

References:
MediaPipe Hands official documentation
https://chuoling.github.io/mediapipe/solutions/hands.html

MediaPipe official Python source
https://github.com/google-ai-edge/mediapipe/blob/master/mediapipe/python/solutions/hands.py

MediaPipe hand landmarker documentation
https://developers.google.com/edge/mediapipe/solutions/vision/hand_landmarker/python

OpenCV color conversion documentation
https://docs.opencv.org/4.x/d8/d01/group__imgproc__color__conversions.html

Temporary camera screenshots are saved in order to track the right hand.
"""

from pathlib import Path
import math
import sys
import time


def write_status(path: Path, message: str) -> None:
    temporary = path.with_suffix(".tmp")
    temporary.write_text(message, encoding="utf-8")
    temporary.replace(path)

def is_hand_closed(hand_landmarks) -> bool:
    landmarks = hand_landmarks.landmark
    wrist = landmarks[0]
    closed_fingers = 0
    for tip_index, pip_index in ((8, 6), (12, 10), (16, 14), (20, 18)):
        tip = landmarks[tip_index]
        pip = landmarks[pip_index]
        tip_distance = math.dist((tip.x, tip.y), (wrist.x, wrist.y))
        pip_distance = math.dist((pip.x, pip.y), (wrist.x, wrist.y))
        if tip_distance < pip_distance:
            closed_fingers += 1
    return closed_fingers >= 3


def main() -> int:
    if len(sys.argv) != 2:
        return 2

    # Debugging and diagnostics were written with the help of GitHub Copilot
    directory = Path(sys.argv[1])
    directory.mkdir(parents=True, exist_ok=True)
    status_path = directory / "status.txt"
    diagnostics_path = directory / "diagnostics.txt"

    try:
        import cv2
        import mediapipe as mp
        hands = mp.solutions.hands.Hands(
            static_image_mode=False,
            max_num_hands=2,
            model_complexity=0,
            min_detection_confidence=0.6,
            min_tracking_confidence=0.6,
        )
    except Exception as error:
        write_status(status_path, f"Tracker unavailable: {error}")
        return 1

    write_status(status_path, "ready")
    last_frame_time = 0.0
    processed_frames = 0

    try:
        while True:
            frame_paths = list(directory.glob("frame_*.jpg"))
            if not frame_paths:
                time.sleep(0.01)
                continue

            try:
                frame_path = max(frame_paths, key=lambda path: path.stat().st_mtime_ns)
                frame_time = frame_path.stat().st_mtime_ns
                if frame_time <= last_frame_time:
                    time.sleep(0.01)
                    continue

                frame = cv2.imread(str(frame_path))
                frame_path.unlink(missing_ok=True)
                last_frame_time = frame_time
            except OSError:
                time.sleep(0.01)
                continue

            if frame is None:
                continue

            processed_frames += 1
            mirrored_frame = cv2.flip(frame, 1)
            results = hands.process(cv2.cvtColor(mirrored_frame, cv2.COLOR_BGR2RGB))
            right_hand = None
            right_hand_landmarks = None
            labels = []
            if results.multi_hand_landmarks and results.multi_handedness:
                for landmarks, handedness in zip(
                    results.multi_hand_landmarks, results.multi_handedness
                ):
                    label = handedness.classification[0].label
                    labels.append(label)
                    if label == "Right":
                        right_hand_landmarks = landmarks
                        right_hand = landmarks.landmark[9]
                        break

            detected_hands = len(results.multi_hand_landmarks or [])
            diagnostics_path.write_text(
                f"frames={processed_frames}, hands={detected_hands}, "
                f"labels={','.join(labels) or 'none'}",
                encoding="ascii",
            )
            result = "none"
            if right_hand is not None:
                x = 1.0 - right_hand.x
                result = f"{x:.4f},{right_hand.y:.4f},{int(is_hand_closed(right_hand_landmarks))}"

            result_path = directory / f"right_hand_{processed_frames:08d}.txt"
            result_path.write_text(result, encoding="ascii")
    except Exception as error:
        write_status(status_path, f"Tracker error: {error}")
        return 1
    except KeyboardInterrupt:
        return 0
    finally:
        hands.close()


if __name__ == "__main__":
    raise SystemExit(main())
