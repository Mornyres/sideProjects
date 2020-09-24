import string
import sys
from datetime import datetime, date, time

import serial

directory = "logs/receive/"
        
# initial timestamp for filename
timestamp = datetime.now()
timestamp = str(timestamp)

# formatting to make it compatible with windows style file names
timestamp = timestamp.replace(" ", "_")
timestamp = timestamp.replace(".","_")
timestamp = timestamp.replace(":","_")

receive_log=open(directory+timestamp+".dat","w+")

# following
# https://pimylifeup.com/raspberry-pi-serial/
uart_port = serial.Serial(
        #port="/dev/ttyAMA0",   # raspi <= 2
        port="/dev/ttyS0", # raspi >= 3
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
        )


while(True):
    try:
        received_data=""
        timestamp = datetime.now()
        timestamp = str(timestamp)

        # wait for non-null input
        while not received_data:
            received_data = uart_port.readline()


        sys.stdout.write("(")
        sys.stdout.write(timestamp)
        sys.stdout.write(")")

        sys.stdout.write(received_data)
        sys.stdout.write('\r\n')


        receive_log.write("(")
        receive_log.write(timestamp)
        receive_log.write(")")

        receive_log.write(received_data)
        receive_log.write('\r\n')

    except KeyboardInterrupt:
        receive_log.close()
        uart_port.close()
        break




