from time import sleep
import pygame

# Initialize pygame
pygame.init()
pygame.joystick.init()

# Detect available joysticks
if pygame.joystick.get_count() == 0:
    print("No joystick detected!")
    exit()

# Initialize the first joystick (steering wheel)
joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Connected to: {joystick.get_name()}")
print(f"Number of axes detected: {joystick.get_numaxes()}")

# Main loop to read joystick input
while True:
    pygame.event.pump()  # Process joystick events

    # Read steering rotation (Axis 0)
    steering_value = joystick.get_axis(0)

    # Read acceleration (Axis 5) and braking (Axis 4)
    accel_value = joystick.get_axis(5)
    brake_value = joystick.get_axis(4)

    # Convert Pedals from [-1 to 1] to [0 to 1]
    accel_value = (accel_value + 1) / 2
    brake_value = (brake_value + 1) / 2

    # Get the average pedal value which will be transmitted later to the ESP32 of the fixed station
    # pedal_value will be in range [-1 to 1]
    pedal_value = accel_value - brake_value

    print(f"Steering: {steering_value:.2f} | Accel: {accel_value:.2f} | Brake: {brake_value:.2f} | Pedal: {pedal_value:.2}")
    sleep(0.1)
