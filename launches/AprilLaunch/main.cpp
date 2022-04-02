

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "ArduinoTimer.h"
#include "File_write.h"
#include "Flight_States.h"
#include "AltitudeControlSystem.h"
#include "DFRobot_BMX160.h"
#include "MS5607.h"
#include "PWM.h"
#include "SerialLinux.h"
#include "LED.h"

using namespace std;

/* Globals */
bool armed = false;
bool shutdown = false;

Data flight_data = Data(); // creating class to store data
File_write file; // data logging
bmx160SensorData Omagn, Ogyro, Oaccel;


void process_flight_state_changes() {
    // must call do not remove!
    rocket_flight_state.process_next_state(
        flight_data.relative_altitude,
        flight_data.net_acceleration,
        millis()
    );

    if (!rocket_flight_state.if_state_changed()) {
        // exit if rocket is flight state did not change
        return;
    }

    if (rocket_flight_state.current_state == FlightState::LAUNCHED) {
        // do some actions when rocket is launched
        Serial.println("Launched! Awaiting burnout for deployment.");
        cout << "Launched! Awaiting burnout for deployment.\n";
        file.log_info("Launched");
    }
    else if (rocket_flight_state.current_state == FlightState::BURNT_OUT) {
        // do some actions when motor is burnt out
        Serial.println("Burnout Detected! Deploying System!");
        cout << "Burnout Detected! Deploying System!\n";
        file.log_info("Burnout");
    }
    else if (rocket_flight_state.current_state == FlightState::LANDING) {
        // do some actions when rocket reached apogee
        Serial.println("Rocket is landing.");
        cout << "Rocket is landing.\n";
        file.log_info("Apogee");
    }
    else if (rocket_flight_state.current_state == FlightState::LANDED) {
        // do some actions when rocket is landed
        Serial.println("System landed.");
        cout << "System landed.\n";
        file.log_info("Landed");
        ServoOff();
    }
}

void test_display_values() {
    cout << "Time seconds: " << seconds() << endl;
    cout << "Time millis: " << millis() << endl;
    cout << "Ground altitude: " << flight_data.ground_altitude << endl;
    cout << "Target altitude: " << acs.get_target_altitude() << endl;
    cout << "Altitude: " << flight_data.altitude << endl;
    cout << "Vertical Velocity: " << flight_data.vertical_velocity << endl;
    cout << "Relative altitude: " << flight_data.relative_altitude << endl;
    cout << "Net Acceleration: " << flight_data.net_acceleration  << endl;
    cout << "Projected Altitude: " << flight_data.projected_altitude << endl;
    cout << "Delta target altitude: " << flight_data.projected_altitude-acs.get_target_altitude() << endl;
    cout << "Current State: " << rocket_flight_state.current_state << endl << endl;
    cout << endl << endl << endl;
}

void control_altitude() {
    static int previous_angle = 0;
    
    // Only control altitude when armed
    if (!armed) {
        return;
    }

    // Only run duing burnt out
    if (rocket_flight_state.current_state != FlightState::BURNT_OUT) {
        SetAngle(0);
        return;
    }

    int angle = acs.estimate_angle(flight_data.projected_altitude);

    if (previous_angle != angle) {
        string msg = "Setting angle " + to_string(angle);
        file.log_info(msg);
        SetAngle(angle);
    } else {
        previous_angle = angle;
    }
}

void process_io_async() {
    /* Process Radio Requests */
    std::string command = "";
    command = Serial.asyncReadString();

    if (command.back() != ';') {
        // make sure packet received
        return;
    }

    if (command.compare(":War Eagle;") == 0) {
        // arm or disarm
        armed = !armed;

        if (armed) {
            Serial.println("Armed! Awaiting launch.");
            cout << "Armed! Awaiting launch.\n";
            file.log_info("Armed");
            SetAngle(0);
        } else {
            Serial.println("System is disarmed.");
            cout << "System is disarmed.\n";
            file.log_info("Disarmed");
            ServoOff();
        }
    }
    else if(command.compare(":Servo Sweep;") == 0) {
        if (!armed) {
            cout << "Running servo sweep!\n";
            Serial.println("Running servo sweep!");
            file.log_info("Running Servo sweep!");
            servoSweep();
            Serial.println("Servo sweep ended.");
        } else {
            cout << "Disarm the rocket!\n";
            Serial.println("Disarm the rocket!");
        }
    }
    else if(command.compare(":Shutdown;") == 0) {
        cout << "Shutting down the system\n";
        Serial.println("Shutting down the system");
        file.log_info("Shutting down the system");
        shutdown = true;
    }
}

int main() {
    // Setup target altitude
    // 1005.84 m = 3300 ft
    acs.set_target_altitude(1005.84f);

    ///////////////     Data Manager        //////////////
    // section for init
    
    flight_data.override_altitude_calculation(true);

    ///////////////     File Creation       /////////////
    
    file.connect_data(flight_data);
    file.create_log_file();
    if(!file.open_data_file()) cout << "File writing is not working.\n";
    file.create_table_names();

    //////////          SERVO, BUZZER/LED INIT AND BMP388 DISABLE  /////////////

    if (gpioInitialise() < 0) {
        cout << "GPIO Init Fail\n";
        while (1);
    }
    disBMP();

    /////////////       I2C Bus Startup     /////////////

    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {

        return 0;
    }

    ///////////         SERIAL INITIALIZATION       //////////////

    Serial.begin(9600, SERIAL_8N1);


    //////////////      BMX160 Initialization      /////////////////

    DFRobot_BMX160 bmx160_1(RPI_I2C_BUS, 0x68);

    if (bmx160_1.begin() == false) { //if begin == false
        cout << "68 init false\n";
        Serial.println("68 init false");
        while (1);
    }
    bmx160_1.wakeUp();
    bmx160_1.setAccelRange(eAccelRange_16G);
    bmx160_1.setAccelODR(eAccelODR_100Hz);
    bmx160_1.setGyroRange(eGyroRange_250DPS);
    bmx160_1.setGyroODR(eGyroODR_100Hz);
    bmx160_1.getAllData(&Omagn, &Ogyro, &Oaccel);
    cout << "BMX160_1 Initialized and Configured.\n";
    Serial.println("BMX160_1 Initialized and Configured.");
    file.log_info("BMX160_1 Initialized and Configured.");



    //////////////      MS5607 Initialization       /////////////////
    MS5607 ms5607_1(RPI_I2C_BUS, 0x76);
    if (ms5607_1.begin() == false) {
        cout << "76 init false\n";
        Serial.println("76 init false");
        while (1);
    }
    ms5607_1.setOSR(4096);  //set the oversampling ratio to maximum for snoothness in vertical velocity

    float P_val, T_val, H_val;
    if (ms5607_1.readDigitalValue()) {
        T_val = ms5607_1.getTemperature();
        P_val = ms5607_1.getPressure();
        H_val = ms5607_1.getAltitude();    //getAltitude() has calls to getTemp() and getPres() to calculate
    } else {                                          // and return the Altitude. See MS5607.cpp for more.
        cout << "Error in reading digital value in sensor! \n";
        Serial.println("Error in reading digital value in sensor!");
        while (1);
    }

    Serial.println("MS5607_1 Initialized and Configured.");
    cout << "MS5607_1 Initialized and Configured.\n";
    file.log_info("MS5607_1 Initialized and Configured.");

    /////

    file.log_info("All systems nominal.");

    ///////////////////////////////////////////////////////////////////////
    /////////               FLIGHT CODE HERE                ///////////////
    ///////////////////////////////////////////////////////////////////////


    ///////         wait for 3 second button press to enable system     //////
    cout << "Waiting\n";
    Serial.println("Waiting");
    int timeout = millis() + 3000;
    while(millis() < timeout) {}

    // confirming sound
    buzzOn();
    sleep(3);
    buzzOff();

    cout << "Data logging started.\n";
    Serial.println("Data logging started.");

    cout << "v. 1.0.2\n";
     Serial.println("v. 1.0.2");

    do {

        // Collect data
        ms5607_1.readDigitalValue();
        bmx160_1.getAllData(&Omagn,&Ogyro,&Oaccel);

        // Please do not updates below here
        // This code will be later refactored

        // Pass raw data to Data
        flight_data.set_altimeter_data(ms5607_1.getPressure(), ms5607_1.getTemperature());
        flight_data.set_acceleration_data(Oaccel.x, Oaccel.y, Oaccel.z);
        flight_data.set_gyroscope_data(Ogyro.x, Ogyro.y, Ogyro.z);
        flight_data.set_magnetometer_data(Omagn.x, Omagn.y, Omagn.z);
        flight_data.altitude = ms5607_1.getAltitude();

        // Data processing
        flight_data.process_data();

        // Log the data
        if (!file.save_data()) cout << "File writing is not working.\n";

        process_flight_state_changes();
        control_altitude();
        process_io_async();
        test_display_values();
    } while(!shutdown);
    
    //////////////////  closing everything   /////////////////////////

    buzzOn();
    sleep(3);
    buzzOff();

    ServoOff();
    file.close_files();
    Serial.end();
    gpioTerminate();
    close(RPI_I2C_BUS);
    return 0;
}
