import sys
from usli_radio.log_manager import USLICommunicationLogger
from usli_radio.command import Command


from usli_radio.device import RadioDevice
from usli_radio.event import CommDevices, Event


def event_handler(event: Event):

    # New SPACEPORT code added below
    comm_logger.log_event(event)

    if event.command is Command.RBCAST:
        comm_logger.log_data(event.data)
    # New SPACEPORT code ends here

def button_callback(command: Command):
    global radio_device
    radio_device.transmit(command.value)

if __name__ == "__main__":
    global radio_device

    # New SPACEPORTcode added below
    comm_logger = USLICommunicationLogger()
    # New SPACEPORT code ends here
    
    radio_device = RadioDevice("230400", "/dev/tty.usbserial-DN06AA8K", CommDevices.Vova)

    if radio_device.start():
        radio_device.run(event_handler)
    str = "0"
    while (str != "q"):
        str = input()
    radio_device.stop()