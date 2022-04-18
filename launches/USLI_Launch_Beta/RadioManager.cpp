#include <iostream>
#include "RadioManager.h"
#include "ArduinoTimer.h"


RadioManager::RadioManager(Requests *groundRequests, LogManager *logManager)
{
    this->groundRequests = groundRequests;
    this->logManager = logManager;
}

bool RadioManager::start() {
    if (groundRequests == NULL) {
        std::cout << millis() << "\t[RadioManager] groundRequests was not specified ❌\n";
        logManager->error("[RadioManager] groundRequests was not specified ❌\n");
        return false;
    }

    std::cout << millis() << "\t[RadioManager] Successfully started ✔️\n";
    logManager->info("[RadioManager] Successfully started ✔️\n");
    return true;
}

void RadioManager::collect() {
    std::string request = ""

    // Get the data from the radio
    buffer += Serial.asyncReadString();
    
    for (int i = 0; i < buffer.length(); i++) {
        if (buffer[i] == START_BYTE) {
            request = ""
        }
        else if (buffer[i] == END_BYTE) {
            decoded_requests.push_back(request);

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
    for (auto i = decoded_requests.begin(); i != decoded_requests.end(); ++i) {
        if (*i == "War Eagle") {
            groundRequests->arm = true;
        }
        else if (*i == "Servo Sweep") {
            groundRequests->run_servo_sweep = true;
        }
        else if (*i == "Real Time") {
            groundRequests->send_real_time_data = true;
        }
        else if (*i == "Shutdown") {
            groundRequests->shutdown = true;
        }
    }
}

void RadioManager::run() {
    this->collect();
    this->handle();
}