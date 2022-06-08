#ifndef _ALTITUDE_CONTROL_SYSTEM_H
#define _ALTITUDE_CONTROL_SYSTEM_H

#include <string>
#include <fstream>

#include "hardware/PWM.h"
#include "hardware/SerialLinux.h"

#include "SharedData.h"
#include "FileManager.h"
#include "LogManager.h"
#include "DataManager.h"
#include "BuzzerManager.h"


class AltitudeControlSystem {
private:
    float targetAltitude = 1280.16f; // in m 4200 ft

    Configuration *configuration;
    std::ofstream *servoLogFile;
    
    LogManager *logManager;
    DataManager *dataManager;
    BuzzerManager *buzzerManager;

    size_t timeout = 0;
    const int servoControlDelay = 150; //ms

    int estimateAngle(float projectedAltitude);

public:
    AltitudeControlSystem(Configuration *configuration,
            std::ofstream *servoLogFile, LogManager *logManager, DataManager *dataManager, BuzzerManager *buzzerManager);
    AltitudeControlSystem(Configuration *configuration,
            std::ofstream *servoLogFile, LogManager *logManager, DataManager *dataManager, BuzzerManager *buzzerManager,
            float targetAltitude);

    float getTargetAltitude() {return targetAltitude;}
    void setTargetAltitude(float targetAltitude);

    void checkAndHandleServoSweep();
    void checkAndHandleTargetAltitude();
    void controlAltitude();

    bool start();
    void run();
    void stop();
};

#endif