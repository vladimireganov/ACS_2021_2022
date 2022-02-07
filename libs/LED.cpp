#include "LED.h"


void buzzOn() {
    gpioSetMode(gpio,1);    //set pin to output 3.3V
}

void buzzOff() {
    gpioSetMode(gpio,0);    //set pin to output 0 V
}