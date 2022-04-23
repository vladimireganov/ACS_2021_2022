from enum import Enum
from time import sleep

class FlightState(Enum):
    PRE_LAUNCH = 0
    LAUNCHED = 1
    BURNT_OUT = 2
    LANDING = 3
    LANDED = 4

class SharedData:
    def __init__(self) -> None:
        self.arm_state = ""

        self.elapsed_time = ""
        self.relative_altitude = ""
        self.max_altitude = ""
        self.projected_altitude = ""
        self.flight_state = FlightState.PRE_LAUNCH

        self.messages = []

    def update(self, data_list: list):
        self._process_messages(data_list)

        for data in data_list:
            self._process_arm_state(data)
            self._process_real_time_data(data)

    def _process_messages(self, messages: list):
        if len(self.messages) > 10:
            self.messages = self.messages[len(self.messages) - 10:]

        self.messages += messages

    def _process_arm_state(self, data: str):
        if "DISARMED" in data:
            sleep(3)
            self.arm_state = "DISARMED"
            return True
        
        elif "ARMED" in data:
            self.arm_state = "ARMED"
            return True

        return False
    
    def _process_real_time_data(self, data: str):
        if not isinstance(data, str):
            print("Error! data is not string")
            return False

        data_list = data.split(',')

        if len(data_list) != 5:
            print("Error! Invalid data")
            return False

        self.elapsed_time = data_list[0]
        self.relative_altitude = data_list[1]
        self.max_altitude = data_list[2]
        self.projected_altitude = data_list[3]
        self.vertical_velocity = data_list[4]
        self.net_acceleration = data_list[5]
        self.temperature = data_list[6]

        try:
            flight_state_value = int(data_list[7])
            self.flight_state = FlightState(flight_state_value)
        except TypeError:
            self.flight_state = data_list[7]
            print("Flight state decoding error")

        return True
        