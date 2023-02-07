import serial                                                             
import time        
import numpy as np

ArduinoUnoSerial = serial.Serial('COM4',9600)   
        
if not ArduinoUnoSerial.isOpen():
    ArduinoUnoSerial.open()                                         

id = ["11724223747"]
name = ["master", "slave"]

user = ""
while (ArduinoUnoSerial.isOpen()):         #Do this forever
    dist_r = ArduinoUnoSerial.readline().decode("utf-8").split()
    num = ""
    if 'dec:' in dist_r:
        for i in dist_r:
            if i.isdigit():
                num = num + i
        if num == id[0]:
            user = name[0]
            print(f"you are {name[0]}")
        else:
            user = name[1]
            print(f"you are {name[1]}")
        print(num)
    elif 'previously.' in dist_r:
        print(user)
