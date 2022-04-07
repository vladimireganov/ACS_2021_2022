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

#include "DataManager.h"

void DataManager::setTime(double newTime) {this->currentTime = newTime;}

void DataManager::setPressure(float pressure) {this->pressure = pressure;}

void DataManager::setAltimeterTemperature(float altTemp) {this->altimeterTemperature = altTemp;}

void DataManager::setAltitude(float altitude) {this->altitude = altitude;}

void DataManager::setIMUTemperature(float imuTemp) {this->imuTemperature = imuTemp;}

void DataManager::setAcceleration(float accelX, float accelY, float accelZ) {
    this->accelerationX = accelX;
    this->accelerationY = accelY;
    this->accelerationZ = accelZ;
}

void DataManager::setGyroscope(float gyroX, float gyroY, float gyroZ) {
    this->gyroscopeX = gyroX;
    this->gyroscopeY = gyroY;
    this->gyroscopeZ = gyroZ;
}

void DataManager::setMagnetometer(float magX, float magY, float magZ) {
    this->magnetometerX = magX;
    this->magnetometerY = magY;
    this->magnetometerZ = magZ;
}

