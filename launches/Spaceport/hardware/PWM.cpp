
#include "PWM.h"

/*
gcc -Wall -pthread -o ver2 ver2.c -lpigpio
sudo ./ver2
*/
  

void ServoOff() {
	gpioHardwarePWM(PWM_PIN, freq, 135000);
}


void SetAngle(int angle) {
    /*int cyc = angle*2463+135000;      //calculate duty cycle from given angle
     * if (cyc > 245835) {     //duty cycle to maximum
     *      cyc = 245835;
     * }
     * else if (cyc < 135000) {    //duty cycle to minimum
     *      cyc = 135000;
     * }
     *
     * if (cyc == gpioGetPWMdutycycle(PWM_PIN) {    //return if duty cycle currently set
     *      return;
     * }
     * gpioHardwarePWM(PWM_PIN, freq, cyc);         //set duty cycle to pin
     * */
	if (angle <= 0) {
		gpioHardwarePWM(PWM_PIN, freq, 135000);     //sets to 0 degrees
	}
	else if (angle>= 45){
        gpioHardwarePWM(PWM_PIN, freq, 245835);     //sets to 45 degrees
    }
    else {
		int cyc=angle*2463+135000;
		gpioHardwarePWM(PWM_PIN, freq, cyc);        //sets to between 0 and 45 degrees

	}
}

void servoSweep() {
    /////       show off servo motion           /////
    SetAngle(0); usleep(500000);
    SetAngle(10); usleep(500000);
    SetAngle(20); usleep(500000);
    SetAngle(30); usleep(500000);
    SetAngle(45); usleep(500000);
    SetAngle(25); usleep(500000);
    SetAngle(10); usleep(500000);
    SetAngle(0); usleep(500000);
}

