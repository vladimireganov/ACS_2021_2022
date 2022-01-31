#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pigpio.h>

 
#define gpio 18
#define dCycle0 135000		//13.5% is zero point 0degrees, 80% is max, 270degrees
#define freq 333		//135 355 575 795
  
void InitServo(void);
void ServoOff(void);
void SetAngle(int angle);