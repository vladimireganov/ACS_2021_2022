/**
 * @file flight.cpp
 * @author Kazybek Mizam (kzm0099@auburn.com)
 * @brief Main file for the USLI 2022 Flight.
 * @version 0.1
 * @date 2022-04-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "flight.h"

#define DATA_FILENAME "data.csv"
#define LOG_FILENAME "log.txt"
#define SERVO_LOG_FILENAME "servo_log.txt"

int main() {
    /* Initialization & Setup */
    // Order is very importand do not change

    /////////////////////////
    // Data Initialization //
    /////////////////////////

    Configuration configuration = Configuration();

    ///////////////////////////////////
    // Bussines Logic Initialization //
    ///////////////////////////////////

    FileManager fileManager = FileManager();
    if (!fileManager.start()) return 1;

    auto logFile = fileManager.createFile(LOG_FILENAME);
    LogManager logManager = LogManager(logFile);

    auto dataFile = fileManager.createFile(DATA_FILENAME);
    DataManager dataManager = DataManager(dataFile, &logManager);
    dataManager.setOverrideAltitude(true); // use altitude value from the sensor

    /////////////////////////////
    // Hardware Initialization //
    /////////////////////////////

    HardwareManager hardwareManager = HardwareManager(&logManager, &dataManager);

    BuzzerManager buzzerManager = BuzzerManager(&logManager);

    RealTimeManager realTimeManager = RealTimeManager(&logManager, &dataManager, &configuration, &buzzerManager);

    RadioManager radioManager = RadioManager(&configuration, &logManager, &buzzerManager);

    auto servoLogFile = fileManager.createFile(SERVO_LOG_FILENAME);
    AltitudeControlSystem altitudeControlSystem = AltitudeControlSystem(&configuration, servoLogFile, &logManager, &dataManager, &buzzerManager);

    ButtonManager buttonManager = ButtonManager(&configuration, &logManager, &buzzerManager);

    /* Setup */
    if (!logManager.start()) return 1;
    if (!dataManager.start()) return 1;
    if (!hardwareManager.start()) return 1;
    if (!radioManager.start()) return 1;
    if (!altitudeControlSystem.start()) return 1;
    if (!realTimeManager.start()) return 1;
    if (!buttonManager.start()) return 1;
    if (!buzzerManager.start()) return 1;

    std::cout << millis() << "\t[Flight] All systems are nominal." << std::endl;
    logManager.info("[Flight] All systems are nominal");
    Serial.println(":[Flight] All systems are nominal.;");
    buzzerManager.successSound();

    /* Run */
    do {
        hardwareManager.run();
        dataManager.run();
        radioManager.run();
        altitudeControlSystem.run();
        realTimeManager.run();
        buttonManager.run();
        buzzerManager.run();
    } while (!configuration.shutdown);
    
    std::cout << millis() << "\t[Flight] Shutting down system." << std::endl;
    logManager.info("[Flight] Shutting down system.");
    Serial.println(":[Flight] Shutting down system.;");
    buzzerManager.unInterruptableConfirmationSound();

    /* Stop & Cleanup */
    logManager.stop();
    dataManager.stop();
    radioManager.stop();
    altitudeControlSystem.stop();
    realTimeManager.stop();
    buttonManager.stop();
    buzzerManager.stop();
    hardwareManager.stop();
}