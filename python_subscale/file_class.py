
from data_class import data


class file_work:
    def __init__(self):
        self.log_f = open("log.txt", "x")
        self.data_f = open("data.scv", "x")
        self.data = data()
    def start_files(self):
        self.log_f.write("log file sucessfully created\n")
        self.data_f.write("iterator,iteration_time,pressure,linear_acceleration_x,linear_acceleration_y,linear_acceleration_z,gyroscope_x,gyroscope_y,gyroscope_z,acceleration_x,acceleration_y,acceleration_z,gravity_x,gravity_y,gravity_z\n")
        self.data_f.flush()
        self.log_f.flush()
    
    def end_files(self):
        self.data_f.flush()
        self.log_f.flush()
        self.data_f.close()
        self.log_f.close()

    def write_log(self,log):
        self.log_f.write(log)
        self.log_f.flush()
        
    
    def write_data(self):
        self.data_f.write(str(self.data.iterator))
        self.data_f.write(",")
        self.data_f.write(str(self.data.iteration_time))
        self.data_f.write(",")
        self.data_f.write(str(self.data.pressure))
        self.data_f.write(",")
        self.data_f.write(str(self.data.linear_acceleration_x))
        self.data_f.write(",")
        self.data_f.write(str(self.data.linear_acceleration_y))
        self.data_f.write(",")
        self.data_f.write(str(self.data.linear_acceleration_z))
        self.data_f.write(",")
        self.data_f.write(str(self.data.gyroscope_x))
        self.data_f.write(",")
        self.data_f.write(str(self.data.gyroscope_y))
        self.data_f.write(",")
        self.data_f.write(str(self.data.gyroscope_z))
        self.data_f.write(",")
        self.data_f.write(str(self.data.acceleration_x))
        self.data_f.write(",")
        self.data_f.write(str(self.data.acceleration_y))
        self.data_f.write(",")
        self.data_f.write(str(self.data.acceleration_z))
        self.data_f.write(",")
        self.data_f.write(str(self.data.gravity_x))
        self.data_f.write(",")
        self.data_f.write(str(self.data.gravity_y))
        self.data_f.write(",")
        self.data_f.write(str(self.data.gravity_z))
        self.data_f.write("\n")


