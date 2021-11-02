import time
import serial
import RPi.GPIO as GPIO


ser = serial.Serial(
    port='/dev/ttyS0',
    baudrate = 9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1            
 )

def read_msg():
    x=ser.readline().strip()
    print(x)
    if x == 'a':
        GPIO.output(23,GPIO.HIGH)
        time.sleep(3)
    return x

def send_msg(msg):
    ser.write(msg)
    