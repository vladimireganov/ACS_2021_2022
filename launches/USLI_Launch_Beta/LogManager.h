#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H
#include <string>
#include <fstream>

#include "ArduinoTimer.h"

class LogManager
{
private:
    std::ostream *logFile;
    void body(std::string message);

public:
    LogManager(std::ostream *logFileIn);
    ~LogManager();
    void info(std::string message);
    void warning(std::string message);
    void error(std::string message);
};

#endif;