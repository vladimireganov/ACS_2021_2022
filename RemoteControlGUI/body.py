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

from shared_data import SharedData

class BodyUI(QWidget):
    def __init__(self):
        super().__init__()
        layout = QHBoxLayout()

        # Adding widgets from left to right
        layout.addWidget(self.flightpacketUI())
        layout.addWidget(self.niceDataUI())
        layout.addWidget(self.messagesUI())

        self.setLayout(layout) 

    def flightpacketUI(self) -> QWidget:
        flightpacketWidget = QWidget()
        flightpacketLayout = QHBoxLayout()

        flightpacketNameWidget = QWidget()
        flightpacketNameLayout = QVBoxLayout()
        armStateNameLable = QLabel('Arm State: ')
        flightStateNameLable = QLabel('Flight state: ')

        flightpacketNameLayout.addWidget(armStateNameLable)
        flightpacketNameLayout.addWidget(flightStateNameLable)
        flightpacketNameWidget.setLayout(flightpacketNameLayout)

        flightpacketValueWidget = QWidget()
        flightpacketValueLayout = QVBoxLayout()
        self.armStateValueLable = QLabel()
        self.flightStateValueLable = QLabel()

        flightpacketValueLayout.addWidget(self.armStateValueLable)
        flightpacketValueLayout.addWidget(self.flightStateValueLable)
        flightpacketValueWidget.setLayout(flightpacketValueLayout)

        flightpacketLayout.addWidget(flightpacketNameWidget)
        flightpacketLayout.addWidget(flightpacketValueWidget)
        flightpacketWidget.setLayout(flightpacketLayout)
        return flightpacketWidget

    def niceDataUI(self) -> QWidget:
        niceDataWidget = QWidget()
        niceDataLayout = QHBoxLayout()

        altitudeNameLabel = QLabel('Relative Altitude: ')
        projectedAltitudeNameLabel = QLabel('Projected Altitude: ')
        maxAltitudeNameLabel = QLabel('Max Altitude: ')
        verticalVelocityNameLabel = QLabel('Vertical Velocity')
        verticalAccelerationNameLabel = QLabel('Vertical Acceleration')
        temperatureNameLabel = QLabel('Temperature: ')
        

        self.altitudeValueLabel = QLabel('Loading...')
        self.projectedAltitudeValueLabel = QLabel('Loading...')
        self.maxAltitudeValueLabel = QLabel('Loading...')
        self.verticalVelocityValueLabel = QLabel('Loading...')
        self.verticalAccelerationValueLabel = QLabel('Loading...')
        self.temperatureValueLabel = QLabel('Loading...')

        niceDataNameWidget = QWidget()
        niceDataNameLayout = QVBoxLayout()

        niceDataNameLayout.addWidget(altitudeNameLabel)
        niceDataNameLayout.addWidget(projectedAltitudeNameLabel)
        niceDataNameLayout.addWidget(maxAltitudeNameLabel)
        niceDataNameLayout.addWidget(verticalVelocityNameLabel)
        niceDataNameLayout.addWidget(verticalAccelerationNameLabel)
        niceDataNameLayout.addWidget(temperatureNameLabel)
        niceDataNameWidget.setLayout(niceDataNameLayout)

        niceDataValueWidget = QWidget()
        niceDataValueLayout = QVBoxLayout()

        niceDataValueLayout.addWidget(self.altitudeValueLabel)
        niceDataValueLayout.addWidget(self.projectedAltitudeValueLabel)
        niceDataValueLayout.addWidget(self.maxAltitudeValueLabel)
        niceDataValueLayout.addWidget(self.verticalVelocityValueLabel)
        niceDataValueLayout.addWidget(self.verticalAccelerationValueLabel)
        niceDataValueLayout.addWidget(self.temperatureValueLabel)
        niceDataValueWidget.setLayout(niceDataValueLayout)

        niceDataLayout.addWidget(niceDataNameWidget)
        niceDataLayout.addWidget(niceDataValueWidget)
        niceDataWidget.setLayout(niceDataLayout)
        return niceDataWidget

    def update(self, shared_data: SharedData):
        try:
            self.armStateValueLable.setText(str(shared_data.arm_state))
            self.flightStateValueLable.setText(f'{str(shared_data.flight_state).split(".")[1]}')
            self.altitudeValueLabel.setText(f'{str(shared_data.relative_altitude)} m\t{str(round(float(shared_data.relative_altitude) * 3.28, 2))} ft')
            self.projectedAltitudeValueLabel.setText(f'{str(shared_data.projected_altitude)} m\t{str(round(float(shared_data.projected_altitude) * 3.28, 2))} ft')
            self.maxAltitudeValueLabel.setText(f'{str(shared_data.max_altitude)} m\t{str(round(float(shared_data.max_altitude) * 3.28, 2))} ft')
            self.verticalVelocityValueLabel.setText(f'{str(shared_data.vertical_velocity)} m/s  {str(round(float(shared_data.vertical_velocity) * 3.28, 2))} ft/s')
            self.verticalAccelerationValueLabel.setText(f'{str(shared_data.net_acceleration)} m/s^2  {str(round(float(shared_data.net_acceleration) * 3.28, 2))} ft/ss')
            self.temperatureValueLabel.setText(f'{str(shared_data.temperature)} C\t{str(round(float(shared_data.temperature) * 1.8 + 32, 2))} F')

        except ValueError:
            print("Value error!")
            pass

        if len(shared_data.messages) <= 10:
             for index, message in enumerate(shared_data.messages):
                self.radio_label_list[index].setText(message)

    def messagesUI(self):
        self.messages_str = "Radio Messages Appear Here"
        self.radio_messages_label_1 = QLabel(self.messages_str, self)
        self.radio_messages_label_2 = QLabel(self.messages_str, self)
        self.radio_messages_label_3 = QLabel(self.messages_str, self)
        self.radio_messages_label_4 = QLabel(self.messages_str, self)
        self.radio_messages_label_5 = QLabel(self.messages_str, self)
        self.radio_messages_label_6 = QLabel(self.messages_str, self)
        self.radio_messages_label_7 = QLabel(self.messages_str, self)
        self.radio_messages_label_8 = QLabel(self.messages_str, self)
        self.radio_messages_label_9 = QLabel(self.messages_str, self)
        self.radio_messages_label_10 = QLabel(self.messages_str, self)
  
        radioMessagesWidget = QWidget()
        radioMessagesLayout = QVBoxLayout()
        
        radioMessagesLayout.addWidget(self.radio_messages_label_1)
        radioMessagesLayout.addWidget(self.radio_messages_label_2)
        radioMessagesLayout.addWidget(self.radio_messages_label_3)
        radioMessagesLayout.addWidget(self.radio_messages_label_4)
        radioMessagesLayout.addWidget(self.radio_messages_label_5)
        radioMessagesLayout.addWidget(self.radio_messages_label_6)
        radioMessagesLayout.addWidget(self.radio_messages_label_7)
        radioMessagesLayout.addWidget(self.radio_messages_label_8)
        radioMessagesLayout.addWidget(self.radio_messages_label_9)
        radioMessagesLayout.addWidget(self.radio_messages_label_10)
        radioMessagesWidget.setLayout(radioMessagesLayout)

        self.radio_label_list = [
            self.radio_messages_label_1,
            self.radio_messages_label_2,
            self.radio_messages_label_3,
            self.radio_messages_label_4,
            self.radio_messages_label_5,
            self.radio_messages_label_6,
            self.radio_messages_label_7,
            self.radio_messages_label_8,
            self.radio_messages_label_9,
            self.radio_messages_label_10,
        ]

        return radioMessagesWidget