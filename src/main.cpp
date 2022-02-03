

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "File_write.h"
#include "DFRobot_BMX160.h"




using namespace std;

Data flight_data; // creating class to store data




int main(){
    // section for init
    /////////////       SERIAL INITIALIZATION       ////////////////
    Serial.begin(9600, SERIAL_8N1);

    ///////////////     File Creation       /////////////
    File_write file;
    file.connect_data(flight_data);
    file.create_log_file();
    file.create_table_names();
    Serial.println("File created");

    //////////////      GPIO Output to force BMP388 to SPI Mode     //////////

    //whatever code here!
    Serial.println("BMP388 in SPI Mode!");

    /////////////       I2C Bus Startup     /////////////

    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {

        //exit(1); //dont use
        return 0;
    }
    Serial.println("I2C Bus Opened!");
    //some way to print visible devices on the i2c-bus? Should have 0x68, 0x69, 0x76, 0x77


    //////////////      BMX160 Initialization      /////////////////

    DFRobot_BMX160 bmx160(RPI_I2C_BUS, 0x69);
    bmx160SensorData Omagn, Ogyro, Oaccel;
    // create file
    // activate sensors 
    // log everything is good, else log error and exit program

    //Serial.begin(115200);
    //delay(100);
    if (bmx160.begin() == false){ //if begin == false
        //Serial.println("init false");
        cout << "init false\n";
        while(1);

    }
    bmx160.wakeUp();

    Serial.println("BMX160 0x69 Initialized");

    //////////////      REMOVE SOON - BMX160 TESTING        //////////////
    for (int i = 0; i < 25; i++)
    {

        bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);

  /* Display the magnetometer results (magn is magnetometer in uTesla) */
        cout << "M ";
        cout << "X: "; cout << Omagn.x << "  ";
        cout << "Y: "; cout << Omagn.y << "  ";
        cout << "Z: "; cout << Omagn.z << "  ";
        cout << "uT\n";


  /* Display the gyroscope results (gyroscope data is in g) */
        cout << "G ";
        cout << "X: "; cout << Ogyro.x << "  ";
        cout << "Y: "; cout << Ogyro.y << "  ";
        cout << "Z: "; cout << Ogyro.z << "  ";
        cout << "g\n";

  /* Display the accelerometer results (accelerometer data is in m/s^2) */
        cout << "A ";
        cout << "X: "; cout << Oaccel.x << "  ";
        cout << "Y: "; cout << Oaccel.y << "  ";
        cout << "Z: "; cout << Oaccel.z << "  ";
        cout << "m/s^2\n";

        cout << "\n";
        cout << "Completed printing all Data\n"; //output letting user know all data has finished printing
        usleep(100000);
    }

    //////////////      MS5607 Initialization       /////////////////


    Serial.println("MS5607 0x76 Initialized!");

    //section for waiting from radio command that rocket is ready for launch
    std::string wait = "";
    while (wait.compare("START COMMAND") != 0){
        wait = "";
        wait = Serial.readString();
    }
    // section for main code



    // use while loop until landed





    // closing everything
    Serial.end();
    file.close_files();

    return 0;
}
