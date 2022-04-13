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
#ifndef ROCKET_MATH_H
#define ROCKET_MATH_H
#include <math.h>
#include <stdlib.h>

const double GRAVITY = 9.80665;
/**
 * Function that returns net value of three values. 
 * Useful to detect any motion
 * 
 * @param x first value
 * @param y second value
 * @param z third value
 * @return net value of three vales
*/
double net_value(float x, float y, float z)
{
    return sqrt(x*x + y*y + z*z);
}

/**
 * Function that calculates vertical velocity from altitude change. 
 * Useful to check events such as launch, burnt out.
 * 
 * @param altitude Current altitude read
 * @param prev_altitude Previous altitude read
 * @param elapsed_time Elapsed time in seconds between current and previous altitude reads
 * @return vertical_velocity
*/
double vertical_velocity(float altitude, float prev_altitude, double elapsed_time)
{
    double delta_alt = altitude - prev_altitude;
    return (double)(delta_alt / elapsed_time);
}

/**
 * Function that calculates projected altitude from current altitude, velocity and acceleration. 
 * Useful to control rocket fins
 * @param altitude Current elapsed time of data read
 * @param velocity Previous elapsed time of data read
 * @param acceleration Previous elapsed time of data read
 * @return projected altitude
*/
double projected_altitude(double altitude, double velocity, double acceleration)
{
    return fabs(velocity * velocity) / (2.0 * fabs(acceleration + GRAVITY)) * 
        log(fabs(acceleration) / GRAVITY) + altitude;
}

/**
 * Function that calculates altitude from pressure. 
 * Useful to find projected altitude, apogee detection, etc.
 * @param pressure Current pressure read
 * @return altitude
*/
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

/**
 * Function that calculates projected altitude from current altitude and velocity. 
 * Useful for real-time apogee prediction.
 * 
 * @param altitude Altitude
 * @param velocity Vertical velocity
 * @return projected altitude
*/
double simple_projected_altitude(double altitude, double velocity)
{
    return altitude + (velocity * velocity) / (2.0 * GRAVITY);
}

#endif