#include <iostream>

#include "HardwareManager.h"

HardwareManager::HardwareManager(LogManager *logManager, DataManager *dataManager) {
    this->logManager = logManager;
    this->dataManager = dataManager;
}

bool HardwareManager::start() {
    ///////////         SERIAL INITIALIZATION       //////////////

    Serial.begin(9600, SERIAL_8N1);
    std::cout << millis() << "\t[HardwareManager] Serial has started ✔️\n";
    logManager->info("[HardwareManager] Serial has started ✔️");

    //////////          SERVO, BUZZER/LED INIT AND BMP388 DISABLE  /////////////

    if (gpioInitialise() < 0) {
        std::cout << millis() << "\t[HardwareManager] GPIO inialization Failed ❌\n";
         logManager->error("[HardwareManager] GPIO inialization Failed ❌");
        return false;
    } else {
        std::cout << millis() << "\t[HardwareManager] GPIO inialization Successful ✔️\n";
         logManager->info("[HardwareManager] GPIO inialization Successful ✔️");
    }

    disBMP();
    std::cout << millis() << "\t[HardwareManager] BMP is disabled\n";
    logManager->info("[HardwareManager] BMP is disabled");

    /////////////       I2C Bus Startup     /////////////
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {
        std::cout << millis() << "\t[HardwareManager] Error opening I2C bus ❌\n";
        logManager->error("[HardwareManager] Error opening I2C bus ❌");
       return false;
    } else {
        std::cout << millis() << "\t[HardwareManager] I2C bus started successfully ✔️\n";
        logManager->info("[HardwareManager] I2C bus started successfully ✔️");
    }


    //////////////      BMX160 Initialization      /////////////////

    bmx160_1 = new DFRobot_BMX160(RPI_I2C_BUS, 0x68);

    if (bmx160_1->begin() == false) { //if begin == false
        std::cout << millis() << "\t[HardwareManager] IMU at 0x68 initialization has failed ❌\n";
        logManager->error("[HardwareManager] IMU at 0x68 initialization has failed ❌");
        Serial.println("68 init false");
        return false;
    } else {
        std::cout << millis() << "\t[HardwareManager] IMU at 0x68 initialization was successful ✔️\n";
        logManager->info("[HardwareManager] IMU at 0x68 initialization was successfuly ✔️");
    }

    bmx160_1->wakeUp();
    bmx160_1->setAccelRange(eAccelRange_16G);
    bmx160_1->setAccelODR(eAccelODR_100Hz);
    bmx160_1->setGyroRange(eGyroRange_250DPS);
    bmx160_1->setGyroODR(eGyroODR_100Hz);
    bmx160_1->getAllData(&Omagn, &Ogyro, &Oaccel);
    std::cout << millis() << "\t[HardwareManager] BMX160_1 Initialized and Configured. ✔️\n";
    logManager->info("[HardwareManager] BMX160_1 Initialized and Configured. ✔️");
    Serial.println("BMX160_1 Initialized and Configured.");



    //////////////      MS5607 Initialization       /////////////////
    ms5607_1 = new MS5607(RPI_I2C_BUS, 0x76);
    if (ms5607_1->begin() == false) {
        std::cout << millis() << "\t[HardwareManager] Altimeter at 0x76 initialization has failed ❌\n";
        logManager->error("[HardwareManager] Altimeter at 0x76 initialization has failed ❌");
        Serial.println("76 init false");
        return false;
    }  else {
        std::cout << millis() << "\t[HardwareManager] Altimeter at 0x76 initialization was successful ✔️\n";
        logManager->info("[HardwareManager] Altimeter at 0x76 initialization was successful ✔️");
    }

    ms5607_1->setOSR(256);  //set the oversampling ratio to maximum for snoothness in vertical velocity


    if (!ms5607_1->readDigitalValue()) {
        std::cout << millis() << "\t[HardwareManager] Error in reading values from Altimeter sensor ❌\n";
        logManager->error("[HardwareManager] Error in reading values from Altimeter sensor ❌");
        Serial.println("Error in reading values from Altimeter sensor!");
        return false;
    }

    logManager->info("[HardwareManager] MS5607_1 Initialized and Configured. ✔️");
    std::cout << millis() << "\t[HardwareManager] MS5607_1 Initialized and Configured. ✔️\n";
    Serial.println("MS5607_1 Initialized and Configured.");

    return true;
}

void HardwareManager::stop() {
    ServoOff();
    Serial.end();
    gpioTerminate();
    close(RPI_I2C_BUS);
}

void HardwareManager::run() {
    //////////////////////////////
    // Collect altimeter values //
    //////////////////////////////

    if (ms5607_1->readDigitalValue()) {
        dataManager->setAltimeterTemperature(ms5607_1->getTemperature());
        dataManager->setPressure(ms5607_1->getPressure() * 1e3); //mPa to Pa
        dataManager->setAltitude(ms5607_1->getAltitude());
    } else {
        std::cout << millis() << "\t[HardwareManager] Error in reading values from Altimeter sensor!\n";
        logManager->error("[HardwareManager] Error in reading values from Altimeter sensor!");
        Serial.println("Error in reading values from Altimeter sensor!");
    }

    /////////////////////////
    // Collect IMU values //
    ////////////////////////
    bmx160_1->getAllData(&Omagn, &Ogyro, &Oaccel);
    dataManager->setMagnetometer(Omagn.x, Omagn.y, Omagn.z);
    dataManager->setGyroscope(Ogyro.x, Ogyro.y, Ogyro.z);
    dataManager->setAcceleration(Oaccel.x, Oaccel.y, Oaccel.z);

    /////////////////////////
    // Set time //
    ////////////////////////
    dataManager->setTime(micros() / 1000000.0); // to seconds
}