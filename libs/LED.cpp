#include "LED.h"


void buzzOn() {
    if (gpioGetMode(BUZZ_PIN) != PI_OUTPUT) {
        gpioSetMode(BUZZ_PIN, PI_OUTPUT);
    }
    gpioWrite(BUZZ_PIN, pi_HI); //set pin to output 3.3V
}

void buzzOff() {
    if (gpioGetMode(BUZZ_PIN) != PI_OUTPUT) {
        gpioSetMode(BUZZ_PIN, PI_OUTPUT);
    }
    gpioWrite(BUZZ_PIN,pi_LOW);    //set pin to output 0 V
}