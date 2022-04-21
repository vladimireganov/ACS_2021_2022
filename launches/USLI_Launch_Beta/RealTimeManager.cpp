#include "RealTimeManager.h"

RealTimeManager::RealTimeManager(LogManager *logManager, DataManager *dataManager,
        Configuration *configuration, BuzzerManager *buzzerManager) {
    this->logManager = logManager;
    this->dataManager = dataManager;
    this->configuration = configuration;
    this->buzzerManager = buzzerManager;
}

bool RealTimeManager::start() {
    if (logManager == NULL) {
        std::cout << millis() << "\t[RealTimeManager] Error! logManager was not specified ❌\n";
        logManager->info("[RealTimeManager] Error! logManager was not specified ❌");
        return false;
    }

    if (dataManager == NULL) {
        std::cout << millis() << "\t[RealTimeManager] Error! dataManager was not specified ❌\n";
        logManager->info("[RealTimeManager]Error! dataManager was not specified ❌");
        return false;
    }

    if (configuration == NULL) {
        std::cout << millis() << "\t[RealTimeManager] Error! configuration was not specified ❌\n";
        logManager->info("[RealTimeManager] Error! configuration was not specified ❌");
        return false;
    }

    if (buzzerManager == NULL) {
        std::cout << millis() << "\t[RealTimeManager] Error! buzzerManager was not specified ❌\n";
        logManager->info("[RealTimeManager] Error! buzzerManager was not specified ❌");
        return false;
    }

    std::cout << millis() << "\t[RealTimeManager] Successfully started ✔️\n";
    logManager->info("[RealTimeManager] Successfully started ✔️");

    return true;
}

void RealTimeManager::checkAndHanleRadioData() {
    if (!configuration->send_real_time_data) {
        return;
    }

    if (!radioDataTimer.isTimeUp()) {
        return;
    }

    std::cout << millis() << "\t[RealTimeManager] Timeup! Sending data over radio.\n";
    logManager->info("[RealTimeManager] Timeup! Sending data over radio.");
    int flightState = static_cast<int>(dataManager->getCurrentFlightState());

    Serial.print(":");
    Serial.print(std::to_string(dataManager->getElapsedTime()).c_str());
    Serial.print(",");
    Serial.print(std::to_string(dataManager->getRelativeAltitude()).c_str());
    Serial.print(",");
    Serial.print(std::to_string(dataManager->getMaximumAltitude()).c_str());
    Serial.print(",");
    Serial.print(std::to_string(dataManager->getProjectedAltitude()).c_str());
    Serial.print(",");
    Serial.print(std::to_string(flightState).c_str());
    Serial.print(";");

    radioDataTimer.start();
}

void RealTimeManager::run() {
    this->checkAndHanleRadioData();
    this->checkAndHandleArmedState();
    this->checkAndHandleLandedState();
}

void RealTimeManager::stop() {
    configuration->send_real_time_data = false;

    std::cout << millis() << "\t[RealTimeManager] Stopping all real time jobs\n";
    logManager->info("[RealTimeManager] Stopping all real time jobs");

    this->logManager = NULL;
    this->dataManager = NULL;
    this->configuration = NULL;
}

void RealTimeManager::checkAndHandleArmedState() {
    if (!configuration->arm) {
        return;
    }

    if (dataManager->getCurrentFlightState() == FlightState::LANDED) {
        // Do not activate during landed state, because other sound is requested
        return;
    }

    if (!armedStateTimer.isTimeUp()) {
        return;
    }

    buzzerManager->confirmationThreeSound();

    armedStateTimer.start();
}

void RealTimeManager::checkAndHandleLandedState() {
    if (dataManager->getCurrentFlightState() != FlightState::LANDED) {
        return;
    }

    if (!landedStateTimer.isTimeUp()) {
        return;
    }

    buzzerManager->sosSound();

    landedStateTimer.start();
}