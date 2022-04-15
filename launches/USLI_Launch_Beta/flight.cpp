#include "flight.h"

#define DATA_FILENAME "data.csv"
#define LOG_FILENAME "log.txt"

int main() {
    /* Initialization & Setup */
    // Order is very importand do not change

    ///////////////////////////////////
    // Bussines Logic Initialization //
    //////////////////////////////////

    FileManager fileManager = FileManager();
    if (!fileManager.start()) return 1;

    auto logFile = fileManager.createFile(LOG_FILENAME);
    LogManager logManager = LogManager(logFile);

    auto dataFile = fileManager.createFile(DATA_FILENAME);
    DataManager dataManager = DataManager(dataFile);
    dataManager.setOverrideAltitude(true); // use altitude value from the sensor

    /////////////////////////////
    // Hardware Initialization //
    ////////////////////////////

    HardwareManager hardwareManager = HardwareManager(&logManager, &dataManager);

    /* Setup */
    if (!logManager.start()) return 1;
    if (!dataManager.start()) return 1;
    if (!hardwareManager.start()) return 1;

    /* Run */
    do {
        hardwareManager.run();
        dataManager.run();
    } while (true); // later will change it...
    

    /* Stop & Cleanup */
    logManager.stop();
    dataManager.stop();
    hardwareManager.stop();
}