#ifndef _ALTITUDE_CONTROL_SYSTEM_H
#define _ALTITUDE_CONTROL_SYSTEM_H

#include <string>
#include <fstream>

#include "hardware/PWM.h"
#include "SharedData.h"
#include "FileManager.h"
#include "LogManager.h"
#include "DataManager.h"


class AltitudeControlSystem {
private:
    float targetAltitude = 914.4f; // in m

    Configuration *configuration;
    std::ofstream *servoLogFile;
    
    LogManager *logManager;
    DataManager *dataManager;

    int estimateAngle(float projectedAltitude);

public:
    AltitudeControlSystem(Configuration *configuration,
            std::ofstream *servoLogFile, DataManager *dataManager, LogManager *logManager);
    AltitudeControlSystem(Configuration *configuration,
            std::ofstream *servoLogFile, DataManager *dataManager,
            LogManager *logManager, float targetAltitude);

    float getTargetAltitude() {return targetAltitude;}
    void setTargetAltitude(float targetAltitude);

    bool start();
    void run();
    void stop();
};

#endif