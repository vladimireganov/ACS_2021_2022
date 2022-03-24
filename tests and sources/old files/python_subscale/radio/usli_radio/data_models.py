from enum import Enum, unique

@unique
class ArmState(Enum):
    DISARMED = 0
    ARMED = 1
    RESTARTED = 2

@unique
class FlightState(Enum):
    WAIT = 0
    STANDBY = 1
    POWERED_ASCENT = 2
    UNPOWERED_ASCENT = 3
    DESCENT_TO_LAND = 4
    LANDED = 5
    TEST = 6

class FlightDataPacket:
    def __init__(self, armed_state: ArmState, flight_state: FlightState, flight_state_bbb: FlightState,
            use_ext_est, use_ext_fst, altitude, proj_alt, max_alt, vertical_acceleration,
            vertical_velocity, latitude, longitude, temperature) -> None:
        self.armed_state = armed_state
        self.flight_state = flight_state
        self.flight_state_bbb = flight_state_bbb
        self.use_ext_est = use_ext_est
        self.use_ext_fst = use_ext_fst
        self.altitude = altitude
        self.proj_alt = proj_alt
        self.max_alt = max_alt
        self.vertical_acceleration = vertical_acceleration
        self.vertical_velocity = vertical_velocity
        self.latitude = latitude
        self.longitude = longitude
        self.temperature = temperature

    @classmethod
    def from_string_packet(cls, packet):
        data = packet.split(',') if packet else None
        if data and len(data) == 13:
            armed_state = ArmState(int(data[0]))
            flight_state = FlightState(int(data[1]))
            flight_state_bbb = FlightState(int(data[2]))
            use_ext_fst = data[3]
            use_ext_est = data[4]
            altitude = data[5]
            proj_alt = data[6]
            max_alt = data[7]
            vertical_acceleration = data[8]
            vertical_velocity = data[9]
            latitude = data[10]
            longitude = data[11]
            temperature = data[12]

            # print(f'Arm State: {armed_state}')
            # print(f'EXT EST: {use_ext_est}')
            # print(f'Flight state: {flight_state}')
            # print(f'Altitude: {altitude}')
            # print(f'Max Altitude: {max_alt}')
            # print(f'Projected Altitude: {proj_alt}')
            # print(f'Vertical Acceleration: {vertical_acceleration}')
            # print(f'Vertical Velocity: {vertical_velocity}')
            # print(f'Latitude: {latitude}')
            # print(f'Longitude: {longitude}')

            return cls(armed_state, flight_state, flight_state_bbb, use_ext_est, use_ext_fst, \
                altitude, proj_alt, max_alt, vertical_acceleration, vertical_velocity, latitude, \
                longitude, temperature)
        return None

    def __str__(self) -> str:
        print(f'Arm State: {self.armed_state}')
        print(f'EXT EST: {self.use_ext_est}')
        print(f'EXT FST: {self.use_ext_fst}')
        print(f'Flight state teensy: {self.flight_state}')
        print(f'Flight state bbb: {self.flight_state_bbb}')
        print(f'Altitude: {self.altitude}')
        print(f'Max Altitude: {self.max_alt}')
        print(f'Projected Altitude: {self.proj_alt}')
        print(f'Vertical Acceleration: {self.vertical_acceleration}')
        print(f'Vertical Velocity: {self.vertical_velocity}')
        print(f'Latitude: {self.latitude}')
        print(f'Longitude: {self.longitude}')
        print(f'Temperature: {self.temperature}')
        return "^^^"

    def to_list(self):
        return [
            self.armed_state.value,
            self.use_ext_est,
            self.use_ext_fst,
            self.flight_state.value,
            self.flight_state_bbb.value,
            self.altitude,
            self.max_alt,
            self.proj_alt,
            self.vertical_acceleration,
            self.vertical_velocity,
            self.latitude,
            self.longitude,
            self.temperature
        ]

    def to_broadcast_dict(self):
        return {
            "flight_state": self.flight_state.value,
            "outside_temp": self.temperature,
            "altitude": self.altitude,
            "max_altitude": self.max_alt,
            "projected_alt": self.proj_alt,
            "vertical_acceleration": self.vertical_acceleration,
            "vertical_velocity": self.vertical_velocity,
            "latitude": self.latitude,
            "longitude": self.longitude
        }

class SensorPacket:
    def __init__(self, bno, bmp, sd, gps) -> None:
        self.bno = bno
        self.bmp = bmp
        self.sd = sd
        self.gps = gps

    @classmethod
    def from_string_packet(cls, packet):
        data = packet.split(',') if packet else None
        if data and len(data) == 4:
            try:
                bno = int(data[0])
                bmp = int(data[1])
                sd = int(data[2])
                gps = int(data[3])
            except ValueError:
                bno = -1
                bmp = -1
                sd = -1
                gps = -1

            # print(bno)
            # print(bmp)
            # print(sd)
            # print(gps)

            return cls(bno, bmp, sd, gps)
        return None

    def to_dict(self):
        return {
            'bno': self.bno,
            'bmp': self.bmp,
            'sd': self.sd,
            'gps': self.gps,
        }

    def to_list(self):
        return [
            self.bno,
            self.bmp,
            self.sd,
            self.gps,
        ]

    def __str__(self) -> str:
        if (self.bno):
            print(f'BNO Connected')
        else:
            print(f'BNO NOT Connected')

        if (self.bmp):
            print(f'BMP Connected')
        else:
            print(f'BNO NOT Connected')

        if (self.sd):
            print(f'SD Connected')
        else:
            print(f'BNO NOT Connected')

        if (self.gps):
            print(f'GPS fixed')
        else:
            print(f'GPS NOT fixed')
        return "^^^"

class BNOCalibrationPacket:
    def __init__(self, system, gyroscope, accelerometer, magnetometer) -> None:
        self.system = system
        self.gyroscope = gyroscope
        self.accelerometer = accelerometer
        self.magnetometer = magnetometer

    @classmethod
    def from_string_packet(cls, packet):
        data = packet.split(',') if packet else None
        if data and len(data) == 4:
            try:
                system = int(data[0])
                gyroscope = int(data[1])
                accelerometer = int(data[2])
                magnetometer = int(data[3])
            except ValueError:
                system = -1
                gyroscope = -1
                accelerometer = -1
                magnetometer = -1

            # print(bno)
            # print(bmp)
            # print(sd)
            # print(gps)

            return cls(system, gyroscope, accelerometer, magnetometer)
        return None

    def to_dict(self):
        return {
            'system': self.system,
            'gyroscope': self.gyroscope,
            'accelerometer': self.accelerometer,
            'magnetometer': self.magnetometer,
        }

    def to_list(self):
        return [
            self.system,
            self.gyroscope,
            self.accelerometer,
            self.magnetometer,
        ]
    
    def __str__(self) -> str:
        print("BNO Calibration")
        print(f'System: {self.system}')
        print(f'Gyroscope: {self.gyroscope}')
        print(f'Accelerometer: {self.accelerometer}')
        print(f'Magnetometer: {self.magnetometer}')
        return "^^^"