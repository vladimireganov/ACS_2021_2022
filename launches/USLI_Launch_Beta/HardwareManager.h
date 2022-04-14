#ifndef _HARDWARE_MANAGER_H_
#define _HARDWARE_MANAGER_H_

#include "hardware/MS5607.h"
#include "hardware/DFRobot_BMX160.h"
#include "hardware/PWM.h"
#include "hardware/SerialLinux.h"
#include "hardware/LED.h"

class HardwareManager
{
private:
public:
    bool start();
    bool stop();
};

#endif