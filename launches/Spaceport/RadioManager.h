#ifndef _RADIO_MANAGER_H_
#define _RADIO_MANAGER_H_

#include <iostream>
#include <string>
#include <vector>

#include "hardware/SerialLinux.h"

#include "ArduinoTimer.h"
#include "LogManager.h"
#include "SharedData.h"
#include "BuzzerManager.h"


class RadioManager
{
private:
    const char START_BYTE = ':';
    const char END_BYTE = ';';

    std::vector<std::string> decodedRequests;

    std::string buffer = "";
    Configuration *configuration;
    LogManager *logManager;
    BuzzerManager *buzzerManager;

    void collect();
    void handle();
public:
    RadioManager(Configuration *configuration, LogManager *logManager, BuzzerManager *buzzerManager);

    bool start();
    void run();
    void stop();
};

#endif