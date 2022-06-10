#ifndef _HARDWARE_MANAGER_H_
#define _HARDWARE_MANAGER_H_

#include <iostream>

#include "hardware/MS5607.h"
#include "hardware/DFRobot_BMX160.h"
#include "hardware/PWM.h"
#include "hardware/SerialLinux.h"
#include "hardware/LED.h"

#include "ArduinoTimer.h"
#include "LogManager.h"
#include "DataManager.h"

class HardwareManager
{
private:
    int RPI_I2C_BUS;
    MS5607 *ms5607_1;
    DFRobot_BMX160 *bmx160_1;

    bmx160SensorData Omagn, Ogyro, Oaccel;

    LogManager * logManager;
    DataManager * dataManager;
public:
    HardwareManager(LogManager *logManager, DataManager *dataManager);

    bool start();
    void stop();
    void run();
};

#endif