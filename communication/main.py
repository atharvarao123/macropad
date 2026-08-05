import serial


PORT = "/dev/cu.usbserial-1130" ##this is for mac
BAUD_RATE = 115200

SER = serial.Serial(PORT,BAUD_RATE,timeout=1)


def main():
    while True:
        line = SER.readline().decode('utf-8',errors = 'ignore').strip()
         ##ignoring blank line can be blank
        if(line):
            print(line)


main()