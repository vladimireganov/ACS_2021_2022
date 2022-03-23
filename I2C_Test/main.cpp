

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "PWM.h"
#include "LED.h"




using namespace std;




int main() {


    //////////          SERVO, BUZZER/LED INIT AND BMP388 DISABLE  /////////////

    if (gpioInitialise() < 0) {
        cout << "GPIO Init Fail\n";
        while (1);
    }
    disBMP();








            //////////////////  closing everything   /////////////////////////



            gpioTerminate();


            return 0;
        }
