#ifndef _HARDWARE_MANAGER_H_
#define _HARDWARE_MANAGER_H_

#include "hardware/MS5607.h"
#include "hardware/DFRobot_BMX160.h"
#include "hardware/PWM.h"
#include "hardware/SerialLinux.h"
#include "hardware/LED.h"

#include "ArduinoTimer.h"
#include "LogManager.h"

class HardwareManager
{
private:
    int RPI_I2C_BUS;
    LogManager * logManager;
public:
    HardwareManager(LogManager *logManager);
    bool start();
    void stop();
};

#endif