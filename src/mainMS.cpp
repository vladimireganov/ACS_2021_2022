

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "MS5607.h"




using namespace std;






int main(){
    // section for init

    float P_val,T_val,H_val;


    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {

        //exit(1); //dont use
        return 0;
    }

    
    MS5607 ms5607(RPI_I2C_BUS, 0x76);

    // create file
    // activate sensors 
    // log everything is good, else log error and exit program

    //Serial.begin(115200);
    //delay(100);
    if (ms5607.begin() == false){ //if begin == false
        //Serial.println("init false");
        cout << "init false\n";
        while(1);

    }

    if(ms5607.readDigitalValue()){
        T_val = ms5607.getTemperature();
        P_val = ms5607.getPressure();
        H_val = ms5607.getAltitude();
    }else{
        cout << "Error in reading digital value in sensor! \n";
    }

    cout << "Temperature :  ";
    cout << T_val;
    cout << " C"; cout << "\n";
    cout << "Pressure    :  ";
    cout << P_val;
    cout << " mBar"; cout << "\n";
    cout << "Altitude    :  ";
    cout << H_val;
    cout << " meter"; cout << "\n";

    //delay(1000);




    //section for waiting from radio command that rocket is ready for launch

    // section for main code
    // use while loop until landed





    // closing everything


    return 0;
}
