#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <string>
#include <fstream>
#include <iostream>

#include "ArduinoTimer.h"

class LogManager
{
private:
    std::ofstream *logFile;
    void body(std::string message);

public:
    LogManager(std::ofstream *logFileIn);
    ~LogManager();
    void info(std::string message);
    void warning(std::string message);
    void error(std::string message);

    bool start();
    void run();
    void stop();
};

#endif