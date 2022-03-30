// class for data storage
// currently stores raw data
// TO DO
// add raw data
// add functions for obtaining processed data

// add includes for sensors
#include <string.h>
#include <math.h>
#include <chrono>

#include "QuaternionFilter.h"

#define PI 3.14159265
#define SEA_LEVEL_PA 101325.0
#define GRAVITY 9.80665

class Data
{
private:
    /* data */
    float previous_relative_altitude;
    double previous_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_chrono_time;

    // Use own or external altitude calculation
    bool override_altitude = false;

    void calculate_altitude(float* pressure, float* altitude);
    void calculate_ground_altitude(float* altitude, float* ground_altitude);
    void calculate_relative_altitude(float* altitude, float* ground_altitude, float* relative_altitude);
    void calculate_vertical_velocity(float* altitude, float* prev_altitude, double* elapsed_time, float * v_vel);
    void calculate_net_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * net_accel);
    void calculate_vertical_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * vertical_acceleration);

public:
    int iterator = 0; // number of processed iteration

    double current_time; // time in seconds since program started

    // altimeter data
    float pressure;
    float temperature;

    // imu data
    float acceleration_x;
    float acceleration_y;
    float acceleration_z;
    float gyroscope_x;
    float gyroscope_y;
    float gyroscope_z;
    float magnetometer_x;
    float magnetometer_y;
    float magnetometer_z;

    float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};  // vector to hold quaternion
    float rpy[3] {0.f, 0.f, 0.f};
    float lin_acc[3] {0.f, 0.f, 0.f};  // linear acceleration (acceleration with gravity component subtracted)
    QuaternionFilter quat_filter;
    
    size_t n_filter_iter {1};

    /* Section for processed data (derived data) */
    // altimeter derived data
    float altitude;
    float ground_altitude;
    float relative_altitude;
    float vertical_velocity;

    // imu derived data
    float vertical_acceleration;
    float net_acceleration;

    Data(/* args */);
    ~Data();
    void set_altimeter_data(float pressure, float temperature);
    void set_acceleration_data(float accel_x, float accel_y, float accel_z);
    void set_gyroscope_data(float gyro_x, float gyro_y, float gyro_z);
    void set_magnetometer_data(float mag_x, float mag_y, float mag_z);
    void process_data();
    void override_altitude_calculation(bool value);
};

Data::Data(/* args */)
{
    // Time starts when Data instance is created.
    this->start_chrono_time = std::chrono::high_resolution_clock::now();

    // Setup initial values of time
    this->current_time = 1.0;
    this->previous_time = 0.0;

    // Setup highest value so ground altitude resets.
    this->ground_altitude = 9.9 * 10e9;
}

Data::~Data()
{
}

void Data::set_altimeter_data(float pressure, float temperature){
    // reading all the data
    this->pressure = pressure * 100.0;// mbar to pa
    this->temperature = temperature;
}

void Data::set_acceleration_data(float accel_x, float accel_y, float accel_z) {
    // reading all the data
    this->acceleration_x = accel_x;
    this->acceleration_y = accel_y;
    this->acceleration_z = accel_z;
}

void Data::set_gyroscope_data(float gyro_x, float gyro_y, float gyro_z) {
    // reading all the data
    this->gyroscope_x = gyro_x;
    this->gyroscope_y = gyro_y;
    this->gyroscope_z = gyro_z;
}

void Data::set_magnetometer_data(float mag_x, float mag_y, float mag_z) {
    // reading all the data
    this->magnetometer_x = mag_x;
    this->magnetometer_y = mag_y;
    this->magnetometer_z = mag_z;
}

void Data::process_data() {
    // Update iteration
    this->iterator++;

    /* Section for timing starts here */
    // Get current time in chrono.
    auto current_chrono_time = std::chrono::high_resolution_clock::now();
    // Calculate elapsed time since beginning of the flight code.
    this->current_time = std::chrono::duration<double>(current_chrono_time - start_chrono_time).count();

    // Calculate elapsed time between processing iterations.
    double elapsed_time = current_time - previous_time;
    /* ------------------------------ */
    
    /* Section for derived data calculations starts here */

    // Calculate altitude if override_altitude flag is false.
    if (!override_altitude) {
        this->calculate_altitude(&pressure, &altitude);
    }

    this->calculate_ground_altitude(&altitude, &ground_altitude);
    this->calculate_relative_altitude(&altitude, &ground_altitude, &relative_altitude);
    this->calculate_vertical_velocity(&relative_altitude, &previous_relative_altitude, &elapsed_time, &vertical_velocity);
    this->calculate_net_acceleration(&acceleration_x, &acceleration_y, &acceleration_z, &net_acceleration);
    this->calculate_vertical_acceleration(&acceleration_x, &acceleration_y, &acceleration_z, &vertical_acceleration);
    /* ------------------------------ */

    // Update previous values.
    this->previous_time = this->current_time;
    this->previous_relative_altitude = this->relative_altitude;

}

void Data::calculate_altitude(float* pressure, float* altitude) {
    *altitude = 44330 * (1.0 - pow((*pressure / SEA_LEVEL_PA), 0.1903));
}

void Data::calculate_ground_altitude(float* altitude, float* ground_altitude) {
    if (*ground_altitude > *altitude) {
        *ground_altitude = *altitude;
    }
}

void Data::calculate_relative_altitude(float* altitude, float* ground_altitude, float* relative_altitude) {
    *relative_altitude = *altitude - *ground_altitude;
}

void Data::calculate_vertical_velocity(float* altitude, float* prev_altitude, double* elapsed_time, float * v_vel) {
    float delta_alt = *altitude - *prev_altitude;
    *v_vel = (float)(delta_alt / (*elapsed_time));
}

void Data::calculate_net_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * net_accel) {
    *net_accel = sqrt(pow(*acceleration_x,2)+ pow(*acceleration_y, 2) + pow(*acceleration_z, 2));
}

void Data::calculate_vertical_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * vertical_acceleration) {
    *vertical_acceleration = *acceleration_z;
}

void Data::override_altitude_calculation(bool value) {
    this->override_altitude = value;
}