//
// Created by jtc19 on 2/3/2022.
//

#include "pi_util.h"


int init_GPIO() {

    return gpioInitialise();

}

void shift_bmx_Addr(){

    gpioSetMode(SPI_PIN, PI_OUTPUT);

    for (int i = 0; i < 2; i++) {

        gpioWrite(SPI_PIN, 1);
        gpioDelay(DELAY);
        gpioWrite(SPI_PIN, 0);
        gpioDelay(DELAY);
    }


}

int init_Bus(){

    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus

    return RPI_I2C_BUS;
}

int begin_Sensors(){

    RPI_I2C_BUS = init_Bus();

    if (RPI_I2C_BUS < 0) {

        cout << "init fail\n"
        //exit(1); //dont use
        return 0;
    }

    DFRobot_BMX160 bmx160(RPI_I2C_BUS, BMX_ADDR1);
    bmx160SensorData Omagn, Ogyro, Oaccel;

    if (bmx160.begin() == false){ //if begin == false
        cout << "bmx comm fail\n";
        //while(1);
    return 0;
    }

    MS5607 ms5607(RPI_I2C_BUS, MS_ADDR1);

    if (ms5607.begin() == false){ //if begin == false
        //Serial.println("init false");
        cout << "ms comm fail\n";
        //while(1);
    return 0;
    }
    return 1;
}