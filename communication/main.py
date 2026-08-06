import serial
import re
import subprocess
import webbrowser

PORT = "/dev/cu.usbserial-1130"  ## this is for mac
BAUD_RATE = 115200

SER = serial.Serial(PORT, BAUD_RATE, timeout=1)

pattern = re.compile(r"(\d)\s+(\d)\s*$")

def handle_key(row, col):
    if row == 0 and col == 0:
        subprocess.Popen(["open", "-a", "Visual Studio Code"])
    elif row == 0 and col == 1:
        webbrowser.open("https://www.youtube.com/watch?v=bpD-JVy2zV4&t=99s")
    elif row == 1 and col == 0:
        subprocess.Popen(["open", "-a", "Music"])
    elif row == 1 and col == 1:
        subprocess.Popen(["open", "-a", "iTerm"])
    else:
        print(f"No action mapped for {row} {col}")

def main():
    while True:
        line = SER.readline().decode('utf-8', errors='ignore').strip()
        if line:
            print(line)  # keep raw print for debugging
            match = pattern.search(line)
            if match:
                row = int(match.group(1))
                col = int(match.group(2))
                handle_key(row, col)

main()