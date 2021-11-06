


import time

class data:

    iterator : int
    iteration_time : int
    pressure : float
    linear_acceleration_x : float
    linear_acceleration_y : float
    linear_acceleration_z : float
    gyroscope_x : float
    gyroscope_y : float
    gyroscope_z : float
    acceleration_x : float
    acceleration_y : float
    acceleration_z : float
    gravity_x : float
    gravity_y : float
    gravity_z : float
    temperatue : float

    altitude : float

    def __init__(self) -> None:
        
        self.iterator = -1 #; // number of iteration

        self.iteration_time = time.time()   #; // time
        self.pressure = 0
        self.linear_acceleration_x = 0
        self.linear_acceleration_y = 0
        self.linear_acceleration_z = 0
        self.gyroscope_x = 0
        self.gyroscope_y = 0
        self.gyroscope_z = 0
        self.acceleration_x = 0
        self.acceleration_y = 0
        self.acceleration_z = 0
        self.gravity_x = 0
        self.gravity_y = 0
        self.gravity_z = 0
        self.altitude = 0.0
        self.temperatue =0.0
        
    
    def update_bmx(self,bmx):

        self.acceleration_x = bmx[6]
        self.acceleration_y = bmx[7]
        self.acceleration_z = bmx[8]

        self.gyroscope_x = bmx[3]
        self.gyroscope_y = bmx[4]
        self.gyroscope_z = bmx[5]
    
    def update_ms(self, temp, press, alt):
        self.pressure = press
        self.temperatue = temp
        self.altitude = alt
    
    def update_iter(self):
        self.iterator = self.iterator + 1

