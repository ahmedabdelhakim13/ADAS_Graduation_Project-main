import cv2
import mediapipe as mp
import numpy as np
import serial
import time

# Initialize serial communication
try:
    ser = serial.Serial('COM5', 115200)  # Change 'COM3' to your ESP32 port
    time.sleep(2)  # Wait for serial to initialize
    print("Serial connection established.")
except:
    print("Failed to connect to ESP32 via serial.")
    ser = None

def send_alert():
    if ser and ser.is_open:
        ser.write(b'Drowsy')  # Send byte 'Drowsy'
        print("Sent 'Drowsy' to ESP32")

def eye_aspect_ratio(landmarks, left_eye_indices, right_eye_indices):
    def get_eye_ratio(eye_points):
        A = np.linalg.norm(np.array(eye_points[1]) - np.array(eye_points[5]))
        B = np.linalg.norm(np.array(eye_points[2]) - np.array(eye_points[4]))
        C = np.linalg.norm(np.array(eye_points[0]) - np.array(eye_points[3]))
        return (A + B) / (2.0 * C)
    
    left_eye = [landmarks[i] for i in left_eye_indices]
    right_eye = [landmarks[i] for i in right_eye_indices]
    
    left_ratio = get_eye_ratio(left_eye)
    right_ratio = get_eye_ratio(right_eye)
    
    return (left_ratio + right_ratio) / 2.0

mp_face_mesh = mp.solutions.face_mesh
mp_pose = mp.solutions.pose

face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.5, min_tracking_confidence=0.5)
pose = mp_pose.Pose()

cap = cv2.VideoCapture(0)
EYE_AR_THRESHOLD = 0.25
DROWSY_FRAMES = 30
frame_counter = 0

alert_sent = False  # To prevent repeated sends

eye_indices = {
    "left": [33, 160, 158, 133, 153, 144],
    "right": [362, 385, 387, 263, 373, 380]
}

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results_face = face_mesh.process(frame_rgb)
    results_pose = pose.process(frame_rgb)
    
    h, w, _ = frame.shape
    landmarks = []
    
    drowsy = False
    head_down = False

    if results_face.multi_face_landmarks:
        for face_landmarks in results_face.multi_face_landmarks:
            landmarks = [(int(pt.x * w), int(pt.y * h)) for pt in face_landmarks.landmark]
            ear = eye_aspect_ratio(landmarks, eye_indices['left'], eye_indices['right'])
            
            if ear < EYE_AR_THRESHOLD:
                frame_counter += 1
            else:
                frame_counter = 0
            
            if frame_counter >= DROWSY_FRAMES:
                drowsy = True

    if results_pose.pose_landmarks:
        nose = results_pose.pose_landmarks.landmark[mp_pose.PoseLandmark.NOSE]
        if nose.visibility > 0.5:
            nose_y = nose.y * h
            if nose_y > h * 0.7:
                head_down = True

    if drowsy or head_down:
        if not alert_sent:
            send_alert()
            alert_sent = True
        cv2.putText(frame, "Drowsy!" if drowsy else "Head Down!", 
                    (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    else:
        alert_sent = False  # Reset flag if no longer drowsy or head down

    cv2.imshow("Drowsiness Detection", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Cleanup
cap.release()
cv2.destroyAllWindows()
if ser and ser.is_open:
    ser.close()
    print("Serial connection closed.")