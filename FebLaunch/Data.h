// class for data storage
// currently stores raw data
// TO DO
// add raw data
// add functions for obtaining processed data

// add includes for sensors
#include <string.h>
#include <math.h>

#define PI 3.14159265
#define SEA_LEVEL_PA 101325.0
#define GRAVITY 9.80665

class Data
{
private:
    /* data */
    time_t previous_time;
    float previous_relative_altitude;

    void calculate_altitude(float* pressure, float* altitude);
    void calculate_ground_altitude(float* altitude, float* ground_altitude);
    void calculate_relative_altitude(float* altitude, float* ground_altitude, float* relative_altitude);
    void calculate_vertical_velocity(float* altitude, float* prev_altitude, float* elapsed_time_millis, float * v_vel);
    void calculate_net_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * net_accel);
    void calculate_vertical_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * vertical_acceleration);

public:
    int iterator; // number of iteration

    time_t current_time; // time

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

    // section for processed data
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
    void get_data();
    void set_altimeter_data(float pressure, float temperature);
    void set_acceleration_data(float accel_x, float accel_y, float accel_z);
    void set_gyroscope_data(float gyro_x, float gyro_y, float gyro_z);
    void set_magnetometer_data(float mag_x, float mag_y, float mag_z);
    void process_data();
};

Data::Data(/* args */)
{
}

Data::~Data()
{
}

void Data::set_altimeter_data(float pressure, float temperature){
    // reading all the data
    this->pressure = pressure * 100;// mbar to pa
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
    iterator++;
    // getting processed data
    float elapsed_time = 0.01; // problem solved

    this->calculate_altitude(&pressure, &altitude);
    this->calculate_ground_altitude(&altitude, &ground_altitude);
    this->calculate_relative_altitude(&altitude, &ground_altitude, &relative_altitude);
    this->calculate_vertical_velocity(&relative_altitude, &previous_relative_altitude, &elapsed_time, &vertical_velocity);
    this->calculate_net_acceleration(&acceleration_x, &acceleration_y, &acceleration_z, &net_acceleration);
    this->calculate_vertical_acceleration(&acceleration_x, &acceleration_y, &acceleration_z, &vertical_acceleration);

    // update previous values
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

void Data::calculate_vertical_velocity(float* altitude, float* prev_altitude, float* elapsed_time, float * v_vel) {
    float delta_alt = *altitude - *prev_altitude;
    *v_vel = (float)(delta_alt / (*elapsed_time / 1e3));
}

void Data::calculate_net_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * net_accel) {
    *net_accel = sqrt(pow(*acceleration_x,2)+ pow(*acceleration_y, 2) + pow(*acceleration_z, 2));
}

void Data::calculate_vertical_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * vertical_acceleration) {
    *vertical_acceleration = *acceleration_z;
}