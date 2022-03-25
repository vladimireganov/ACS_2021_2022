

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
    cout << "BMX160_1 Initialized and Configured.\n";
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
    cout << "BMX160_2 Initialized and Configured.\n";


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
    cout << "MS5607_1 Initialized and Configured.\n";

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
    cout << "MS5607_2 Initialized and Configured.\n";
    /////


    ///////////////////////////////////////////////////////////////////////
    /////////               FLIGHT CODE HERE                ///////////////
    ///////////////////////////////////////////////////////////////////////


    ///////         wait for 3 second button press to enable system     //////
    cout << "Press button for 3 seconds to enter standy.\n";
    Serial.println("Press button for 3 seconds to enter standy.");
    int pressed = 0;
    while(!pressed) {
        pressed = press(START_TIME);
    }
    pressed = 0;


    /////           wait for system arm command         /////

    cout << "System in standby. Awaiting commend \"Rocket\"\n";
    Serial.println("System in standy. Awaiting commend \"Rocket\"");

    std::string command = "";
    while ((command.compare("Rocket")) != 0) {
        command = "";
        command = Serial.readString();
        if(command.compare("Servo Sweep") == 0) {
            cout << "Running servo sweep!\n";
            Serial.println("Running servo sweep!");
            servoSweep();
        }
    }
    Serial.println("Armed! Awaiting launch. ");
    cout << "Armed! Awaiting launch. \n";
    usleep(500000);


    /////           wait until launch is detected                       /////
    long altDiff = 0;

    do {
        ms5607_1.readDigitalValue();
        ms5607_1.getAltitude();
    }while(!press(1));
    sleep(2);

    Serial.println("Launched! Awaiting burnout for deployment.");
    cout << "Launched! Awaiting burnout for deployment.\n";

    /////           wait until burnout is detected                      /////

    do {
        bmx160_1.getAllData(&Omagn,&Omagn,&Oaccel);
    } while(!press(1));
    sleep(2);

    //gauge when acceleration becomes negative again
    Serial.println("Burnout Detected! Deploying System!");
    cout << "Burnout Detected! Deploying System!\n";

    while(!press(1));
    sleep(2);

    //when velocity becomes negative
    Serial.println("System in free fall.");
    cout << "System in free fall.\n";

    while(!press(1));
    sleep(2);

    /////           run until button is pressed after recovery          /////


    cout << "System landed. Press and Hold Button for 5 seconds to end program.\n";
    Serial.println("System landed. Press and Hold Button for 5 seconds to end program.");
    while (!pressed) {
        pressed = press(END_TIME);
    }
    pressed = 0;
    cout << "System shutting down.\n";
    Serial.println("System shutting down.");
    //////////////////  closing everything   /////////////////////////

    file.close_files();
    Serial.end();
    gpioTerminate();
    close(RPI_I2C_BUS);
    return 0;
}
