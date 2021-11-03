from enum import Enum

class ResponceStatus(Enum):
    PROCESSING = 0x12
    NOT_FOUND = 0x44
    BAD_REQUEST = 0x40
    TIMEOUT = 0x54
    SUCCESS = 0x20
    PARSE_ERROR = 'parse_error'

def to_responce_status(val):
    if (val == '12'):
        return ResponceStatus.PROCESSING
    if (val == '44'):
        return ResponceStatus.NOT_FOUND
    if (val == '40'):
        return ResponceStatus.BAD_REQUEST
    if (val == '54'):
        return ResponceStatus.TIMEOUT
    if (val == '20'):
        return ResponceStatus.SUCCESS
    return ResponceStatus.PARSE_ERROR