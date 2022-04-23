from PyQt5.QtWidgets import (
    QApplication,
    QGridLayout,
    QHBoxLayout,
    QLabel,
    QPushButton,
    QVBoxLayout,
    QWidget,
    QLineEdit
)

import functools

class FooterUI(QWidget):
    def __init__(self, button_callback) -> None:
        super().__init__()
        self.button_callback = button_callback

        footerLayout = QHBoxLayout()

        footerLayout.addWidget(self.sendCommandUI())
        footerLayout.addWidget(self.stateCommandUI())

        self.setLayout(footerLayout)

    def sendCommandUI(self) -> QWidget:
        sendCommandWidget = QWidget()
        sendCommandLayout = QGridLayout()

        echoButton = QPushButton('Hello')
        armSystemButton = QPushButton('SOS')
        servoSweepButton = QPushButton('Servo Sweep')

        echoButton.clicked.connect(functools.partial(self.button_callback, "Hello"))
        armSystemButton.clicked.connect(functools.partial(self.button_callback, "SOS"))
        servoSweepButton.clicked.connect(functools.partial(self.button_callback, "Servo Sweep"))

        sendCommandLayout.addWidget(echoButton, 0, 0)
        sendCommandLayout.addWidget(armSystemButton, 0, 1)
        sendCommandLayout.addWidget(servoSweepButton, 0, 2)

        sendCommandWidget.setLayout(sendCommandLayout)
        return sendCommandWidget

    def stateCommandUI(self) -> QWidget:
        stateCommandWidget = QWidget()
        stateCommandLayout = QGridLayout()

        armEBSBoneButton = QPushButton('Arm')
        realTimeDataButton = QPushButton('Real Time')
        shutdownButton = QPushButton('Shutdown')
        
        armEBSBoneButton.clicked.connect(functools.partial(self.button_callback, "War Eagle"))
        realTimeDataButton.clicked.connect(functools.partial(self.button_callback, "Real Time"))
        shutdownButton.clicked.connect(functools.partial(self.button_callback, "Shutdown"))

        stateCommandLayout.addWidget(armEBSBoneButton, 0, 0)
        stateCommandLayout.addWidget(realTimeDataButton, 0, 1)
        stateCommandLayout.addWidget(shutdownButton, 0, 2)

        stateCommandWidget.setLayout(stateCommandLayout)
        return stateCommandWidget