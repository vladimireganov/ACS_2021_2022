import sys
from usli_radio.log_manager import USLICommunicationLogger
from usli_radio.command import Command

from PyQt5.QtWidgets import (
    QApplication,
    QGridLayout,
    QHBoxLayout,
    QLabel,
    QPushButton,
    QVBoxLayout,
    QWidget,
)
from PyQt5.QtCore import Qt

from views.header import HeaderUI
from views.body import BodyUI
from views.footer import FooterUI

from usli_radio.device import RadioDevice
from usli_radio.event import CommDevices, Event

class Window(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("ASC Remote App Beta")

        # Create a QHBoxLayout instance
        layout = QVBoxLayout()

        self.headerWidget = HeaderUI()

        self.bodyWidget = BodyUI()

        self.footerWidget = FooterUI(button_callback)
        
        # Add Sensor Connection UI
        layout.addWidget(self.headerWidget)
        layout.addWidget(self.bodyWidget)
        layout.addWidget(self.footerWidget)
        self.setLayout(layout)

def event_handler(event: Event):
    global window
    window.headerWidget.updateFromEvent(event)
    window.bodyWidget.updateFromEvent(event)

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
        app = QApplication(sys.argv)
        global window
        window = Window()
        window.show()
        radio_device.run(event_handler)
        sys.exit(app.exec_())
    
    radio_device.stop()