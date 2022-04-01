/**
 * @file Data.h
 * @author Kazybek Mizam (kzm0099@auburn.edu)
 * @brief Processing raw data to useful derived data.
 * Quaternion & Raw Pitch Yaw (RPY) estimation taken from https://github.com/hideakitai/MPU9250.
 * 
 * @version 0.1
 * @date 2022-04-01
 * 
 * @copyright Copyright (c) Auburn Rocketry Association 2022.
 * 
 */

#include <math.h>
#include <chrono>

#include "QuaternionFilter.h"
#include "flight_math.h"

#define PI 3.14159265
#define SEA_LEVEL_PA 101325.0
#define GRAVITY 9.80665

class Data
{
private:
    // Constants
    const float DEG_TO_RAD = 0.01745329f;
    const float TESLA_TO_GAUSS = 10000.0f;          // 1 T == 10,000 Gauss
    const float MS2_TO_MG = 101.972f;
    const float magnetic_declination = -4.50486;    // Auburn, AL, USA, 1st April
    
    // Previous values
    float previous_relative_altitude;

    /* Timing */
    double current_time;    // time in seconds since program started.
    double previous_time;   // time in seconds since previous data processing.
    std::chrono::time_point<std::chrono::high_resolution_clock> start_chrono_time;

    // Use own or external altitude calculation.
    bool override_altitude = false;
    
    QuaternionFilter quat_filter;   // Filter that estimates quaternions
    size_t n_filter_iter {1};       // Number of filtering

    float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};  // vector to hold quaternion
    float rpy[3] {0.f, 0.f, 0.f};           // Raw Pitch Yaw in degrees.
    float lin_acc[3] {0.f, 0.f, 0.f};       // linear acceleration (acceleration with gravity component subtracted)
    

    void calculate_quaternions();
    void calculate_altitude();
    void calculate_ground_altitude();
    void calculate_relative_altitude(float* altitude, float* ground_altitude, float* relative_altitude);
    void calculate_vertical_velocity(float* altitude, float* prev_altitude, double* elapsed_time, float * v_vel);
    void calculate_net_acceleration(float * acceleration_x, float * acceleration_y, float * acceleration_z, float * net_accel);
    void calculate_vertical_acceleration();

    void calculate_raw_pitch_yaw(float qw, float qx, float qy, float qz);
public:
    int iterator = 0; // counter of processed data

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
    float getRoll() const { return rpy[0]; }
    float getPitch() const { return rpy[1]; }
    float getYaw() const { return rpy[2]; }
    float getLinearAccX() const { return lin_acc[0]; }
    float getLinearAccY() const { return lin_acc[1]; }
    float getLinearAccZ() const { return lin_acc[2]; }
    void set_altimeter_data(float pressure, float temperature);
    void set_acceleration_data(float accel_x, float accel_y, float accel_z);
    void set_gyroscope_data(float gyro_x, float gyro_y, float gyro_z);
    void set_magnetometer_data(float mag_x, float mag_y, float mag_z);
    void process_data();
    void override_altitude_calculation(bool value);
    double last_updated_time();
};

Data::Data(/* args */)
{
    // Time starts when Data instance is created.
    this->start_chrono_time = std::chrono::high_resolution_clock::now();

    // Setup initial values of time in seconds.
    this->current_time = 1.0;
    this->previous_time = 0.0;

    // Setup highest value so ground altitude resets.
    this->ground_altitude = 9.9 * 10e9;
}

Data::~Data()
{
}

void Data::set_altimeter_data(float pressure, float temperature){
    this->pressure = pressure * 100.0;  // mbar to pa
    this->temperature = temperature;
}

void Data::set_acceleration_data(float accel_x, float accel_y, float accel_z) {
    this->acceleration_x = accel_x * GRAVITY;
    this->acceleration_y = accel_y * GRAVITY;
    this->acceleration_z = accel_z * GRAVITY;
}

void Data::set_gyroscope_data(float gyro_x, float gyro_y, float gyro_z) {
    this->gyroscope_x = gyro_x;
    this->gyroscope_y = gyro_y;
    this->gyroscope_z = gyro_z;
}

void Data::set_magnetometer_data(float mag_x, float mag_y, float mag_z) {
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
        this->calculate_altitude();
    }

    this->calculate_quaternions();
    this->calculate_raw_pitch_yaw(q[0], q[1], q[2], q[3]);
    this->calculate_ground_altitude();
    this->calculate_relative_altitude(&altitude, &ground_altitude, &relative_altitude);
    this->calculate_vertical_velocity(&relative_altitude, &previous_relative_altitude, &elapsed_time, &vertical_velocity);
    this->calculate_net_acceleration(&acceleration_x, &acceleration_y, &acceleration_z, &net_acceleration);
    this->calculate_vertical_acceleration();
    /* ------------------------------ */

    // Update previous values.
    this->previous_time = this->current_time;
    this->previous_relative_altitude = this->relative_altitude;

}

void Data::calculate_altitude() {
    altitude = 44330 * (1.0 - pow(pressure / SEA_LEVEL_PA, 0.1903));
}

void Data::calculate_ground_altitude() {
    if (ground_altitude > altitude) {
        ground_altitude = altitude;
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

void Data::calculate_vertical_acceleration() {
    imu::Matrix<3> tm = transformation_matrix(q[0], q[1], q[2], q[3]);
    imu::Vector<3> accel =  imu::Vector<3>(acceleration_x, acceleration_y, acceleration_z);
    vertical_acceleration = vertical_acceleration_from_lin(accel, tm);
}

void Data::override_altitude_calculation(bool value) {
    this->override_altitude = value;
}

void Data::calculate_quaternions() {
    float an = -acceleration_x / 1000.0F; // g to mg
    float ae = +acceleration_y / 1000.0F; // g to mg
    float ad = +acceleration_z / 1000.0F; // g to mg
    float gn = +gyroscope_x * DEG_TO_RAD;
    float ge = -gyroscope_y * DEG_TO_RAD;
    float gd = -gyroscope_z * DEG_TO_RAD;
    float mn = +magnetometer_y * 1e3 * TESLA_TO_GAUSS; // 1e3 from μ to m
    float me = -magnetometer_x * 1e3 * TESLA_TO_GAUSS; // 1e3 from μ to m
    float md = +magnetometer_z * 1e3 * TESLA_TO_GAUSS; // 1e3 from μ to m

    for (size_t i = 0; i < n_filter_iter; ++i) {
        quat_filter.update(an, ae, ad, gn, ge, gd, mn, me, md, q);
    }

}

void Data::calculate_raw_pitch_yaw(float qw, float qx, float qy, float qz) {
    // Define output variables from updated quaternion---these are Tait-Bryan angles, commonly used in aircraft orientation.
    // In this coordinate system, the positive z-axis is down toward Earth.
    // Yaw is the angle between Sensor z-axis and Earth magnetic North (or true North if corrected for local declination, looking down on the sensor positive yaw is counterclockwise.
    // Pitch is angle between sensor x-axis and Earth ground plane, toward the Earth is positive, up toward the sky is negative.
    // Roll is angle between sensor y-axis and Earth ground plane, y-axis up is positive roll.
    // These arise from the definition of the homogeneous rotation matrix constructed from quaternions.
    // Tait-Bryan angles as well as Euler angles are non-commutative; that is, the get the correct orientation the rotations must be
    // applied in the correct order which for this configuration is yaw, pitch, and then roll.
    // For more see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles which has additional links.
    float a12, a22, a31, a32, a33;  // rotation matrix coefficients for Euler angles and gravity components
    a12 = 2.0f * (qx * qy + qw * qz);
    a22 = qw * qw + qx * qx - qy * qy - qz * qz;
    a31 = 2.0f * (qw * qx + qy * qz);
    a32 = 2.0f * (qx * qz - qw * qy);
    a33 = qw * qw - qx * qx - qy * qy + qz * qz;
    rpy[0] = atan2f(a31, a33);
    rpy[1] = -asinf(a32);
    rpy[2] = atan2f(a12, a22);
    rpy[0] *= 180.0f / PI;
    rpy[1] *= 180.0f / PI;
    rpy[2] *= 180.0f / PI;
    rpy[2] += magnetic_declination;
    if (rpy[2] >= +180.f)
        rpy[2] -= 360.f;
    else if (rpy[2] < -180.f)
        rpy[2] += 360.f;

    lin_acc[0] = acceleration_x + a31;
    lin_acc[1] = acceleration_y + a32;
    lin_acc[2] = acceleration_z - a33;
}

double Data::last_updated_time() {
    return current_time;
}