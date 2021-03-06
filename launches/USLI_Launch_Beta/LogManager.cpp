#include "LogManager.h"

LogManager::LogManager(std::ofstream *logFileIn) {
    this->logFile = logFileIn;
}

LogManager::~LogManager() {}

void LogManager::body(std::string message) {
    *logFile << "\t";
    *logFile << millis();
    *logFile << "\t";
    *logFile << message.c_str();
    *logFile << "\n";

    logFile->flush();
}

void LogManager::info(std::string message) {
    *logFile << "INFO";
    this->body(message);
}
void LogManager::warning(std::string message) {
    *logFile << "WARNING";
    this->body(message);
}
void LogManager::error(std::string message) {
    *logFile << "ERROR";
    this->body(message);
}

bool LogManager::start() {
    if (logFile == nullptr) {
        std::cout << millis() << "\t[LogManager] Error! Data file was not specified ❌" << std::endl;
        return false;
    }

    std::cout << millis() << "\t[LogManager] Successfully started ✔️" << std::endl;
    this->info("[LogManager] Successfully started ✔️");
    return true;
}

void LogManager::run() {
    // design
}

void LogManager::stop() {
    logFile->close();
}