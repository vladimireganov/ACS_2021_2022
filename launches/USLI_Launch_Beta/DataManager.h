/**
 * @file DataManager.h
 * @author Kazybek Mizam (kzm0099@auburn.edu)
 * @brief Data manager process Altimeter and IMU data into useful derived data,
 *  and stores the results the into a file.
 * 
 * @version 0.1
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) AURA Embedded Systems 2022
 * 
 */
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <iostream>
#include <fstream>
#include "FlightMath.h"
#include "FlightStates.h"
#include "LogManager.h"

class DataManager {
    LogManager *logManager;

    // default values
    const float defaultGroundAltitude = 9999.9f;
    const float defaultMaximumAltitude = -9999.9f;

    // settings
    bool overrideAltitude = false;
    
    size_t counter = 0; // counter of processed data

    /* Timing */
    double currentTime = 0.0f;    // time in seconds since current data processing.
    double previousTime = 0.0f;   // time in seconds since previous data processing.
    double elapsedTime = 0.0f;     // time in seconds between previous and current data processing.
    
    // Altimeter data
    float pressure = 0.0f;
    float altimeterTemperature = 0.0f;

    // IMU data
    float imuTemperature = 0.0f;
    float accelerationX = 0.0f;
    float accelerationY = 0.0f;
    float accelerationZ = 0.0f;
    float gyroscopeX = 0.0f;
    float gyroscopeY = 0.0f;
    float gyroscopeZ = 0.0f;
    float magnetometerX = 0.0f;
    float magnetometerY = 0.0f;
    float magnetometerZ = 0.0f;

    // Altimeter derived data
    float altitude = 0.0f;
    float groundAltitude = 0.0f;
    float maximumAltitude = 0.0f;
    float relativeAltitude = 0.0f;
    float verticalVelocity = 0.0f;

    float previousAltitude = 0.0f;
    float previousRelativeAltitude = 0.0f;
    float previousVerticalVelocity = 0.0f;

    // IMU derived data
    float verticalAcceleration = 0.0f;
    float netAcceleration = 0.0f;

    // ACS Derived Data
    float projectedAltitude = 0.0f;

    RocketFlightStates flightState = RocketFlightStates();

    std::ofstream *dataFile;

    void calculateElapsedTime();
    void calculateGroundAltitude();
    void calculateMaximumAltitude();
    void calculateAltitude();
    void calculateRelativeAltitude();
    void calculateVerticalVelocity();
    void calculateNetAcceleration();
    void calculateProjectedAltitude();
    void writeHeaderToDataFile();
    
public:
    DataManager(std::ofstream *dataFile, LogManager *logManager);
    
    // Data Manager Configurations
    void setOverrideAltitude(bool value);
    void resetGroundAltitude();
    void resetMaximumAltitude();

    // Data Manager setters (inputs)
    void setTime(double newTime);
    void setPressure(float pressure);
    void setAltimeterTemperature(float altTemp);
    void setAltitude(float altitude); // overridden altitude value from sensor
    void setIMUTemperature(float imuTemp);
    void setAcceleration(float accelX, float accelY, float accelZ);
    void setGyroscope(float gyroX, float gyroY, float gyroZ);
    void setMagnetometer(float magX, float magY, float magZ);

    /* Data Manager getters (outputs) */
    size_t getCounter() {return counter;};
    
    // raw data getters
    double getTime() {return currentTime;};
    float getPressure() {return pressure;};
    float getAltimeterTemperature() {return altimeterTemperature;};
    float getAltitude() {return altitude;}; // overridden altitude value from sensor
    float getIMUTemperature() {return altitude;};
    float getAccelerationX() {return accelerationX;};
    float getAccelerationY() {return accelerationY;};
    float getAccelerationZ() {return accelerationZ;};
    float getGyroscopeX() {return gyroscopeX;};
    float getGyroscopeY() {return gyroscopeY;};
    float getGyroscopeZ() {return gyroscopeZ;};
    float getMagnetometerX() {return magnetometerX;};
    float getMagnetometerY() {return magnetometerY;};
    float getMagnetometerZ() {return magnetometerZ;};

    // derived data getters
    double getElapsedTime() {return elapsedTime;};
    float getGroundAltitude() {return groundAltitude;};
    float getMaximumAltitude() {return maximumAltitude;};
    float getRelativeAltitude() {return relativeAltitude;};
    float getVerticalVelocity() {return verticalVelocity;};
    float getNetAcceleration() {return netAcceleration;};
    float getProjectedAltitude() {return projectedAltitude;};

    FlightState getCurrentFlightState() {return flightState.current_state;};
    bool isFlightStateChanged() {return flightState.if_state_changed();};

    void process();
    void store();
    bool start();
    void run();
    void stop();
};

#endif