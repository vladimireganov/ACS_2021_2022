# ACS_2021_2022

This repository contains code and libraries for the ACS project (2021-2022).

## Folder Structure

- **libs/**  
  Contains reusable libraries for the project.

- **main.h**  
  The main header file for the project logic.

- **main.ino**  
  Main executable file for Teensy and Arduino-like boards.

- **testing/**  
  Holds all sensor and logic testing files.

- **last_year_libs/**  
  Example libraries from last year, written for Arduino-like boards.

## Sensors Used

- **MS5607**  
  Pressure sensor.

- **BMX160**  
  IMU sensor (accelerometer, gyroscope, magnetometer).

## Current Status

- Log file and data storage file creation: **Tested and working**
- Writing data into CSV file: **Implemented, not tested**
- Writing logs: **Implemented, not tested**
- Data storage: **Ready**

## To Do

- Connect sensors
- Read data from sensors
- Process sensor data
- Implement main code logic

## Notes

- Refer to `last_year_libs` for examples and reference implementations.
- All new code is targeted for Teensy boards.

Feel free to add more details or update as the project progresses.
