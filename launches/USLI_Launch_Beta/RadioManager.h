#ifndef _RADIO_MANAGER_H_
#define _RADIO_MANAGER_H_

#include <string>
#include <vector>
#include "hardware/SerialLinux.h"
#include "LogManager.h"
#include "SharedData.h"


class RadioManager
{
private:
    const char START_BYTE = ':';
    const char END_BYTE = ';';

    std::vector<std::string> decodedRequests;

    std::string buffer = "";
    Configuration *configuration;
    LogManager *logManager;

    void collect();
    void handle();
public:
    RadioManager(Configuration *configuration, LogManager *logManager);

    bool start();
    void run();
    void stop();
};

#endif