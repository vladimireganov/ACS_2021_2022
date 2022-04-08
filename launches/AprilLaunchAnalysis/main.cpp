

#include <iostream>
#include <fstream>
#include <sstream>

#include "PWM.h"

using namespace std;

int main()
{
    ifstream servo_file;
    servo_file.open("servo_control.txt");

    std::string line;
    std::string delay_time_str;
    std::string angle_str;

    float delay_time = 0.0f;
    int angle = 0;

    if (gpioInitialise() < 0) {
        cout << "GPIO Init Fail\n";
        while (1);
    }

    cout << "Starting..." << line << endl;
    usleep(3*1e6);
    cout << "Setting angle 0" << line << endl;
    SetAngle(0);
    cout << "Emulation starts in 3 seconds" << line << endl;
    usleep(3*1e6);

    while(getline(servo_file, line)) {

        cout << "Current line: " << line << endl;
        istringstream iss(line);
        
        getline(iss, delay_time_str, ' ');
        getline(iss, angle_str, ' ');

        cout << "Delay time: " << delay_time_str << endl;
        cout << "Angle: " << angle_str << endl;

        delay_time = stof(delay_time_str);
        angle = stoi(angle_str);

        cout << "Angle integer: " << angle << endl;

        usleep(delay_time*1e6);
        SetAngle(angle);
        // usleep(1e6);
    }

    ServoOff();
    gpioTerminate();
    return 0;
}