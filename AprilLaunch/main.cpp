

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "File_write.h"
#include "DFRobot_BMX160.h"
#include "MS5607.h"
#include "PWM.h"
#include "SerialLinux.h"
#include "LED.h"




using namespace std;

Data flight_data; // creating class to store data




int main() {
    // section for init

    ///////////////     File Creation       /////////////
    File_write file;
    file.connect_data(flight_data);
    file.create_log_file();
    file.create_table_names();

    //////////          SERVO, BUZZER/LED INIT AND BMP388 DISABLE  /////////////

    if (gpioInitialise() < 0) {
        cout << "GPIO Init Fail\n";
        while (1);
    }
    disBMP();

    /////////////       I2C Bus Startup     /////////////



    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {

        return 0;
    }

    ///////////         SERIAL INITIALIZATION       //////////////

    Serial.begin(9600, SERIAL_8N1);


    //////////////      BMX160 Initialization      /////////////////

    DFRobot_BMX160 bmx160_1(RPI_I2C_BUS, 0x68);
    bmx160SensorData Omagn, Ogyro, Oaccel;

    if (bmx160_1.begin() == false) { //if begin == false
        cout << "68 init false\n";
        Serial.println("68 init false");
        while (1);
    }
    bmx160_1.wakeUp();
    bmx160_1.setAccelRange(eAccelRange_16G);
    bmx160_1.setAccelODR(eAccelODR_1600Hz);
    bmx160_1.setGyroRange(eGyroRange_1000DPS);
    bmx160_1.setGyroODR(eGyroODR_3200Hz);
    bmx160_1.getAllData(&Omagn, &Ogyro, &Oaccel);
    Serial.println("BMX160_1 Initialized and Configured.");

    DFRobot_BMX160 bmx160_2(RPI_I2C_BUS, 0x69);

    if (bmx160_2.begin() == false) { //if begin == false
        cout << "69 init false\n";
        Serial.println("69 init false");
        while (1);
    }
    bmx160_2.wakeUp();
    bmx160_2.setAccelRange(eAccelRange_16G);
    bmx160_2.setAccelODR(eAccelODR_1600Hz);
    bmx160_2.setGyroRange(eGyroRange_1000DPS);
    bmx160_2.setGyroODR(eGyroODR_3200Hz);
    bmx160_2.getAllData(&Omagn, &Ogyro, &Oaccel);
    Serial.println("BMX160_2 Initialized and Configured.");
    cout("BMX160_2 Initialized and Configured.");


    //////////////      MS5607 Initialization       /////////////////
    MS5607 ms5607_1(RPI_I2C_BUS, 0x76);
    if (ms5607_1.begin() == false) {
        cout << "76 init false\n";
        Serial.println("76 init false");
        while (1);
    }
    ms5607_1.setOSR(256);            //set the oversampling ratio to minimum for device

    float P_val, T_val, H_val;
    if (ms5607_1.readDigitalValue()) {
        T_val = ms5607_1.getTemperature();
        P_val = ms5607_1.getPressure();
        H_val = ms5607_1.getAltitude();    //getAltitude() has calls to getTemp() and getPres() to calculate
    } else {                                          // and return the Altitude. See MS5607.cpp for more.
        cout << "Error in reading digital value in sensor! \n";
        Serial.println("Error in reading digital value in sensor!");
        while (1);
    }

    Serial.println("MS5607_1 Initialized and Configured.");

    /////

    MS5607 ms5607_2(RPI_I2C_BUS, 0x77);
    if (ms5607_2.begin() == false) {
        cout << "77 init false\n";
        Serial.println("77 init false");
        while (1);
    }
    ms5607_2.setOSR(256);            //set the oversampling ratio to minimum for device

    if (ms5607_2.readDigitalValue()) {
        T_val = ms5607_2.getTemperature();
        P_val = ms5607_2.getPressure();
        H_val = ms5607_2.getAltitude();    //getAltitude() has calls to getTemp() and getPres() to calculate
    } else {                                          // and return the Altitude. See MS5607.cpp for more.
        cout << "Error in reading digital value in sensor! \n";
        Serial.println("Error in reading digital value in sensor!");
        while (1);
    }

    Serial.println("MS5607_2 Initialized and Configured.");
    cout("MS5607_2 Initialized and Configured.");
    /////


    ///////////////////////////////////////////////////////////////////////
    /////////               FLIGHT CODE HERE                ///////////////
    ///////////////////////////////////////////////////////////////////////


    ///////         wait for 3 second button press to enable system     //////
    int hold = 0;
    long startTime = 3;  //3000 ms = 3s
    long turnOffTime = 3;
    long dur = 0;
    timespec start;
    timespec end;

    Serial.println("Press and Hold Button for 3 seconds to Begin.");
    cout("Press and Hold Button for 3 seconds to Begin");
    while (!hold) {
        clock_gettime(CLOCK_REALTIME,&start);
        while(readButton()) {
            clock_gettime(CLOCK_REALTIME,&end);
            dur = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
            if (dur >= startTime) {
                buzzOn(); usleep(500000); buzzOff();
                hold = 1;
            }
            dur = 0;
        }
    }
    hold = 0;

    /////           wait for system arm command         /////

    cout("System ready for arming. Awaiting commend \"Rocket\"\n");
    Serial.println("System ready for arming. Awaiting commend \"Rocket\"");

    std::string command = "";
    while ((command.compare("Rocket")) != 0) {
        command = "";
        command = Serial.readString();
    }
    Serial.println("Armed! Actuator Sweep Starting.");
    cout("Armed! Actuator Sweep Starting.\n");
    usleep(500000);


    /////       show off servo motion           /////
    SetAngle(0); usleep(500000);
    SetAngle(10); usleep(500000);
    SetAngle(20); usleep(500000);
    SetAngle(30); usleep(500000);
    SetAngle(45); usleep(500000);
    SetAngle(25); usleep(500000);
    SetAngle(10); usleep(500000);
    SetAngle(0); usleep(500000);

    cout("Sweep Completed. System armed and ready!\n");
    Serial.println("Sweep Completed. System armed and ready!");

    /////           wait until launch is detected                       /////
    long altDiff = 0;

    do {
        ms5607_1.readDigitalValue();
        ms5607_1.getAltitude();
    }while();

    Serial.println("Launched!");
    cout << "Launched!\n";

    /////           wait until burnout is detected                      /////

    do {
        bmx160_1.getAllData(&Omagn,&Omagn,&Oaccel);
    } while();

    //gauge when acceleration becomes negative again
    Serial.println("Burnout Detected! Deploying System!");
    cout << "Burnout Detected! Deploying System!\n";

    /////           run until button is pressed after recovery          /////

    do {
        clock_gettime(CLOCK_REALTIME,&start);
        while(readButton()) {
            clock_gettime(CLOCK_REALTIME,&end);
            dur = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
            if (dur >= turnOffTime) {
                buzzOn(); usleep(500000); buzzOff();
                hold = 1;
            }
            dur = 0;
        }

        //flight code




    } while(!hold)


    //////////////////  closing everything   /////////////////////////

    file.close_files();
    Serial.end();
    gpioTerminate();
    close(RPI_I2C_BUS);
    return 0;
}
