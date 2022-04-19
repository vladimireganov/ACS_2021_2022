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
#include "ArduinoTimer.h"

DataManager::DataManager(std::ofstream *dataFile, LogManager *logManager) {
    this->dataFile = dataFile;
    this->logManager = logManager;
}

void DataManager::writeHeaderToDataFile() {
    // raw data
    *dataFile << "iterator,";
    *dataFile << "time,";
    *dataFile << "elapsed_time,";
    *dataFile << "pressure,";
    *dataFile << "temperature,";
    *dataFile << "gyroscope_x,";
    *dataFile << "gyroscope_y,";
    *dataFile << "gyroscope_z,";
    *dataFile << "acceleration_x,";
    *dataFile << "acceleration_y,";
    *dataFile << "acceleration_z,";
    *dataFile << "magnetometer_x,";
    *dataFile << "magnetometer_y,";
    *dataFile << "magnetometer_z,";

    // derived data
    *dataFile << "altitude,";
    *dataFile << "relative_altitude,";
    *dataFile << "vertical_velocity,";
    *dataFile << "vertical_acceleration,";
    *dataFile << "net_acceleration,";
    *dataFile << "projected_altitude,";

    *dataFile << "\n";

    dataFile->flush();
}

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
    if (groundAltitude > altitude) {
        groundAltitude = altitude;
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
    // Prevent Zero Divsion Error
    if (elapsedTime == 0.0f) {
        std::cout << millis() << "\t[DataManager] Error! Elapsed time is 0, skipping vertical velocity calculation" << std::endl;
        logManager->error("[DataManager]Error! Elapsed time is 0, skipping vertical velocity calculation");
        return;
    }

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

    flightState.process_next_state(relativeAltitude, netAcceleration, currentTime);
    this->counter++;
}

void DataManager::store() {
    // raw data
    *dataFile << counter << ",";
    *dataFile << currentTime << ",";
    *dataFile << elapsedTime << ",";
    *dataFile << pressure << ",";
    *dataFile << altimeterTemperature << ",";
    *dataFile << gyroscopeX << ",";
    *dataFile << gyroscopeY << ",";
    *dataFile << gyroscopeZ << ",";
    *dataFile << accelerationX << ",";
    *dataFile << accelerationY << ",";
    *dataFile << accelerationZ << ",";
    *dataFile << magnetometerX << ",";
    *dataFile << magnetometerY << ",";
    *dataFile << magnetometerZ << ",";
    

    // processed data
    *dataFile << altitude << ",";
    *dataFile << relativeAltitude << ",";
    *dataFile << verticalVelocity << ",";
    *dataFile << verticalAcceleration << ",";
    *dataFile << netAcceleration << ",";
    *dataFile << projectedAltitude << ",";

    *dataFile << "\n";

    dataFile->flush();
}

bool DataManager::start() {
    if (dataFile == nullptr) {
        std::cout << millis() << "\t[DataManager] Error! Data file was not specified ❌" << std::endl;
        logManager->info("[DataManager] Error! Data file was not specified ❌");
        return false;
    }

    this->writeHeaderToDataFile();
    this->resetGroundAltitude();
    this->resetMaximumAltitude();

    std::cout << millis() << "\t[DataManager] Successfully started ✔️" << std::endl;
    logManager->info("[DataManager] Successfully started ✔️");
    return true;
}

void DataManager::run() {
    this->process();
    this->store();
}

void DataManager::stop() {
    dataFile->close();

    std::cout << millis() << "\t[DataManager] Data file is closed" << std::endl;
    logManager->info("[DataManager] Data file is closed");

    this->dataFile = NULL;
    this->logManager = NULL;
}