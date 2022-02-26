

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "File_write.h"
#include "DFRobot_BMX160.h"
#include "MS5607.h"
#include "PWM.h"
#include "SerialLinux.h"
#include "LED.h"
#include "Flight_States.h"




using namespace std;

Data flight_data; // creating class to store data
struct timespec gettime_now;

int main() {
    // section for init

    ///////////////     File Creation       /////////////
    File_write file;
    file.connect_data(flight_data);
    file.create_log_file();
    file.create_table_names();

    //////////          SERVO, BUZZER/LED INIT AND BMP388 DISABLE  /////////////

    if (gpioInitialise() < 0) {
        cout << "GPIO Init Fail\n";
        while (1);
    }
    //disBMP();

    /////////////       I2C Bus Startup     /////////////



    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {

        return 0;
    }

    //////////////      BMX160 Initialization      /////////////////

    DFRobot_BMX160 bmx160_1(RPI_I2C_BUS, 0x68);
    bmx160SensorData Omagn, Ogyro, Oaccel;
    // create file
    // activate sensors 
    // log everything is good, else log error and exit program

    if (bmx160_1.begin() == false) { //if begin == false

        cout << "68 init false\n";
        while (1);

    }
    bmx160_1.wakeUp();
    bmx160_1.setAccelRange(eAccelRange_16G);
    bmx160_1.setAccelODR(eAccelODR_1600Hz);
    bmx160_1.setGyroRange(eGyroRange_1000DPS);
    bmx160_1.setGyroODR(eGyroODR_3200Hz);

    //////////////      MS5607 Initialization       /////////////////
    MS5607 ms5607_1(RPI_I2C_BUS, 0x76);
    if (ms5607_1.begin() == false) { //if begin == false
        //Serial.println("init false");
        cout << "76 init false\n";
        while (1);

    }

    //set the oversampling ratio to minimum for device
    ms5607_1.setOSR(256);


    while(true) {
        // Collect altimeter data
        if (ms5607_1.readDigitalValue()) {
            flight_data.set_altimeter_data(ms5607_1.getTemperature(), ms5607_1.getPressure());

        } else {
            cout << "Error in reading digital value in sensor! \n";
        }

        // collect imu data
        bmx160_1.getAllData(&Omagn, &Ogyro, &Oaccel);
        flight_data.set_acceleration_data((float)Oaccel.x, (float)Oaccel.y, (float)Oaccel.z);
        flight_data.set_gyroscope_data((float)Oaccel.x, (float)Oaccel.y, (float)Oaccel.z);
        flight_data.set_magnetometer_data((float)Oaccel.x, (float)Oaccel.y, (float)Oaccel.z);

        // record time
        clock_gettime(CLOCK_REALTIME, &gettime_now);
        flight_data.current_time = gettime_now.tv_nsec / 1e6;

        cout << "Mag x " << flight_data.magnetometer_x << endl;
        cout << "Mag y " << flight_data.magnetometer_y << endl;
        cout << "Mag z " << flight_data.magnetometer_z << endl;

        // process raw data to derived data
        flight_data.process_data();

        // process flight state
        rocket_flight_state.process_next_state(flight_data.relative_altitude, flight_data.vertical_acceleration, flight_data.net_acceleration, flight_data.current_time);

        // setup servo
        if (rocket_flight_state.current_state == LAUNCHED) {
            // set angle 0 when launched
            SetAngle(0);
        }
        else if (rocket_flight_state.current_state == BURNT_OUT) {
            // set angle 45 when burnt out
            SetAngle(45);
        }
        else if (rocket_flight_state.current_state == LANDING) {
            if (flight_data.relative_altitude > 100.0) {
                // set angle 0 when landing and current relative altitude higher than 100 m
                SetAngle(0);
            }
            else {
                // Disable Servo if current relative altitude less than 100 m
            }
        }
    }
}
