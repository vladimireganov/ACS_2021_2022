import sys
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot

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
from shared_data import SharedData

from body import BodyUI
from footer import FooterUI
from radio import USLI2022Radio

class Window(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("EBS Remote Control Beta")

        # Create a QHBoxLayout instance
        layout = QVBoxLayout()

        # self.headerWidget = HeaderUI()

        self.bodyWidget = BodyUI()

        self.footerWidget = FooterUI(button_callback)
        
        # Add Sensor Connection UI
        # layout.addWidget(self.headerWidget)
        layout.addWidget(self.bodyWidget)
        layout.addWidget(self.footerWidget)
        self.setLayout(layout)

def button_callback(command: str):
    # global radio_device
    # radio_device.transmit(command.value)
    print("Sent command:", command)
    global usli_radio
    usli_radio.send(command)

def data_event(messages):
    print("Received Data")
    print(messages)

    global shared_data
    shared_data.update(messages)

    global window
    window.bodyWidget.update(shared_data)
   
if __name__ == '__main__':
    global shared_data
    shared_data = SharedData()

    global usli_radio
    usli_radio = USLI2022Radio("/dev/tty.usbserial-AB0ML1P5")
    usli_radio.start(data_event)

    app = QApplication(sys.argv)
    global window
    window = Window()
    window.show()

    sys.exit(app.exec_())