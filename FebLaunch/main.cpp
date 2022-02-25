

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
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
    // create file
    // activate sensors 
    // log everything is good, else log error and exit program

    if (bmx160_1.begin() == false) { //if begin == false

        cout << "68 init false\n";
        while (1);

    }
    bmx160_1.wakeUp();
    bmx160_1.setAccelRange(eAccelRange_2G);
    bmx160_1.setGyroRange(eGyroRange_1000DPS);

    // call for data with
    bmx160_1.getAllData(&Omagn, &Ogyro, &Oaccel);

    DFRobot_BMX160 bmx160_2(RPI_I2C_BUS, 0x69);
    // create file
    // activate sensors
    // log everything is good, else log error and exit program

    if (bmx160_2.begin() == false) { //if begin == false

        cout << "69 init false\n";
        while (1);

    }
    bmx160_2.wakeUp();
    bmx160_2.setAccelRange(eAccelRange_2G);
    bmx160_2.setGyroRange(eGyroRange_1000DPS);


    //////////////      MS5607 Initialization       /////////////////
    MS5607 ms5607_1(RPI_I2C_BUS, 0x76);
    if (ms5607_1.begin() == false) { //if begin == false
        //Serial.println("init false");
        cout << "76 init false\n";
        while (1);

    }
    //ms5607_1.setOSR(256);            //set the oversampling ratio to minimum for device

    MS5607 ms5607_2(RPI_I2C_BUS, 0x77);
    if (ms5607_2.begin() == false) { //if begin == false
        //Serial.println("init false");
        cout << "77 init false\n";
        while (1);

    }
    //ms5607_2.setOSR(256);            //set the oversampling ratio to minimum for device

    //handle data as below
    float P_val, T_val, H_val;

    if (ms5607_1.readDigitalValue()) {
        T_val = ms5607_1.getTemperature();
        P_val = ms5607_1.getPressure();
        H_val = ms5607_1.getAltitude();    //getAltitude() has calls to getTemp() and getPres() to calculate
    } else {                                          // and return the Altitude. See MS5607.cpp for more.
        cout << "Error in reading digital value in sensor! \n";
        while (1);
    }



    ///////////////////////////////////////////////////////////////////////
    /////////               TEST CODE HERE                ///////////////
    ///////////////////////////////////////////////////////////////////////


    /////////               TESTING RADIO               ///////////////
    cout<<"Testing Serial Port Write to Radio. Writing \"Hello World!\" \n";
    Serial.println("Hello World!\n");
    sleep(2);
   /* cout<<"Testing Serial Port Read from Radio. Waiting until arm command \"Rocket\" is received.\n";
    Serial.setTimeout(1000);
    std::string test = "THIS Is Not THE right PAssWoRd!";
    while ((test.compare("Rocket")) != 0) {
        test = "";
        test = Serial.readString();
    }
    Serial.println("Armed!\n");
    std::cout << test << "\n";*/

    /////////           TESTING BUZZER/LED             //////////////
    cout << "BEEP!\n";
    buzzOn();
    sleep(2);
    buzzOff();

    //////////            TESTING BUTTON               /////////////

    cout << "Press button! \n";
    while (!readButton());
    cout << "BEEP!\n";
    buzzOn();
    sleep(2);
    buzzOff();


    ///////////         TESTING SERVO               //////////////

    cout << "Testing Servos!\n";

    SetAngle(0);
    cout << "0 degrees\n";
    sleep(2);
    SetAngle(10);
    cout << "10 degrees\n";
    sleep(2);
    SetAngle(20);
    cout << "20 degrees\n";
    sleep(2);
    SetAngle(30);
    cout << "30 degrees\n";
    sleep(2);
    SetAngle(45);
    cout << "45 degrees\n";
    sleep(2);
    SetAngle(-90);
    cout << "set to -90, defaults to 0 degrees\n";
    sleep(2);
    SetAngle(135);
    cout << "set to 135, defaults to 45 degrees\n";
    sleep(2);



    ////////    TESTING BMX160 Communications and Test Scheme       ////////

    cout << "\nBMX160_1 Data\n";
    for (int i = 0; i < 10; i++) {

        bmx160_1.getAllData(&Omagn, &Ogyro, &Oaccel);

        // Display the magnetometer results (magn is magnetometer in uTesla)
        cout << "M ";
        cout << "X: ";
        cout << Omagn.x << "  ";
        cout << "Y: ";
        cout << Omagn.y << "  ";
        cout << "Z: ";
        cout << Omagn.z << "  ";
        cout << "uT\n";


        // Display the gyroscope results (gyroscope data is in g)
        cout << "G ";
        cout << "X: ";
        cout << Ogyro.x << "  ";
        cout << "Y: ";
        cout << Ogyro.y << "  ";
        cout << "Z: ";
        cout << Ogyro.z << "  ";
        cout << "g\n";

        // Display the accelerometer results (accelerometer data is in m/s^2)
        cout << "A ";
        cout << "X: ";
        cout << Oaccel.x << "  ";
        cout << "Y: ";
        cout << Oaccel.y << "  ";
        cout << "Z: ";
        cout << Oaccel.z << "  ";
        cout << "m/s^2\n";

        cout << "\n";
        usleep(500000);
    }
   /* while (bmx160_1.pubScan()) {      //to be used when sensor can be physically disconnected
    }
    cout << "BMX160_1 Comm Failure!\n";*/

    bmx160_2.getAllData(&Omagn, &Ogyro, &Oaccel);
    cout << "\nBMX160_2 Data\n";
    for (int i = 0; i < 10; i++) {

        bmx160_2.getAllData(&Omagn, &Ogyro, &Oaccel);

        // Display the magnetometer results (magn is magnetometer in uTesla)
        cout << "M ";
        cout << "X: ";
        cout << Omagn.x << "  ";
        cout << "Y: ";
        cout << Omagn.y << "  ";
        cout << "Z: ";
        cout << Omagn.z << "  ";
        cout << "uT\n";

        // Display the gyroscope results (gyroscope data is in g)
        cout << "G ";
        cout << "X: ";
        cout << Ogyro.x << "  ";
        cout << "Y: ";
        cout << Ogyro.y << "  ";
        cout << "Z: ";
        cout << Ogyro.z << "  ";
        cout << "g\n";

        // Display the accelerometer results (accelerometer data is in m/s^2)
        cout << "A ";
        cout << "X: ";
        cout << Oaccel.x << "  ";
        cout << "Y: ";
        cout << Oaccel.y << "  ";
        cout << "Z: ";
        cout << Oaccel.z << "  ";
        cout << "m/s^2\n";

        cout << "\n";
        usleep(500000);
    }
    /*while (bmx160_2.pubScan()) {      // to be used when sensor can be physically disconnected
    }

            cout << "BMX160_2 Comm Failure!\n";*/


            //////////      MS5607 Comm Test and Failures       ///////////////
            cout<<"MS5607_1 Data!\n\n";
            for (int x=0; x<25; x++) {
                ms5607_1.readDigitalValue();
                cout << "MS5607_1 Temperature: " << ms5607_1.getTemperature() << "\n";
                cout << "MS5607_1 Pressure: " << ms5607_1.getPressure() << "\n";
                cout << "MS5607_1 Altitude: " << ms5607_1.getAltitude() << "\n\n";
                usleep(500000);
            }
          /*  while(!ms5607_1.readDigitalValue()) {     //to be used when sensor can be physically disconnected
            }
            cout << "MS5607_1 Comm Failure! Using MS5607_2! \n"; */
            cout<<"MS5607_2 Data!\n\n";
            for (int x=0; x<25; x++) {
                ms5607_2.readDigitalValue();
                cout << "MS5607_2 Temperature: " << ms5607_2.getTemperature() << "\n";
                cout << "MS5607_2 Pressure: " << ms5607_2.getPressure() << "\n";
                cout << "MS5607_2 Altitude: " << ms5607_2.getAltitude() << "\n\n";
                usleep(500000);
            }
        /*  while(!ms5607_2.readDigitalValue()) {     //to be used when sensor can be physically disconnected
            }
      cout << "MS5607_2 Comm Failure! \n"; */

            //////////////////  closing everything   /////////////////////////

            cout << "Closing all comm buses and terminating GPIO.\n";
            ServoOff();
            file.close_files();
            Serial.end();
            gpioTerminate();
            close(RPI_I2C_BUS);
            return 0;
}
