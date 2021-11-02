# main file for sub scale if c++ brakes

#TO DO 

# add MS5607
# add file work
# 


import sys
sys.path.append('../')
import time

from DFRobot_BMX160 import BMX160
from MS5607 import MS5607

from data_class import data

from file_class import file_work

bmx = BMX160(1)
sensor = MS5607()

#begin return True if succeed, otherwise return False
while not bmx.begin():
    time.sleep(2)


def main():
    
    # test = data()

    f = file_work()
    f.start_files()
    while True:
        temperature = sensor.getDigitalTemperature()
        pressure = sensor.getDigitalPressure()
        converted = sensor.convertPressureTemperature(pressure, temperature)
        proper_temp = sensor.getTemperature()
        print("pressure = ", pressure)
        print("pressure in milibar = ", converted)
        print("temperature = ", proper_temp)
        altitude = sensor.getMetricAltitude(converted, sensor.inHgToHectoPascal(30.06))
        print(altitude, "\n")
        # print(sensor.getMetricAltitude(converted, sensor.inHgToHectoPascal(29.95)))
        bmx_data = bmx.get_all_data()
        
        print("magn: x: {0:.2f} uT, y: {1:.2f} uT, z: {2:.2f} uT".format(bmx_data[0],bmx_data[1],bmx_data[2]))
        print("gyro  x: {0:.2f} g, y: {1:.2f} g, z: {2:.2f} g".format(bmx_data[3],bmx_data[4],bmx_data[5]))
        print("accel x: {0:.2f} m/s^2, y: {1:.2f} m/s^2, z: {2:.2f} m/s^2".format(bmx_data[6],bmx_data[7],bmx_data[8]))
        print("\n")
        time.sleep(1)
        f.data.update_bmx(bmx_data)
        f.data.update_ms(proper_temp, converted ,altitude)
        f.data.update_iter()
        f.write_data()

if __name__ == "__main__":
    main()