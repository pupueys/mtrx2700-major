import serial
import time
from pynput import keyboard

# Set up serial connection
ser = serial.Serial('COM7', 115200)  # Change COM port as needed

# Track pressed keys
pressed_keys = set()
key_order = ['w', 'a', 's', 'd']
last_sent_index = 0

def on_press(key):
    try:
        if key.char in key_order:
            pressed_keys.add(key.char)
    except AttributeError:
        pass

def on_release(key):
    try:
        if key.char in key_order:
            pressed_keys.discard(key.char)
    except AttributeError:
        pass

# Start keyboard listener
listener = keyboard.Listener(on_press=on_press, on_release=on_release)
listener.start()

# Main loop
try:
    while True:
        keys_to_send = [k for k in key_order if k in pressed_keys]
        if keys_to_send:
            last_sent_index
            key = keys_to_send[last_sent_index % len(keys_to_send)]
            ser.write(key.encode())
            print(key.encode())
            last_sent_index += 1
        time.sleep(0.0005)  # Adjust for speed
except KeyboardInterrupt:
    listener.stop()
    ser.close()
