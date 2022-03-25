#include "LED.h"
#include <unistd.h>

int hold = 0;
double timeSpan= 0;
timespec start;
timespec end;

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


int readButton() {
    if (gpioGetMode(BUTTON_PIN) != PI_INPUT) {
        gpioSetMode(BUTTON_PIN, PI_INPUT);
    }

    return gpioRead(BUTTON_PIN);

}

int press(long dur) {
    do {
        clock_gettime(CLOCK_REALTIME,&start);
        while(readButton()) {
            clock_gettime(CLOCK_REALTIME,&end);
            timeSpan= ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9);
            if (timeSpan>= dur) {
                buzzOn();
                usleep(500000);
                buzzOff();
                break;
            }
            hold = 1;
        }
    } while (!hold);
    std::cout << "Held for " << timeSpan << " seconds \n";
    hold = 0;
    if (timeSpan>= dur) {
        timeSpan= 0;
        return 1;
    }
    timeSpan= 0;
    return 0;
}

void disBMP() {
    if (gpioGetMode(BMP_PIN) != PI_OUTPUT) {
        gpioSetMode(BMP_PIN, PI_OUTPUT);
    }
    gpioSetPullUpDown(BMP_PIN,GPIO_DOWN);       //set pin 13 (GPIO 27) to pull down
}
