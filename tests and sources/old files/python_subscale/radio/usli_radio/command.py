from enum import Enum, unique

@unique
class Command(Enum):
    ECHO = 'a'
    ARM_T = 'b'
    ARM_BBB = 'c'
    DATA = 'd'
    SENSOR_INFO = 'e'
    BNO_INFO = 'f'
    UEFS = 'g'
    UEFE = 'h'
    RESET = 'i'
    START = 'j'
    RBCAST = 'k'
    ACS_TEST = 'l'
    BAD = 'z'
    GBCAST = 'y'
    TRASH = -1
    NONE = -2
    DECODE_ERROR = -3
    PARSE_ERROR = -4
    BAD_REQUEST = "@"
    NOT_FOUND = "D"