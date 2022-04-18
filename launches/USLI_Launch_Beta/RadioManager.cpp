#include <iostream>
#include "RadioManager.h"
#include "ArduinoTimer.h"


RadioManager::RadioManager(Configuration *configuration, LogManager *logManager)
{
    this->configuration = configuration;
    this->logManager = logManager;
}

bool RadioManager::start() {
    if (configuration == NULL) {
        std::cout << millis() << "\t[RadioManager] groundRequests was not specified ❌\n";
        logManager->error("[RadioManager] groundRequests was not specified ❌\n");
        return false;
    }

    std::cout << millis() << "\t[RadioManager] Successfully started ✔️\n";
    logManager->info("[RadioManager] Successfully started ✔️\n");
    return true;
}

void RadioManager::collect() {
    std::string request = "";

    // Get the data from the radio
    buffer += Serial.asyncReadString();
    
    for (int i = 0; i < buffer.length(); i++) {
        if (buffer[i] == START_BYTE) {
            request = "";
        }
        else if (buffer[i] == END_BYTE) {
            decodedRequests.push_back(request);

            std::string infoMessage = "[RadioManager] Received request " + request;
            std::cout << millis() << "\t[RadioManager] Received request " << request << "\n";
            logManager->info(infoMessage);
        }
        else {
            request += buffer[i];
        }
    }

    // Leave remaining not fully received message in buffer
    buffer = request;
}

void RadioManager::handle() {
    for (auto i = decodedRequests.begin(); i != decodedRequests.end(); ++i) {
        if (*i == "War Eagle") {
            configuration->arm = true;
        }
        else if (*i == "Servo Sweep") {
            configuration->run_servo_sweep = true;
        }
        else if (*i == "Real Time") {
            configuration->send_real_time_data = true;
        }
        else if (*i == "Shutdown") {
            configuration->shutdown = true;
        }
    }
}

void RadioManager::run() {
    this->collect();
    this->handle();
}