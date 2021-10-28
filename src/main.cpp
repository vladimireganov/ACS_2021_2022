

#include <iostream>
#include <fstream>
#include "File_write.h"

#include "Serial.h"
#include "Core.h"
#include "SPI.h"
#include "piDuino.h"
#include "DFRobot_BMX160.h"
#include "MS5607.h"

//#include "PiDuino_Library/DFRobot_BMX160.cpp"
//#include "PiDuino_Library/MS5607.cpp"

using namespace std;

Data flight_data; // creating class to store data


int main(){
    // section for init
    File_write file;
    file.connect_data(flight_data);
    file.create_log_file();
    file.create_table_names();
    
    DFRobot_BMX160 bmx160;
    bmx160SensorData Omagn, Ogyro, Oaccel;
    // create file
    // activate sensors 
    // log everything is good, else log error and exit program

    //Serial.begin(115200);
    //delay(100);
    if (bmx160.begin() != true){
        //Serial.println("init false");
        cout << "init false";
        while(1);
    }
    cout << "Hello World";// debug
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
    }
    
    
    
    
    //section for waiting from radio command that rocket is ready for launch

    // section for main code
    // use while loop until landed
    
    
    
    
    
    // closing everything
    file.close_files();

    return 0;
}
