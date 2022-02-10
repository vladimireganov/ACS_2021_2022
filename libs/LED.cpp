#include "LED.h"


void buzzOn() {
    gpioSetMode(BUZZ_PIN,1);    //set pin to output 3.3V
}

void buzzOff() {
    gpioSetMode(BUZZ_PIN,0);    //set pin to output 0 V
}