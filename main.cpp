

#include <iostream>
#include <fstream>
#include "libs/File_write.h"

#include "PiDuino_Library/Core.h"
#include "SPI.h"
#include "PiDuino_Library/Arduinoh/Arduino.h"
#include "PiDuino_Library/DFRobot_BMX160.h"
#include "PiDuino_Library/MS5607.h"
//#include <fstream>
using namespace std;

Data flight_data; // creating class to store data

int main(){
    // section for init
    File_write file;
    file.connect_data(flight_data);
    file.create_log_file();
    file.create_table_names();
    // create file
    // activate sensors 
    // log everything is good, else log error and exit program

    cout << "Hello World";// debug
    
    
    
    //section for waiting from radio command that rocket is ready for launch

    // section for main code
    // use while loop until landed
    
    
    
    
    
    // closing everything
    file.close_files();

    return 0;
}
