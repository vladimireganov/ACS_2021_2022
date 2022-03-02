//
//  Written by rwn0006 on 02/07/22
//
//  Provide easy-to-call functions for LED and Buzzer output on RPi pin 16
//



#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pigpio.h>

#define BUZZ_PIN 23
#define BUTTON_PIN 24
#define BMP_PIN 27
#define pi_LOW 0
#define pi_HI 1
#define GPIO_DOWN 2

void buzzOn();
void buzzOff();

int readButton();
void disBMP();