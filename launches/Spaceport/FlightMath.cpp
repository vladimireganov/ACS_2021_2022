/**
 * @file FlightMath.h
 * @author Kazybek Mizam (kzm0099@auburn.edu)
 * @author Austin Le... (you@domain.com) // TODO last name and email
 * @brief Library with IMU math functions that are used to process
 * raw data like pressure, quaternions to useful data such as vertical velocity
 * and vertical acceleration.
 * 
 * 
 * @version 0.2
 * @date 2022-04-17
 * 
 * @copyright Copyright (c) AURA Embedded Systems 2021
 * 
 */

#include "FlightMath.h"

const double GRAVITY = 9.80665;

double net_value(float x, float y, float z)
{
    return sqrt(x*x + y*y + z*z);
}

double vertical_velocity(float altitude, float prev_altitude, double elapsed_time)
{
    double delta_alt = altitude - prev_altitude;
    return (double)(delta_alt / elapsed_time);
}

double projected_altitude(double altitude, double velocity, double acceleration)
{
    return fabs(velocity * velocity) / (2.0 * fabs(acceleration + GRAVITY)) * 
        log(fabs(acceleration) / GRAVITY) + altitude;
}

double altitude_from_pressure(double pressure)
{
    double seaLevelhPa = 1013.25;
    double alt;

    pressure /= 100;
    alt = 44330 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));

    return alt;
}

double low_pass_filter(double previous_value, double current_value, double elapsed_time) {
    const double RC = 0.3;
    const double alpha = elapsed_time / (RC + elapsed_time);
    return (alpha * current_value) + (1.0 - alpha) * previous_value;
}

double simple_projected_altitude(double altitude, double velocity)
{
    return altitude + (velocity * velocity) / (2.0 * GRAVITY);
}