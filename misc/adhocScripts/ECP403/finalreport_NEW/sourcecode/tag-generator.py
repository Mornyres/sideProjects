import string
import random
import sys
from datetime import datetime, date, time
from time import sleep

import serial

from collections import Counter
# TODO: more error handling
# TODO: allow for specific environment & tag layout

# only uppercase hex
def tags_generator(numTags):
	# Generate some number of dummy tags
	tagIndex = []
	tagID = []
	for i in range(numTags):
		tagIndex.append(i)
		tagID.append(id_generator(24))
	return tagID
	

def id_generator(size=24, chars=(string.hexdigits.replace("abcdef",""))):
    # return random hex string 24 chars long, replacing lowercase with null
    return ''.join(random.choice(chars) for _ in range(size))

directory = "logs/transmit/"

timestamp = datetime.now()
timestamp = str(timestamp)

# formatting to make it compatible with windows style file names
timestamp = timestamp.replace(" ", "_")
timestamp = timestamp.replace(".","_")
timestamp = timestamp.replace(":","_")

transmit_log=open(directory+timestamp+".dat","w+")

# following
# https://pimylifeup.com/raspberry-pi-serial/
transmit_port = serial.Serial(
        #port="/dev/ttyAMA0",   # raspi <= 2
        port="/dev/ttyS0", # raspi >= 3
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
        )

tag_pool = tags_generator(int(sys.argv[1]))

while(True):
    try:
        tagnum = random.randint(0,len(tag_pool)-1)

        output_data = tag_pool[tagnum]
        output_data = str(output_data)

        # random delay
        delay = random.randint(1,5)
        sleep(delay)
        
        # get timestamp at start of each iteration

        timestamp = datetime.now()
        timestamp = str(timestamp)

        transmit_port.write(output_data)

        sys.stdout.write("(")
        sys.stdout.write(timestamp)
        sys.stdout.write(")")

        sys.stdout.write(output_data)
        sys.stdout.write('\r\n')


        transmit_log.write("(")
        transmit_log.write(timestamp)
        transmit_log.write(")")

        transmit_log.write(output_data)
        transmit_log.write('\r\n')

    except KeyboardInterrupt:
        transmit_log.close()
        transmit_port.close()
        break

