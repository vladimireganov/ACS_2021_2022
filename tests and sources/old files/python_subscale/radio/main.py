import sys
from usli_radio.log_manager import USLICommunicationLogger
from usli_radio.command import Command


from usli_radio.device import RadioDevice
from usli_radio.event import CommDevices, Event

import time

if __name__ == "__main__":
    global radio_device

    # New SPACEPORTcode added below
    comm_logger = USLICommunicationLogger()
    # New SPACEPORT code ends here
    
    radio_device = RadioDevice("230400", "/dev/tty", CommDevices.Vova)

    #if radio_device.start():
    #    radio_device.run(event_handler)
    str = "0"
    print("start")
    radio_device.start()
    print("packet")
    # radio_device.transmit("Test")
    print("packet sent")
    while (str != "q"):
        # str = input()
        print(radio_device.radio_serial.readline())
        time.sleep(0.5)
        #radio_device.transmit("Test")
        radio_device.radio_serial.writelines("hi\0".encode('utf-8'))
        print("packet sent")
        

    radio_device.stop()