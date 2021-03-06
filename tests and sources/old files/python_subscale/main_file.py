# main file for sub scale if c++ brakes

#TO DO 

# add MS5607
# add file work
# 


import sys
sys.path.append('../')
import time

from DFRobot_BMX160 import BMX160

from data_class import data

from file_class import file_work

bmx = BMX160(1)

#begin return True if succeed, otherwise return False
while not bmx.begin():
    time.sleep(2)


def main():
    
    #test = data

    f = file_work
    f.start_files()
    while True:
        bmx_data = bmx.get_all_data()
        time.sleep(1)
        print("magn: x: {0:.2f} uT, y: {1:.2f} uT, z: {2:.2f} uT".format(bmx_data[0],bmx_data[1],bmx_data[2]))
        print("gyro  x: {0:.2f} g, y: {1:.2f} g, z: {2:.2f} g".format(bmx_data[3],bmx_data[4],bmx_data[5]))
        print("accel x: {0:.2f} m/s^2, y: {1:.2f} m/s^2, z: {2:.2f} m/s^2".format(bmx_data[6],bmx_data[7],bmx_data[8]))
        print(" ")

if __name__ == "__main__":
    main()