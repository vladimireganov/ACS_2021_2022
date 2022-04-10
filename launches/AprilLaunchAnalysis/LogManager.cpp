#include "LogManager.h"

LogManager::LogManager(std::ostream *logFileIn) {
    this->logFile = logFileIn;
}

LogManager::~LogManager() {}

void LogManager::body(std::string message) {
    *logFile << "\t";
    *logFile << seconds();
    *logFile << "\t";
    *logFile << message.c_str();
    *logFile << "\n";
}

void LogManager::info(std::string message) {
    *logFile << "INFO";
}
void LogManager::warning(std::string message) {
    *logFile << "WARNING";
}
void LogManager::error(std::string message) {
    *logFile << "ERROR";
}