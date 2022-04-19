#include "AltitudeControlSystem.h"
#include "hardware/SerialLinux.h"

AltitudeControlSystem::AltitudeControlSystem(Configuration *configuration,
            std::ofstream *servoLogFile, LogManager *logManager, DataManager *dataManager) {
    this->configuration = configuration;
    this->servoLogFile = servoLogFile;
    this->logManager = logManager;
    this->dataManager = dataManager;

}

AltitudeControlSystem::AltitudeControlSystem(Configuration *configuration,
    std::ofstream *servoLogFile, LogManager *logManager, DataManager *dataManager, float targetAltitude) {
    this->configuration = configuration;
    this->servoLogFile = servoLogFile;
    this->logManager = logManager;
    this->dataManager = dataManager;
    this->targetAltitude = targetAltitude;
}

void AltitudeControlSystem::setTargetAltitude(float targetAltitude) {
    this->targetAltitude = targetAltitude;
}

int AltitudeControlSystem::estimateAngle(float projectedAltitude) {
    float deltaAlt = projectedAltitude - targetAltitude;

    if (deltaAlt > 15.0) {
        return 40;
    }
    else if (deltaAlt > 10.0 && deltaAlt <= 15.0) {
        return 30;
    }
    else if (deltaAlt < 5.0 && deltaAlt <= 10.0) {
        return 20;
    }
    else if (deltaAlt > 3.0 && deltaAlt <= 5.0) {
        return 10;
    }
    else {
        return 0;
    }
}

void AltitudeControlSystem::run() {
    this->controlAltitude();
    this->checkAndHandleServoSweep();
}

bool AltitudeControlSystem::start() {
    if (configuration == NULL) {
        std::cout << millis() << "\t[AltitudeControlSystem] Error! configuration is not specified ❌\n";
        logManager->error("[AltitudeControlSystem] Error! configuration is not specified");
        return false;
    }

    if (servoLogFile == NULL) {
        std::cout << millis() << "\t[AltitudeControlSystem] Error! servoLogFile is not specified ❌\n";
        logManager->error("[AltitudeControlSystem] Error! servoLogFile is not specified");
        return false;
    }

    if (logManager == NULL) {
        std::cout << millis() << "\t[AltitudeControlSystem] Error! logManager is not specified ❌\n";
        logManager->error("[AltitudeControlSystem] Error! logManager is not specified");
        return false;
    }

    if (dataManager == NULL) {
        std::cout << millis() << "\t[AltitudeControlSystem] Error! dataManager is not specified ❌\n";
        logManager->error("[AltitudeControlSystem] Error! dataManager is not specified");
        return false;
    }

    std::cout << millis() << "\t[AltitudeControlSystem] Successfully started ✔️\n";
    logManager->info("[AltitudeControlSystem] Successfully started ✔️");

    return true;
}

void AltitudeControlSystem::stop() {
    this->servoLogFile->close();

    std::cout << millis() << "\t[AltitudeControlSystem] Servo log file is closed\n";
    logManager->info("[AltitudeControlSystem] Servo log file is closed");

    this->configuration = NULL;
    this->servoLogFile = NULL;
    this->logManager = NULL;
    this->dataManager = NULL;
}

void AltitudeControlSystem::controlAltitude() {
    static int previousAngle = 0;
    int angle = 0;
    
    // Only control fins altitude when system is armed
    if (!configuration->arm) {
        return;
    }

    // Only control fins during burnt out state
    if (dataManager->getCurrentFlightState() != FlightState::BURNT_OUT) {
        SetAngle(0);
        return;
    }

    // Servo delayed control
    if (timeout > millis()) {
        int timeLeft = timeout - millis();
        *servoLogFile << "[AltitudeControlSystem] Servo timeout for ";
        *servoLogFile << timeLeft << "ms\n";
        return;
    }

    // reset timeout
    timeout = millis() + servoControlDelay;

    angle = estimateAngle(dataManager->getProjectedAltitude());

    if (previousAngle != angle) {
        SetAngle(angle);

        std::string msg = std::to_string(millis()) + "\t[AltitudeControlSystem] Setting angle " + std::to_string(angle) + "\n";
        *servoLogFile << msg;
        servoLogFile->flush();
    }

    previousAngle = angle;
}

void AltitudeControlSystem::checkAndHandleServoSweep() {
    if (!configuration->run_servo_sweep) {
        return;
    }
    configuration->run_servo_sweep = false;

    if (configuration->arm) {
        std::cout << millis() << "\t[AltitudeControlSystem] Can not run servo sweep, system should be disarmed!\n";
        logManager->info("[AltitudeControlSystem] Can not run servo sweep, system should be disarmed!");
        Serial.println("[AltitudeControlSystem] Can not run servo sweep, system should be disarmed!");
        return;
    }

    std::cout << millis() << "\t[AltitudeControlSystem] Running Servo Sweep command\n";
    logManager->info("[AltitudeControlSystem] Running Servo Sweep command");
    Serial.println("[AltitudeControlSystem] Running Servo Sweep command");

    servoSweep();
}