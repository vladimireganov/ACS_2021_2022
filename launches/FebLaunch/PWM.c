
#include "PWM.h"

/*
gcc -Wall -pthread -o ver2 ver2.c -lpigpio
sudo ./ver2
*/
  

void ServoOff() {
	gpioPWM(PWM_PIN,0);
}


void SetAngle(int angle) {	
	if (angle <= 0) {
		gpioHardwarePWM(PWM_PIN, freq, 135000);
	}
	else if (angle>= 45){
        gpioHardwarePWM(PWM_PIN, freq, 245835);
    }
    else {
		int cyc=angle*2463+135000;
		gpioHardwarePWM(PWM_PIN, freq, cyc);

	}
}

