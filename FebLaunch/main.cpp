

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "File_write.h"
#include "DFRobot_BMX160.h"
#include "MS5607.h"




using namespace std;

Data flight_data; // creating class to store data




int main(){
    // section for init

    ///////////////     File Creation       /////////////
    File_write file;
    file.connect_data(flight_data);
    file.create_log_file();
    file.create_table_names();


    /////////////       I2C Bus Startup     /////////////

    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {

        return 0;
    }


    //////////////      BMX160 Initialization      /////////////////

    DFRobot_BMX160 bmx160_1(RPI_I2C_BUS, 0x68);
    bmx160SensorData Omagn, Ogyro, Oaccel;
    // create file
    // activate sensors 
    // log everything is good, else log error and exit program

    if (bmx160_1.begin() == false){ //if begin == false

        cout << "68 init false\n";

    }
    bmx160_1.wakeUp();
    bmx160_1.setAccelRange(eAccelRange_16G);
    bmx160_1.setGyroRange(eGyroRange_1000DPS);

    // call for data with
    bmx160_1.getAllData(&Omagn, &Ogyro, &Oaccel);

    //then extract coordinates with
    int magnX = Omagn.x;
    int magnY = Omagn.y;
    //etc....


    //////////////      MS5607 Initialization       /////////////////
    MS5607 ms5607_1(RPI_I2C_BUS, 0x76);
    if (ms5607_1.begin() == false){ //if begin == false
        //Serial.println("init false");
        cout << "76 init false\n";
        while(1);

    }
    ms5607_1.setOSR(256)            //set the oversampling ratio to minimum for device


    //handle data as below
    float P_val,T_val,H_val;

    if(ms5607_1.readDigitalValue()){
        T_val = ms5607_1.getTemperature();
        P_val = ms5607_1.getPressure();
        H_val = ms5607_1.getAltitude();    //getAltitude() has calls to getTemp() and getPres() to calculate
    }else{                                          // and return the Altitude. See MS5607.cpp for more.
        cout << "Error in reading digital value in sensor! \n";
    }



    ///////////////////////////////////////////////////////////////////////
    /////////               FLIGHT CODE HERE                ///////////////
    ///////////////////////////////////////////////////////////////////////








    //////////////////  closing everything   /////////////////////////

    file.close_files();

    return 0;
}
