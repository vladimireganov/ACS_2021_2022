//
//  Written by rwn0006 on 02/07/22
//
//  Provide easy-to-call functions for LED and Buzzer output on RPi pin 16
//



#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pigpio.h>

#define gpio 23

void buzzOn();
void buzzOff();