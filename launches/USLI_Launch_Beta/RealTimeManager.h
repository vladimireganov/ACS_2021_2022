#ifndef _REAL_TIME_MANAGER_H
#define _REAL_TIME_MANAGER_H

#include <iostream>
#include <string>

#include "hardware/SerialLinux.h"

#include "ArduinoTimer.h"
#include "LogManager.h"
#include "DataManager.h"
#include "SharedData.h"
#include "BuzzerManager.h"

#define RADIO_DATA_DURATION 500 //ms
#define FIVE_SECOND_DELAY 5000 //ms

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
    Timer armedStateTimer = Timer(FIVE_SECOND_DELAY);
    Timer landedStateTimer = Timer(FIVE_SECOND_DELAY);
    
    LogManager *logManager;
    DataManager *dataManager;
    Configuration *configuration;
    BuzzerManager *buzzerManager;

    void checkAndHanleRadioData();
    void checkAndHandleArmedState();
    void checkAndHandleLandedState();
public:
    RealTimeManager(LogManager *logManager, DataManager *dataManager, Configuration *configuration, BuzzerManager *buzzerManager);

    bool start();
    void run();
    void stop();
};

#endif