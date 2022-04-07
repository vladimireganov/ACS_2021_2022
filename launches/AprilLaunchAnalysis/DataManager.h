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

#include "FlightMath.h"

class DataManager {

    // default values
    const float defaultGroundAltitude = 9999.9f;
    const float defaultMaximumAltitude = -9999.9f;

    // settings
    bool overrideAltitude = false;
    
    size_t counter = 0; // counter of processed data

    /* Timing */
    double currentTime;    // time in seconds since current data processing.
    double previousTime;   // time in seconds since previous data processing.
    double elapsedTime;     // time in seconds between previous and current data processing.
    
    // Altimeter data
    float pressure;
    float altimeterTemperature;

    // IMU data
    float imuTemperature;
    float accelerationX;
    float accelerationY;
    float accelerationZ;
    float gyroscopeX;
    float gyroscopeY;
    float gyroscopeZ;
    float magnetometerX;
    float magnetometerY;
    float magnetometerZ;

    // Altimeter derived data
    float altitude;
    float groundAltitude;
    float maximumAltitude;
    float relativeAltitude;
    float verticalVelocity;

    float previousAltitude;
    float previousRelativeAltitude;
    float previousVerticalVelocity;

    // IMU derived data
    float verticalAcceleration;
    float netAcceleration;

    // ACS Derived Data
    float projectedAltitude;


    void calculateElapsedTime();
    void calculateGroundAltitude();
    void calculateMaximumAltitude();
    void calculateAltitude();
    void calculateRelativeAltitude();
    void calculateVerticalVelocity();
    void calculateNetAcceleration();
    void calculateProjectedAltitude();
    
public:// void calculate_quaternions();
    // void calculate_altitude();
    // void calculate_ground_altitude();
    // void calculate_relative_altitude();
    // void calculate_vertical_velocity();
    // void calculate_net_acceleration();
    // void calculate_vertical_acceleration();
    // void calculate_projected_altitude();

    // void calculate_raw_pitch_yaw(float qw, float qx, float qy, float qz);
    
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

    void process();
    void store();
};

#endif;