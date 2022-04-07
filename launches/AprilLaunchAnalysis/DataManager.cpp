/**
 * @file DataManager.h
 * @author Kazybek Mizam (kzm0099@auburn.edu)
 * @brief Data manager process Altimeter and IMU data into useful derived data,
 *  and stores the results the into a file.
 * 
 * @version 0.1
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) AURA Embedded Systems 2022.
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

void DataManager::setOverrideAltitude(bool value) {
    overrideAltitude = value;
}

void DataManager::resetGroundAltitude() {
    groundAltitude = defaultGroundAltitude;
}

void DataManager::resetMaximumAltitude() {
    maximumAltitude = defaultMaximumAltitude;
}

void DataManager::calculateElapsedTime() {
    elapsedTime = currentTime - previousTime;
    previousTime = currentTime;
}

void DataManager::calculateGroundAltitude() {
    if (groundAltitude > relativeAltitude) {
        groundAltitude = relativeAltitude;
    }
}

void DataManager::calculateMaximumAltitude() {
    if (maximumAltitude < relativeAltitude) {
        maximumAltitude = relativeAltitude;
    }
}

void DataManager::calculateAltitude() {
    previousAltitude = altitude;

    if (!overrideAltitude) {
        altitude = altitude_from_pressure(pressure);
    }

    altitude = low_pass_filter(previousAltitude, altitude, elapsedTime);
}

void DataManager::calculateRelativeAltitude() {
    previousRelativeAltitude = relativeAltitude;
    relativeAltitude = altitude - groundAltitude;
}

void DataManager::calculateVerticalVelocity() {
    previousVerticalVelocity = verticalVelocity;
    verticalVelocity = vertical_velocity(relativeAltitude, previousRelativeAltitude, elapsedTime);
    verticalVelocity = low_pass_filter(previousVerticalVelocity, verticalVelocity, elapsedTime);
}

void DataManager::calculateNetAcceleration() {
    netAcceleration = net_value(accelerationX, accelerationY, accelerationZ);
}

void DataManager::calculateProjectedAltitude() {
    projectedAltitude = simple_projected_altitude(relativeAltitude, verticalVelocity);
}

void DataManager::process() {
    this->calculateElapsedTime();
    this->calculateGroundAltitude();
    this->calculateMaximumAltitude();
    this->calculateAltitude();
    this->calculateRelativeAltitude();
    this->calculateVerticalVelocity();
    this->calculateNetAcceleration();
    this->calculateProjectedAltitude();
}