# Hand Gesture Recognition and Finger Counter

This project uses OpenCV and MediaPipe to detect hand gestures and count fingers in real-time using a camera feed. Optimized for Raspberry Pi 3B.

## Features
- Real-time dual hand landmark detection
- Finger counting for both hands (0-10 fingers total)
- Individual finger count for each hand
- Total finger count display
- FPS display
- Mirror display for intuitive interaction

## Requirements
- Raspberry Pi 3B
- Python 3.7+
- USB webcam or Raspberry Pi Camera Module

## Installation

1. Clone this repository:
```bash
git clone [your-repo-url]
cd [repo-directory]
```

2. Install system dependencies:
```bash
sudo apt-get update
sudo apt-get install -y python3-pip python3-opencv
```

3. Install Python dependencies:
```bash
pip3 install -r requirements.txt
```

## Usage

1. Connect your camera to the Raspberry Pi

2. Run the script:
```bash
python3 hand_gesture.py
```

3. Controls:
- Press 'q' to quit the application
- Hold one or both hands in front of the camera to detect gestures
- The number of detected fingers for each hand will be displayed on screen
- Total finger count (sum of both hands) will be shown

## Notes
- The application is optimized for Raspberry Pi 3B with reduced resolution (640x480) for better performance
- For best results, ensure good lighting conditions
- Keep your hands within the camera frame and at a reasonable distance
- The system can detect and count fingers on both hands simultaneously

## Troubleshooting
- If the camera feed doesn't open, check if your camera is properly connected
- If the performance is slow, try reducing the frame resolution in the code
- Make sure all dependencies are properly installed
- If hand detection is unstable, ensure adequate lighting and contrast with the background