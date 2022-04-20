#include <iostream>

#include "ButtonManager.h"

ButtonManager::ButtonManager(Configuration *configuration, LogManager *logManager) {
    this->configuration = configuration;
    this->logManager = logManager;
}

void ButtonManager::listen() {
    int currentButtonState = readButton();

    if (currentButtonState != previousButtonState) {
        size_t elapsedTime = millis() - startTime;
        ButtonEvent buttonEvent = new ButtonEvent(previousButtonState, elapsedTime);

        previousEvents.push_back(buttonEvent);

        previousButtonState = currentButtonState;
        startTime = millis();
    }
}

void ButtonManager::process() {
    if (previousEvents.empty()) {
        return;
    }

    size_t currentElapsedTime = millis() - startTime;
    ButtonState currentButtonState = previousButtonState;

    if (currentButtonState != ButtonState::OFF) {
        return;
    }

    if (currentElapsedTime < MIN_TIME_TO_START_PROCESSING) {
        return;
    }

    ButtonEvent previousButtonEvent = previousEvents.front();

    if (previousButtonEvent.elapsedTime >= FIVE_SECONDS) {
        buttonRequests.push_back(ButtonRequest::LONG_HOLD);
    }
    else if (previousButtonEvent.elapsedTime >= THREE_SECONDS) {
        buttonRequests.push_back(ButtonRequest::HOLD);
    }
    else if (previousButtonEvent.elapsedTime >= ONE_SECOND) {
        buttonRequests.push_back(ButtonRequest::LONG_CLICK);
    }
    else if (previousButtonEvent.elapsedTime >= HALF_SECOND) {
        buttonRequests.push_back(ButtonRequest::CLICK);
    }

    previousEvents.clear();
}

void ButtonManager::handle() {
    for (auto it = buttonRequests.begin(); it != buttonRequests.end(); ++it) {
        switch (*it)
        {
        case ButtonRequest::CLICK:
            this->handleClickRequest();
            break;
        case ButtonRequest::LONG_CLICK:
            this->handleLongClickRequest();
            break;
        case ButtonRequest::HOLD:
            this->handleHoldRequest();
            break;
        case ButtonRequest::LONG_HOLD:
            this->handleLongHoldRequest();
            break;
        default:
            break;
        }
    }

    buttonRequests.clear();
}

void ButtonManager::handleClickRequest() {
    // Functionality has not beed decided yet.
    // ¯\_(ツ)_/¯
}

void ButtonManager::handleLongClickRequest() {
    configuration->run_servo_sweep = true;
    std::cout << millis() << "\t[ButtonManager] Processed Servo Sweep request\n";
    logManager->info("[ButtonManager] Processed Servo Sweep request");
}

void ButtonManager::handleHoldRequest() {
    configuration->arm = !configuration->arm;
    if (configuration->arm) {
        std::cout << millis() << "\t[ButtonManager] System is ARMED\n";
        logManager->info("[ButtonManager] System is ARMED");
    }
    else {
        std::cout << millis() << "\t[ButtonManager] System is DISARMED\n";
        logManager->info("[ButtonManager] System is DISARMED");
    }
}
void ButtonManager::handleLongHoldRequest() {
    configuration->shutdown = true;
    std::cout << millis() << "\t[ButtonManager] Processed Shutdown request\n";
    logManager->info("[ButtonManager] Processed Shutdown request");
}

bool ButtonManager::start() {
    if (configuration == NULL) {
        std::cout << millis() << "\t[ButtonManager] configuration was not specified ❌\n";
        logManager->error("[ButtonManager] configuration was not specified ❌");
        return false;
    }

    if (logManager == NULL) {
        std::cout << millis() << "\t[ButtonManager] logManager was not specified ❌\n";
        logManager->error("[ButtonManager] logManager was not specified ❌");
        return false;
    }

    std::cout << millis() << "\t[ButtonManager] Successfully started ✔️\n";
    logManager->info("[ButtonManager] Successfully started ✔️");
    return true;
}

void ButtonManager::run() {
    this->listen();
    this->process();
    this->handle();
}

void ButtonManager::stop() {
    std::cout << millis() << "\t[ButtonManager] Stopping button processing\n";
    logManager->info("[ButtonManager] Stopping button processing");

    this->configuration = NULL;
    this->logManager = NULL;
}