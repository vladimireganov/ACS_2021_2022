/*
 * To complie this code use for mac:
 * g++ -std=c++11 flight_state_test.cpp -o flight_state_test.o
 * To complie this code use for windows:
 * Use g++ flight_state_test.cpp -o flight_state_test.exe
*/

// Please make sure pathes are correct
#include "../../AprilLaunchAnalysis/FlightStates.h"
#include "data_parser.h"

void test_flight_state(map<string, string> &data) {
    static FlightState mystate = FlightState::PRE_LAUNCH;
    static RocketFlightStates rocket_flight_state = RocketFlightStates();

    int data_number = stoi(data["iterator"]);
    float current_time = stof(data["time"]);
    float vertical_velocity = stof(data["vertical_velocity"]);
    float vertical_acceleration = stof(data["vertical_acceleration"]);
    float current_altitude = stof(data["relative_altitude"]);
    float net_acceleration = stof(data["net_acceleration"]);
    static double max_altitude; // = stod(data["Apogee"]);

    static unsigned long apogee_fixed_time = 0;
    if(max_altitude < current_altitude) {
        max_altitude = current_altitude;
        apogee_fixed_time = current_time;
    }

    // cout << "Current altitude: " << current_altitude << endl;
    
    // double linear_acceleration_x = stod(data["Lin_Acc_x"]);
    // double linear_acceleration_y = stod(data["Lin_Acc_y"]);
    // double linear_acceleration_z = stod(data["Lin_Acc_z"]);

    static double land_altitude;
    static unsigned long land_time;
    static bool run_once = true;

    if(mystate == FlightState::PRE_LAUNCH) {
        if(rocket_flight_state.if_launched_by_altitude(current_altitude, net_acceleration, current_time)) {
            mystate = FlightState::LAUNCHED;

            cout << "Time: " << current_time << endl;
            cout << "Launched at data number: " << data_number << endl;
            cout << "Current altitude: " << current_altitude << endl;
            cout << "Net acceleration: " << net_acceleration << endl;
            cout << "Vertical velocity: " << vertical_velocity << endl;
            cout << endl;
        }
    }
    else if(mystate == FlightState::LAUNCHED) {
        if(rocket_flight_state.if_burnt_out(current_altitude, net_acceleration, current_time)) {
            mystate = BURNT_OUT;
            apogee_fixed_time = current_time;
            max_altitude = current_altitude;

            cout << "Time: " << current_time << endl;
            cout << "Burnt out at data number: " << data_number << endl;
            cout << "Current altitude: " << current_altitude << endl;
            cout << "Net acceleration: " << net_acceleration << endl;
            cout << "Vertical velocity: " << vertical_velocity << endl;
            cout << endl;
        }
    }
    else if(mystate == FlightState::BURNT_OUT) {
        if(rocket_flight_state.if_reached_apogee(current_altitude, current_time)) {
            mystate = LANDING;
            land_altitude = current_altitude;
            land_time = current_time;

            cout << "Time: " << current_time << endl;
            cout << "Reached apogee at data number: " << data_number << endl;
            cout << "Max altitude: " << max_altitude << endl;
            cout << "Current altitude: " << current_altitude << endl;
            cout << endl;
        }
    }
    else if(mystate == FlightState::LANDING) {
        if(rocket_flight_state.if_landed(current_altitude, net_acceleration, current_time)) {
            mystate = FlightState::LANDED;

            cout << "Time: " << current_time << endl;
            cout << "Landed at data number: " << data_number << endl;
            cout << "Current altitude: " << current_altitude << endl;
            cout << "Land altitude: " << land_altitude << endl;
            cout << "Net acceleration: " << net_acceleration << endl;
            cout << endl;
        }
    }
    else if(mystate == FlightState::LANDED) {
        if (run_once) {
            printf("Done!\n");
            run_once = false;
        }
    }
}

int main(int argc, char *argv[]) {

    FullscaleDataParser FSDP = FullscaleDataParser(); // Data parsing and processing class
    map<string, string> data; // data instance stored here

    /**
     * @brief Open desired data file
     * fullscale_v1_data.txt // Teensy data for Fullscale 2021 V1 Launch
     * fullscale_v2_data.txt // Teensy data for Fullscale 2021 V2 Launch
     * fv2_teensy_vel_filtered_full.csv // Teensy data for Fullscale 2021 V2 Launch full and same altitude filtered
     * fv2_teensy_vel_filtered.csv // Teensy data for Fullscale 2021 V2 Launch full and same altitude range filtered
     */
    ifstream myfile("april_launch.csv");

    if (FSDP.start(&myfile)) {
        while (FSDP.get(&myfile, data))
        {
            // FSDP.print_data_keys();
            // FSDP.print_data_values();
            test_flight_state(data);
        }
    }
    return 0;
}