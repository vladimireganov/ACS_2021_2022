#include <iostream>
#include <fstream>
#include "libs/File_write.h"

//#include <fstream>
using namespace std;

Data flight_data; // creating class to store data

int main(){
    // section for init
    // create file
    // activate sensors 
    // log everything is good, else log error and exit program

    cout << "Hello World";
    File_write file;
    file.flight_data = &flight_data;
    file.create_log_file();
    file.log_error();
    file.create_table_names();
    //section for waiting from radio command that rocket is ready for launch

    // section for main code
    // use while loop until landed
    
    
    
    
    
    // closing everything
    

    return 0;
}
