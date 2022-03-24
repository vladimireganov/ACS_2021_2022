//
// Created by Robert Nance on 1/14/22.
//
#include <string.h>
#include "SerialLinux.h"

int main() {
    Serial.begin(9600, SERIAL_8N1);
    Serial.setTimeout(1000);
    std::string test = "";
    while( (test.compare("Rocket")) != 0 ) {
        test = "";
        test = Serial.readString();
    }
    Serial.println("Armed!");
    //std::cout << test;
    Serial.println(test);
    Serial.end();
    return 0;
}

