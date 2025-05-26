import pygame
import serial
import time

ser = serial.Serial('COM10', 115200, timeout=0.1)
pygame.mixer.init()

#loading sounds 
C4 = pygame.mixer.Sound("C4.wav")
D4 = pygame.mixer.Sound("D4.wav")
E4 = pygame.mixer.Sound("E4.wav")
F4 = pygame.mixer.Sound("F4.wav")
G4 = pygame.mixer.Sound("G4.wav")

while True:

    if ser.in_waiting:

        key_press = ser.readline().decode('utf-8', errors='ignore').strip()

        print(key_press)

        if key_press == 'C':
            C4.play()
            print("test1!")

        elif key_press == 'D':
            D4.play()
            print("test2")

    
        elif key_press == 'E':
            E4.play()
            print("test3!")

        
        elif key_press == 'F':
            F4.play()
            print("test4!")


        elif key_press == 'G':
            G4.play()
            print("test1!")


        time.sleep(0.2)