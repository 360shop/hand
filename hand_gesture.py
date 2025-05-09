import cv2
import mediapipe as mp
import time

class HandGestureCounter:
    def __init__(self):
        # Initialize MediaPipe Hands
        self.mp_hands = mp.solutions.hands
        self.hands = self.mp_hands.Hands(
            static_image_mode=False,
            max_num_hands=2,  # Changed to detect two hands
            min_detection_confidence=0.5,
            min_tracking_confidence=0.5
        )
        self.mp_draw = mp.solutions.drawing_utils
        
        # Initialize camera
        self.cap = cv2.VideoCapture(0)
        # Set lower resolution for better performance on Pi 3B
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        
    def count_fingers(self, hand_landmarks):
        finger_tips = [8, 12, 16, 20]  # Index, Middle, Ring, Pinky tips
        thumb_tip = 4
        fingers = []
        
        if hand_landmarks:
            # Thumb
            if hand_landmarks.landmark[thumb_tip].x < hand_landmarks.landmark[thumb_tip - 1].x:
                fingers.append(1)
            else:
                fingers.append(0)
            
            # Other fingers
            for tip in finger_tips:
                if hand_landmarks.landmark[tip].y < hand_landmarks.landmark[tip - 2].y:
                    fingers.append(1)
                else:
                    fingers.append(0)
                    
        return sum(fingers)
    
    def process_frame(self, frame):
        # Convert BGR to RGB
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        
        # Process the frame and detect hands
        results = self.hands.process(rgb_frame)
        
        total_fingers = 0
        
        # Draw hand landmarks and count fingers
        if results.multi_hand_landmarks:
            for idx, hand_landmarks in enumerate(results.multi_hand_landmarks):
                # Draw landmarks
                self.mp_draw.draw_landmarks(
                    frame,
                    hand_landmarks,
                    self.mp_hands.HAND_CONNECTIONS
                )
                
                # Count fingers for this hand
                hand_fingers = self.count_fingers(hand_landmarks)
                total_fingers += hand_fingers
                
                # Display fingers for each hand
                hand_text = f'Hand {idx + 1}: {hand_fingers}'
                cv2.putText(
                    frame,
                    hand_text,
                    (10, 70 + (idx * 40)),  # Offset text for each hand
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1,
                    (255, 0, 0),
                    2
                )
        
        # Display total finger count
        cv2.putText(
            frame,
            f'Total Fingers: {total_fingers}',
            (10, 150),
            cv2.FONT_HERSHEY_SIMPLEX,
            1,
            (0, 255, 0),
            2
        )
        
        return frame
    
    def run(self):
        prev_time = 0
        
        while True:
            # Read frame
            success, frame = self.cap.read()
            if not success:
                print("Failed to capture frame")
                break
                
            # Mirror the frame horizontally
            frame = cv2.flip(frame, 1)
            
            # Process the frame
            frame = self.process_frame(frame)
            
            # Calculate and display FPS
            current_time = time.time()
            fps = 1 / (current_time - prev_time)
            prev_time = current_time
            
            cv2.putText(
                frame,
                f'FPS: {int(fps)}',
                (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                (255, 0, 0),
                2
            )
            
            # Display the frame
            cv2.imshow('Hand Gesture Counter', frame)
            
            # Break loop on 'q' press
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        
        # Clean up
        self.cap.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    try:
        gesture_counter = HandGestureCounter()
        gesture_counter.run()
    except Exception as e:
        print(f"Error: {e}")