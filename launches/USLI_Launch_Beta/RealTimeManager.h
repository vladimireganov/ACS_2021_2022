#ifndef _REAL_TIME_MANAGER_H
#define _REAL_TIME_MANAGER_H

#include <iostream>
#include <string>

#include "hardware/SerialLinux.h"

#include "ArduinoTimer.h"
#include "LogManager.h"
#include "DataManager.h"
#include "SharedData.h"

#define RADIO_DATA_DURATION 500 //ms

class Timer
{
private:
    size_t timeout = 0;
    size_t duration = 0;

public:
    Timer(size_t duration) {this->duration = duration;};

    void start() { timeout = millis() + duration; };
    bool isTimeUp() { return timeout < millis(); };
};


class RealTimeManager
{
private:
    Timer radioDataTimer = Timer(RADIO_DATA_DURATION);
    
    LogManager *logManager;
    DataManager *dataManager;
    Configuration *configuration;

    void checkAndHanleRadioData();
public:
    RealTimeManager(LogManager *logManager, DataManager *dataManager, Configuration *configuration);

    bool start();
    void run();
    void stop();
};

#endif