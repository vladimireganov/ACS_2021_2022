import serial
from enum import Enum, unique
from ctypes import alignment, c_double
import struct

import logging
import threading
import time

from .data_models import *
from .command import Command
from .event import Event, CommDevices
from .responce_status import ResponceStatus, to_responce_status

format = "%(asctime)s: %(message)s"
logging.basicConfig(format=format, level=logging.INFO,
                        datefmt="%H:%M:%S")

class RadioDevice:

    def __init__(self, baudrate, port, device_id: CommDevices, start_byte: str = '%', end_byte: str = ';'):
        self.baudrate = baudrate
        self.port = port
        self.device_id = device_id

        self.radio_serial = serial.Serial()
        self.radio_serial.baudrate = baudrate
        self.radio_serial.port = port
        self.radio_serial.timeout = 0.001
        self.connected = False
        self.started = False
        
        self.start_byte = start_byte
        self.end_byte = end_byte

        self.__communication_thread = threading.Thread(target=self.recieve, args=(1,), daemon=True)
        self.__stop_request = False
        self.func = None

    def start(self):
        try:
            self.radio_serial.open()
            self.started = True
            return True
        except serial.serialutil.SerialException as e:
            print(e)
        return False

    def recieve(self, name):
        logging.info("Radio Communication thread started")
        available = False
        parsed_data = bytes()
        character = bytes()

        while not self.__stop_request:
            if not self.radio_serial.is_open:
                continue

            # print('before recieve')
            try:
                character = self.radio_serial.read()
            except serial.serialutil.SerialException as e:
                print("Radio connection LOST!")
                # print(e)
                self.connected = False
                break

            if character == self.start_byte.encode():
                if available:
                    # print('Got some trash')
                    parsed_data = bytes()
                available = True
                continue

            if character == self.end_byte.encode():
                event = self.parse(parsed_data)
                if event:
                    self.func(event)

                parsed_data = bytes()
                available = False
                continue
            
            if character is not None and len(character) > 0:
                # print(character)
                parsed_data += character

    """
        Transmit command request
    """
    def request_bad(self, command: Command):
        transmit_packet = self.start_byte.encode('utf-8')
        transmit_packet += command.value.encode('utf-8')
        transmit_packet += self.device_id.value.encode('utf-8')
        transmit_packet += self.end_byte.encode('utf-8')
        # print(transmit_packet)
        return self.radio_serial.write(transmit_packet)

    def request(self, command: Command):
        transmit_packet = self.start_byte.encode('utf-8')
        transmit_packet += command.value.encode('utf-8')
        transmit_packet += self.end_byte.encode('utf-8')
        # print(transmit_packet)
        return self.radio_serial.write(transmit_packet)

    def transmit(self, data: str):
        transmit_packet = self.start_byte.encode('utf-8')
        transmit_packet += data.encode('utf-8')
        transmit_packet += self.end_byte.encode('utf-8')
        # print(transmit_packet)
        return self.radio_serial.write(transmit_packet)

    def parse(self, binary_packet):
        if binary_packet is not None and len(binary_packet) > 0:
            try:
                decoded_data = binary_packet.decode('utf-8')
            except UnicodeDecodeError:
                return (Command.DECODE_ERROR, binary_packet)
            
            if len(decoded_data) == 1:
                # print("list_data = 1")
                try:
                    command = Command(decoded_data[0])
                    return Event(command, True, None)
                except ValueError:
                    # print('Value error')
                    return Event(Command.PARSE_ERROR, False, decoded_data[1:])
            if len(decoded_data) == 2:
                # print("list_data = 2")
                try:
                    command = Command(decoded_data[0])
                    state = True if decoded_data[1] == "1" else False
                    return Event(command, state, None)
                except ValueError:
                    return Event(Command.PARSE_ERROR, False, decoded_data[1:])
            if len(decoded_data) > 2:
                # print("list_data > 2")
                try:
                    command = Command(decoded_data[0])
                    # print(decoded_data)

                    data = None
                    if command is Command.BNO_INFO:
                        data = BNOCalibrationPacket.from_string_packet(decoded_data[1:])
                    elif command is Command.SENSOR_INFO:
                        data = SensorPacket.from_string_packet(decoded_data[1:])
                    if not data:
                        data = FlightDataPacket.from_string_packet(decoded_data[1:])
                    if not data:
                        return Event(Command.PARSE_ERROR, False, decoded_data[1:])
                    return Event(command, True, data)
                except ValueError:
                    return Event(Command.PARSE_ERROR, False, decoded_data[1:])
            else:
                return Event(Command.NONE, False, None)
        
        return Event(Command.NONE, False, None)

    """
        Accepts function that is called as parameter
        Starts radio connection
        Receive and Parse data in thread
    """
    def run(self, func):
        if func:
            self.func = func
            self.start()
            if self.radio_serial.is_open:
                self.connected = True
                self.__communication_thread.start()

                return True
        else:
            raise ValueError('You must pass function that is called')
        
        return False
    
    """
        Stop radio thread and close connection
    """
    def stop(self):
        if self.started:
            self.__stop_request = True
            self.__communication_thread.join()
            self.radio_serial.close()