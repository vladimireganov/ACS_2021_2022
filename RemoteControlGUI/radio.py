import threading
import serial

from shared_data import SharedData

class USLI2022Radio:
    START_BYTE = ':'
    END_BYTE = ';'
    
    def __init__(self, port: str, baudrate: int = 9600) -> None:
        self.radio_serial = serial.Serial()
        self.radio_serial.port = port
        self.radio_serial.baudrate = baudrate
        self.radio_serial.timeout = 0.001

        self.messages = []
        self._radio_listening_thread = threading.Thread(target=self._listen, args=(), daemon=True)

        self.stop_listening = False
        self.func = None

    def start(self, func):
        self.func = func

        self.radio_serial.open()
        self._radio_listening_thread.start()

    def _process(self, message):
        buf = ""

        for char in message:
            if not (char == self.START_BYTE or char == self.END_BYTE or char == '\n'):
                buf += char
                continue

            if buf != "":
                self.messages.append(buf)
                buf = ""
        
        self.func(self.messages)
        self.messages = []

    def _listen(self):
        message = ""

        while not self.stop_listening:
            c = self.radio_serial.read()

            while c:
                message += c.decode('utf-8')
                c = self.radio_serial.read()
            
            self._process(message)
            message = ""

    def stop(self):
        self.stop_listening = True
        self._radio_listening_thread.join()
        self.radio_serial.close()

    def send(self, message: str):
        self.radio_serial.write(self.START_BYTE.encode('utf-8'))
        self.radio_serial.write(message.encode('utf-8'))
        self.radio_serial.write(self.END_BYTE.encode('utf-8'))