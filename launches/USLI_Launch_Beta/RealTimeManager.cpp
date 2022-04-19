#include <iostream>

#include "RealTimeManager.h"

RealTimeManager::RealTimeManager(LogManager *logManager, DataManager *dataManager, Configuration *configuration) {
    this->logManager = logManager;
    this->dataManager = dataManager;
    this->configuration = configuration;
}

bool RealTimeManager::start() {
    if (logManager == NULL) {
        std::cout << millis() << "\t[RealTimeManager] Error! logManager was not specified ❌\n";
    }

    if (dataManager == NULL) {
        std::cout << millis() << "\t[RealTimeManager] Error! dataManager was not specified ❌\n";
    }

    if (configuration == NULL) {
        std::cout << millis() << "\t[RealTimeManager] Error! configuration was not specified ❌\n";
    }

    std::cout << millis() << "\t[RealTimeManager] Successfully started ✔️\n";
    logManager->info("[RealTimeManager] Successfully started ✔️");
}

void RealTimeManager::checkAndHanleRadioData() {
    if (!configuration->send_real_time_data) {
        return;
    }

    if (!radioDataTimer.isTimeUp()) {
        return;
    }

    Serial.print(":");
    Serial.print(dataManager->getCurrentFlightState());
    Serial.print(",");
    Serial.print(dataManager->getRelativeAltitude());
    Serial.print(",");
    Serial.print(dataManager->getMaximumAltitude());
    Serial.print(",");
    Serial.print(dataManager->getProjectedAltitude());
    Serial.print(";");
    Serial.flush();

    radioDataTimer.start();
}

void RealTimeManager::run() {
    this->checkAndHanleRadioData();
}
void RealTimeManager::stop() {
    configuration->send_real_time_data = false;

    std::cout << millis() << "\t[RealTimeManager] Stopping all real time jobs\n";
    logManager->info("[RealTimeManager] Stopping all real time jobs");

    this->logManager = NULL;
    this->dataManager = NULL;
    this->configuration = NULL;
}