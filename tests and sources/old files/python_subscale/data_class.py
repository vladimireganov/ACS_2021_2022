


class data:

    iterator : int
    iteration_time : int
    pressure : int
    linear_acceleration_x : int
    linear_acceleration_y : int
    linear_acceleration_z : int
    gyroscope_x : int
    gyroscope_y : int
    gyroscope_z : int
    acceleration_x : int
    acceleration_y : int
    acceleration_z : int
    gravity_x : int
    gravity_y : int
    gravity_z : int

    def __init__(self) -> None:
        
        self.iterator = 0 #; // number of iteration

        self.iteration_time = 0#; // time
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
    
    def update_bmx(self,bmx):

        self.acceleration_x = bmx[6]
        self.acceleration_y = bmx[7]
        self.acceleration_z = bmx[8]

        self.gyroscope_x = bmx[3]
        self.gyroscope_y = bmx[4]
        self.gyroscope_z = bmx[5]
