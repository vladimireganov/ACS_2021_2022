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
#define pi_LOW 0
#define pi_HI 1

void buzzOn();
void buzzOff();