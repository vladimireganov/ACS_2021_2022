#include "BuzzerManager.h"

BuzzerManager::BuzzerManager(LogManager *logManager) {
    this->logManager = logManager;
}

void BuzzerManager::addRequest(BuzzerRequest buzzerRequest) {
    buzzerRequests.push(buzzerRequest);
}

void BuzzerManager::addRequest(BuzzerState state, size_t duration) {
    buzzerRequests.push(BuzzerRequest(state, duration));
}

void BuzzerManager::endOfSound() {
    this->addRequest(BuzzerState::PASSIVE, TASK_DELAY);
}

void BuzzerManager::addRepeatingSoundRequest(size_t duration, size_t delay, int repeats) {
    for (int i = 0; i < repeats; i++) {
        buzzerRequests.push(BuzzerRequest(BuzzerState::ACTIVE, duration));
        buzzerRequests.push(BuzzerRequest(BuzzerState::PASSIVE, delay));
    }

    endOfSound();
}

bool BuzzerManager::start() {
    if (logManager == NULL) {
        std::cout << millis() << "\t[BuzzerManager] Error! logManager was not specified ❌\n";
        logManager->info("[BuzzerManager] Error! logManager was not specified ❌");
        return false;
    }

    std::cout << millis() << "\t[BuzzerManager] Successfully started ✔️\n";
    logManager->info("[BuzzerManager] Successfully started ✔️");

    return true;
}

void BuzzerManager::run() {
    if (timeout > millis()) {
        // Active task ignore
        return;
    }

    if (buzzerRequests.empty()) {
        buzzOff(); // Disable sound
        busy = false;
        return;
    }

    // Setup new buzzer state
    busy = true;
    BuzzerRequest currentBuzzerRequest = buzzerRequests.front();

    timeout = millis() + currentBuzzerRequest.duration;
    if (currentBuzzerRequest.state == BuzzerState::ACTIVE) {
        std::cout << millis() << "\t[BuzzerManager] Received request with ACTIVE state for " << currentBuzzerRequest.duration << "ms\n";
        std::string logMsg = "[BuzzerManager] Received request with ACTIVE state for " + std::to_string(currentBuzzerRequest.duration) + "ms";
        logManager->info(logMsg);

        buzzOn();
    } else {
        std::cout << millis() << "\t[BuzzerManager] Received request with PASSIVE state for " << currentBuzzerRequest.duration << "ms\n";
        std::string logMsg = "[BuzzerManager] Received request with PASSIVE state for " + std::to_string(currentBuzzerRequest.duration) + "ms";
        logManager->info(logMsg);

        buzzOff();
    }

    buzzerRequests.pop();
}

void BuzzerManager::stop() {
    buzzOff();  // Turnoff sound

    std::cout << millis() << "\t[BuzzerManager] Stopping all buzzer jobs\n";
    logManager->info("[BuzzerManager] Stopping all buzzer jobs");

    this->logManager = NULL;
}

void BuzzerManager::sosSound() {
    // S
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 150);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 150);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 150);

    // O
    this->addRequest(BuzzerState::ACTIVE, 300);
    this->addRequest(BuzzerState::PASSIVE, 150);
    this->addRequest(BuzzerState::ACTIVE, 300);
    this->addRequest(BuzzerState::PASSIVE, 150);
    this->addRequest(BuzzerState::ACTIVE, 300);
    this->addRequest(BuzzerState::PASSIVE, 150);

    // S
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 150);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 150);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 150);

    // avoid overlapping sounds by adding small delay between them
    endOfSound();
}

void BuzzerManager::successSound() {
    this->addRequest(BuzzerState::ACTIVE, 300);
    this->addRequest(BuzzerState::PASSIVE, 200);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);

    // avoid overlapping sounds by adding small delay between them
    endOfSound();
}

void BuzzerManager::unInterruptableConfirmationSound(double duration) {
    buzzOn();
    sleep(duration);
    buzzOff();
}

void BuzzerManager::confirmationOneSound() {
    this->addRepeatingSoundRequest(150, 100, 1);
}

void BuzzerManager::confirmationTwoSound() {
    this->addRepeatingSoundRequest(150, 100, 2);
}

void BuzzerManager::confirmationThreeSound() {
    this->addRepeatingSoundRequest(150, 100, 3);
}

void BuzzerManager::confirmationFourSound() {
    this->addRepeatingSoundRequest(150, 100, 4);
}

void BuzzerManager::armedSound() {
    this->addRequest(BuzzerState::ACTIVE, 1000);
    this->addRequest(BuzzerState::PASSIVE, 200);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);

    // avoid overlapping sounds by adding small delay between them
    endOfSound();
}

void BuzzerManager::disarmedSound() {
    this->addRequest(BuzzerState::ACTIVE, 1000);
    this->addRequest(BuzzerState::PASSIVE, 200);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);

    // avoid overlapping sounds by adding small delay between them
    endOfSound();
}

void BuzzerManager::realTimeDataEnabledSound() {
    this->addRequest(BuzzerState::ACTIVE, 1000);
    this->addRequest(BuzzerState::PASSIVE, 200);
    this->addRequest(BuzzerState::ACTIVE, 1000);
    this->addRequest(BuzzerState::PASSIVE, 200);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);

    // avoid overlapping sounds by adding small delay between them
    endOfSound();
}

void BuzzerManager::realTimeDataDisabledSound() {
    this->addRequest(BuzzerState::ACTIVE, 1000);
    this->addRequest(BuzzerState::PASSIVE, 200);
    this->addRequest(BuzzerState::ACTIVE, 1000);
    this->addRequest(BuzzerState::PASSIVE, 200);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 200);
    this->addRequest(BuzzerState::PASSIVE, 100);

    // avoid overlapping sounds by adding small delay between them
    endOfSound();
}

void BuzzerManager::servoSweepSound() {
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 100);
    this->addRequest(BuzzerState::ACTIVE, 150);
    this->addRequest(BuzzerState::PASSIVE, 100);

    // avoid overlapping sounds by adding small delay between them
    endOfSound();
}