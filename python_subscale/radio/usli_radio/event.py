from enum import Enum, unique

from .command import Command
from .responce_status import ResponceStatus

@unique
class CommDevices(Enum):
    Kaza = '1'
    RPI = '2'
    Vova = '3'
    Jack = '4'
    GS = '5'
    Rocket = '6'

class Event:
    def __init__(self, command: Command, state: bool, data) -> None:
        self.command = command
        self.state = state
        self.data = data
        pass

    def __str__(self):
        state_val = "OFF"
        if (self.state):
            state_val = "ON"
        return str(self.command) + "\t" + str(state_val) + "\t" + str(self.data)