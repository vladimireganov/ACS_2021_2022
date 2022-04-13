#include "flight.h"

#include "FileManager.h"
#include "LogManager.h"
#include "DataManager.h"

// #include "hardware/MS5607.h"
// #include "hardware/DFRobot_BMX160.h"
// #include "hardware/PWM.h"
// #include "hardware/SerialLinux.h"
// #include "hardware/LED.h"


#define DATA_FILENAME "data.csv"
#define LOG_FILENAME "log.txt"

int main() {
    /* Initialization & Setup */
    // Order is very importand do not change

    /////////////////////////////
    // Hardware Initialization //
    ////////////////////////////

    /* hardware initialization will go here */

    ///////////////////////////////////
    // Bussines Logic Initialization //
    //////////////////////////////////

    FileManager fileManager = FileManager();

    auto logFile = fileManager.createFile(DATA_FILENAME);
    LogManager logManager = LogManager(logFile);

    auto dataFile = fileManager.createFile(DATA_FILENAME);
    DataManager dataManager = DataManager(dataFile);

    /* Setup */
    if (!fileManager.start()) return 1;
    if (!logManager.start()) return 1;
    if (!dataManager.start()) return 1;

    /* Run */
    do {
        dataManager.run();
    } while (true); // later will change it...
    

    /* Stop & Cleanup */
    logManager.stop();
    dataManager.stop();
}