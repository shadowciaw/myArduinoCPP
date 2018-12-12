#

import serial
import time

port = '/dev/ttyACM0'


def main():
    arduino = serial.Serial(port, 9600)
    # arduino port

    while True:
        # prints 3 bytes it reads
        datax = int.from_bytes((arduino.read()), byteorder='big') - 48
        datay = int.from_bytes((arduino.read()), byteorder='big') - 48

        if (datax != 0 && datay != 0):
            print("not 0")
            print(datax, datay)


if __name__ == "__main__":
    main()
