#include <iostream>

#include "HardwareManager.h"

bool HardwareManager::start() {
    ///////////         SERIAL INITIALIZATION       //////////////

    Serial.begin(9600, SERIAL_8N1);
    std::cout << "Serial has started\n";

    //////////          SERVO, BUZZER/LED INIT AND BMP388 DISABLE  /////////////

    std::cout << "Starting GPIO initialization\n";
    if (gpioInitialise() < 0) {
        std::cout << "GPIO inialization Failed\n";
        return false;
    } else {
        std::cout << "GPIO inialization Successful\n";
    }

    disBMP(); std::cout << "Disabled BMP\n";

    /////////////       I2C Bus Startup     /////////////

    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {
        std::cout << "Error opening I2C bus\n";
       return false;
    } else {
        std::cout << "I2C bus started successfully\n";
    }


    //////////////      BMX160 Initialization      /////////////////

    DFRobot_BMX160 bmx160_1(RPI_I2C_BUS, 0x68);

    if (bmx160_1.begin() == false) { //if begin == false
        std::cout << "IMU at 0x68 initialization has failed\n";
        Serial.println("68 init false");
        return false;
    } else {
        std::cout << "IMU at 0x68 initialization was successful\n";
    }

    bmx160_1.wakeUp();
    bmx160_1.setAccelRange(eAccelRange_16G);
    bmx160_1.setAccelODR(eAccelODR_100Hz);
    bmx160_1.setGyroRange(eGyroRange_250DPS);
    bmx160_1.setGyroODR(eGyroODR_100Hz);
    bmx160_1.getAllData(&Omagn, &Ogyro, &Oaccel);
    cout << "BMX160_1 Initialized and Configured.\n";
    Serial.println("BMX160_1 Initialized and Configured.");
    file.log_info("BMX160_1 Initialized and Configured.");



    //////////////      MS5607 Initialization       /////////////////
    MS5607 ms5607_1(RPI_I2C_BUS, 0x76);
    if (ms5607_1.begin() == false) {
        std::cout << "Altimeter at 0x76 initialization has failed\n";
        Serial.println("76 init false");
        while (1);
    }  else {
        std::cout << "Altimeter at 0x76 initialization was successful\n";
    }

    ms5607_1.setOSR(4096);  //set the oversampling ratio to maximum for snoothness in vertical velocity

    float P_val, T_val, H_val;
    if (ms5607_1.readDigitalValue()) {
        T_val = ms5607_1.getTemperature();
        P_val = ms5607_1.getPressure();
        H_val = ms5607_1.getAltitude();    //getAltitude() has calls to getTemp() and getPres() to calculate
    } else {                                          // and return the Altitude. See MS5607.cpp for more.
        std::cout << "Error in reading values from Altimeter sensor! \n";
        Serial.println("Error in reading values from Altimeter sensor!");
        return false;
    }

    Serial.println("MS5607_1 Initialized and Configured.");
    std::cout << "MS5607_1 Initialized and Configured.\n";
    file.log_info("MS5607_1 Initialized and Configured.");

    return true;
}

void HardwareManager::stop() {
    ServoOff();
    file.close_files();
    Serial.end();
    gpioTerminate();
    close(RPI_I2C_BUS);
}