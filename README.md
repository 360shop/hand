# Hand Gesture Counter for Raspberry Pi (C Version)

This project uses OpenCV in C to detect and count hand gestures (5-10 fingers) using either a Raspberry Pi Camera V2 or USB webcam on a Raspberry Pi 3B.

## Features

- Real-time hand gesture detection and finger counting
- Support for both Raspberry Pi Camera V2 and USB webcam
- Visual display of hand contours and convexity defects
- On-screen finger count display
- Command-line interface for camera selection

## Requirements

- Raspberry Pi 3B
- Raspberry Pi Camera V2 or USB webcam
- OpenCV development libraries
- GCC compiler
- Make build system

## Installation

1. Install required system packages:
```bash
sudo apt-get update
sudo apt-get install -y build-essential
sudo apt-get install -y libopencv-dev
```

2. Clone this repository:
```bash
git clone <repository-url>
cd hand-gesture-counter
```

3. Build the program:
```bash
make
```

## Usage

1. To use with USB webcam:
```bash
./hand_gesture
```

2. To use with Raspberry Pi Camera V2:
```bash
./hand_gesture --picam
```

3. Controls:
   - Press 'q' to quit the program

## How it Works

The program uses computer vision techniques to detect hands and count fingers:

1. Image Processing:
   - Converts frame to grayscale
   - Applies Gaussian blur for noise reduction
   - Creates binary threshold image

2. Hand Detection:
   - Finds contours in the binary image
   - Identifies the largest contour within valid size range as the hand

3. Finger Counting:
   - Calculates convex hull of the hand contour
   - Finds convexity defects
   - Counts significant defects to determine number of fingers
   - Displays count when 5-10 fingers are detected

## Project Structure

- `hand_gesture.c`: Main C source code containing the hand detection logic
- `Makefile`: Build configuration for compiling the program
- `README.md`: Project documentation

## Troubleshooting

1. If compilation fails:
   - Ensure OpenCV development packages are installed
   - Check if pkg-config is properly set up
   - Verify all required headers are available

2. If camera doesn't initialize:
   - Check camera connections
   - Ensure proper permissions are set
   - Try running with sudo if permission errors occur

3. If hand detection is not accurate:
   - Adjust lighting conditions
   - Keep hands within camera frame
   - Modify threshold values in the code if needed:
     - MIN_AREA and MAX_AREA for hand size detection
     - DEFECTS_THRESHOLD for finger detection sensitivity

## Performance Notes

The C implementation offers better performance compared to Python due to:
- Direct hardware access
- No interpreter overhead
- Efficient memory management
- Lower-level OpenCV operations

This makes it particularly suitable for the Raspberry Pi 3B's limited resources.

## License

This project is open source and available under the MIT License.