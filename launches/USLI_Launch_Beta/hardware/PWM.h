#ifndef _PMW_H_
#define _PMW_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pigpio.h>

 
#define PWM_PIN 18
#define dCycle0 135000		//13.5% is zero point 0degrees, 80% is max, 270degrees
#define freq 333		//135 355 575 795

void ServoOff();
void SetAngle(int angle);
void servoSweep();

#endif