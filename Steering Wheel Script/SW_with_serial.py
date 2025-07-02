from time import sleep
import serial
import pygame

# User-defined Serial Port Input
SERIAL_PORT = input("Enter Serial Port (e.g., COM3 or /dev/ttyUSB0): ")
BAUD_RATE = 115200

# Initialize Serial Connection with Exception Handling
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
except serial.SerialException:
    print("Failed to connect to ESP32. Check your serial port.")
    exit()

# Initialize Pygame for Joystick Input
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No joystick detected!")
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Connected to: {joystick.get_name()}")
print(f"Number of axes detected: {joystick.get_numaxes()}")

try:
    while True:
        pygame.event.pump()

        # Read joystick axes
        steering_value = joystick.get_axis(0)
        accel_value = joystick.get_axis(5)
        brake_value = joystick.get_axis(4)

        # Convert pedals to [0,1] range
        accel_value = (accel_value + 1) / 2
        brake_value = (brake_value + 1) / 2

        # Calculate pedal value
        pedal_value = accel_value - brake_value

        # Format and send data
        data = f"{steering_value:.2f},{pedal_value:.2f}\n"
        ser.write(data.encode())

        print(f"Steering: {steering_value:.2f} | Pedal: {pedal_value:.2f}")
        # print(f"Sent Data: '{data.strip()}'")
        sleep(0.2)

except KeyboardInterrupt:
    print("\nExiting...")
    ser.close()
    pygame.quit()