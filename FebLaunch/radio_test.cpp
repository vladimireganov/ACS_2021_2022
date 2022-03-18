#include <iostream>
#include <fstream>
#include <fcntl.h>
#include "SerialLinux.h"

int main() {


/////////               TESTING RADIO               ///////////////
    cout << "Testing Serial Port to Radio \n";
    Serial.println("Hello World!\n");
    Serial.setTimeout(1000);
    std::string test = "";
    while ((test.compare("Rocket")) != 0) {
        test = "";
        test = Serial.readString();
    }
    Serial.println("Armed!\n");
    std::cout << test << "\n";


}