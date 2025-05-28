#!/usr/bin/env python3
# compare_by_pwm.py  –  match (H-PWM , V-PWM) and compare distances

import csv, serial, re
from playsound import playsound

# ====== USER CONFIG =======================================================
BASELINE_CSV = "/Users/harshj/scans/scan_20250528_044127.csv"      # your CSV file (h_pwm,v_pwm,dist_cm,…)
PORT         = "/dev/cu.usbmodem1303"    # or "COM7"
BAUD         = 115_200
# =========================================================================

# load CSV into a dict keyed by (h_pwm, v_pwm) → dist_cm
baseline = {}
with open(BASELINE_CSV, newline="") as f:
    for r in csv.DictReader(f):
        h = int(r["h_pwm"])
        v = int(r["v_pwm"])
        baseline[(h, v)] = float(r["dist_cm"])

print(f"Loaded {len(baseline)} baseline entries\n"
      f"Listening on {PORT} … Ctrl-C to stop\n")

# regex to pull V, H, D from each serial line
rx = re.compile(r"V:(\d+)\s+H:(\d+)\s+D:\s*(\d+)cm")

try:
    with serial.Serial(PORT, BAUD, timeout=1) as ser:
        for raw in ser:
            m = rx.search(raw.decode("utf-8", "ignore"))
            if not m:
                continue

            v_pwm  = int(m.group(1))          # first number after V:
            h_pwm  = int(m.group(2))          # number after H:
            live_d = float(m.group(3))        # distance

            base_d = baseline.get((h_pwm, v_pwm))
            if base_d is None:
                # (H,V) combo not in CSV – ignore or print a note
                # print(f"(H={h_pwm}, V={v_pwm}) not found in baseline")
                continue

            diff = live_d - base_d
            print(f"H={h_pwm:4d}  V={v_pwm:4d}  "
                  f"baseline={base_d:6.1f} cm  live={live_d:6.1f} cm  "
                  f"Δ={diff:+6.1f} cm")
            if(diff >5):
                print("DETECTED")
                playsound('sound.mp3')

except KeyboardInterrupt:
    print("\nStopped.")
except serial.SerialException as e:
    print(f"Serial error: {e}")
