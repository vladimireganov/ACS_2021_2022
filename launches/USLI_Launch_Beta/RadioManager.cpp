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
        std::cout << millis() << "\t[RadioManager] configuration was not specified ❌\n";
        logManager->error("[RadioManager] configuration was not specified ❌");
        return false;
    }

    if (logManager == NULL) {
        std::cout << millis() << "\t[RadioManager] logManager was not specified ❌\n";
        logManager->error("[RadioManager] logManager was not specified ❌");
        return false;
    }

    std::cout << millis() << "\t[RadioManager] Successfully started ✔️\n";
    logManager->info("[RadioManager] Successfully started ✔️");
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
            Serial.println(infoMessage);
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
            configuration->arm = !configuration->arm;
            if (configuration->arm) {
                std::cout << millis() << "\t[RadioManager] System is ARMED\n";
                logManager->info("[RadioManager] System is ARMED");
                Serial.println("[RadioManager] System is ARMED");
            }
            else {
                std::cout << millis() << "\t[RadioManager] System is DISARMED\n";
                logManager->info("[RadioManager] System is DISARMED");
                Serial.println("[RadioManager] System is DISARMED");
            }
        }
        else if (*i == "Servo Sweep") {
            configuration->run_servo_sweep = true;
            std::cout << millis() << "\t[RadioManager] Processed Servo Sweep request\n";
            logManager->info("[RadioManager] Processed Servo Sweep request");
        }
        else if (*i == "Real Time") {
            configuration->send_real_time_data = !configuration->send_real_time_data;
            if (configuration->send_real_time_data) {
                std::cout << millis() << "\t[RadioManager] Real Time Data Enabled\n";
                logManager->info("[RadioManager] Real Time Data Enabled");
                Serial.println("[RadioManager] Real Time Data Enabled");
            }
            else {
                std::cout << millis() << "\t[RadioManager] Real Time Data Disabled\n";
                logManager->info("[RadioManager] Real Time Data Disabled");
                Serial.println("[RadioManager] Real Time Data Disabled");
            }
        }
        else if (*i == "Shutdown") {
            configuration->shutdown = true;
            std::cout << millis() << "\t[RadioManager] Processed Shutdown request\n";
            logManager->info("[RadioManager] Processed Shutdown request");
        }
    }

    decodedRequests.clear();
}

void RadioManager::run() {
    this->collect();
    this->handle();
}

void RadioManager::stop() {
    Serial.flush();
    std::cout << millis() << "\t[RadioManager] Stopping radio processing\n";
    logManager->info("[RadioManager] Stopping radio processing");

    this->configuration = NULL;
    this->logManager = NULL;
}