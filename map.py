import csv
import serial
import re
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def pwm_to_angle(pwm, min_pwm, max_pwm, max_angle):
    return (pwm - min_pwm) * max_angle / (max_pwm - min_pwm)

def read_scan(port, baud=115200, csv_file="scan_data.csv"):
    ser = serial.Serial(port, baud, timeout=1)
    pattern = r"V:(\d+)\s+H:(\d+)\s+D:\s*(\d+)cm"

    # Prepare CSV file
    with open(csv_file, mode='w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['h_pwm', 'v_pwm', 'distance_cm', 'h_angle_deg', 'v_angle_deg', 'x_cm', 'y_cm', 'z_cm'])

        data = []
        while True:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if not line:
                continue
            print(line)
            if line.lower().startswith('complete'):
                break

            match = re.match(pattern, line)
            if match:
                v_pwm = int(match.group(1))
                h_pwm = int(match.group(2))
                dist = int(match.group(3))
                # calibration constants
                H_MIN, H_MAX, H_ANGLE = 1200, 1800, 170
                V_MIN, V_MAX, V_ANGLE = 1200, 1900, 90

                h_deg = pwm_to_angle(h_pwm, H_MIN, H_MAX, H_ANGLE)
                v_deg = pwm_to_angle(v_pwm, V_MIN, V_MAX, V_ANGLE)
                h_rad = math.radians(h_deg)
                v_rad = math.radians(v_deg)

                # spherical to Cartesian
                r = dist * math.cos(v_rad)
                x = r * math.cos(h_rad)
                y = r * math.sin(h_rad)
                z = dist * math.sin(v_rad)

                # write row to CSV
                writer.writerow([h_pwm, v_pwm, dist, h_deg, v_deg, x, y, z])
                data.append((x, y, z))

    ser.close()
    return data

def plot_3d(data):
    xs, ys, zs = zip(*data)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(xs, ys, zs)
    ax.set_xlabel('X (cm)')
    ax.set_ylabel('Y (cm)')
    ax.set_zlabel('Z (cm)')
    plt.show()

def main():
    port = '/dev/cu.usbmodem1303'  # change to your serial port
    scan_data = read_scan(port)
    plot_3d(scan_data)

if __name__ == '__main__':
    main()
