from usli_radio.command import Command
import os
from datetime import datetime
from .event import Event
from .data_models import FlightDataPacket

# Tested USLI console working version please do not change!

class USLICommunicationLogger:
    def __init__(self) -> None:
        file_creation_time_str = datetime.now().strftime("%Y-%m-%d-%H-%M")
        self.log_file_name = file_creation_time_str + '.txt'
        self.datalog_file_name = file_creation_time_str + '.csv'

        self.str_time = datetime.now().strftime("%Y-%m-%d %H:%M:%f")

        self.folder_path = 'logs'
        if not os.path.isdir(self.folder_path):
            os.mkdir(self.folder_path)
        
        

    def log(self, data):
        with open(self.folder_path + self.log_file_name, 'a') as data_file:
            data_file.write(str(data))
            data_file.write('\n')

    def log_event(self, event:Event):
        if event.command is not Command.RBCAST:
            self.str_time = datetime.now().strftime("%Y-%m-%d %H:%M:%f")
            with open(self.folder_path + '/' + self.log_file_name, 'a') as data_file:
                data_file.write(self.str_time)
                data_file.write('\t')
                data_file.write(str(event.command.name))
                data_file.write('\t')
                data_file.write(str(event.state))
                data_file.write('\t')
                data_file.write(str(event.data))
                data_file.write('\n')

    def log_data(self, flight_data:FlightDataPacket):
        with open(self.folder_path + '/' + self.datalog_file_name, 'a') as data_file:
            data_file.write(datetime.now().strftime("%Y-%m-%d %H:%M:%f"))
            data_file.write(',')
            for i in flight_data.to_list():
                data_file.write(str(i))
                data_file.write(',')
            data_file.write('\n')