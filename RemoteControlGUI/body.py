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
            # self.verticalVelocityValueLabel.setText(f'{str(event.data.vertical_velocity)} m/s  {str(round(float(event.data.vertical_velocity) * 3.28, 2))} ft/s')
            # self.verticalAccelerationValueLabel.setText(f'{str(event.data.vertical_acceleration)} m/s^2  {str(round(float(event.data.vertical_acceleration) * 3.28, 2))} ft/ss')
            # self.temperatureValueLabel.setText(f'{str()} C\t{str(round(float(event.data.temperature) * 1.8 + 32, 2))} F')
        except ValueError:
            pass