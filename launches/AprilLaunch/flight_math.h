#ifndef FLIGHT_MATH_H
#define FLIGHT_MATH

#include "imumaths.h"

static imu::Matrix<3> transformation_matrix(float qw, float qx, float qy, float qz)
{
    float a = qw;
    float b = qx;
    float c = qy;
    float d = qz;

    imu::Vector<3> row1 =
        imu::Vector<3>(a * a + b * b - c * c - d * d, 2 * b * c - 2 * a * d,
                    2 * b * d + 2 * a * c);
    imu::Vector<3> row2 =
        imu::Vector<3>(2 * b * c + 2 * a * d, a * a - b * b + c * c - d * d,
                    2 * c * d - 2 * a * b);
    imu::Vector<3> row3 =
        imu::Vector<3>(2 * b * d - 2 * a * c, 2 * c * d + 2 * a * b,
                    a * a - b * b - c * c + d * d);

    imu::Matrix<3> trans_mat;
    trans_mat.vector_to_row(row1, 0);
    trans_mat.vector_to_row(row2, 1);
    trans_mat.vector_to_row(row3, 2);
    return trans_mat;
}

/**
 * Function that returns vertical acceleration measured with ground frame. 
 * Useful to check if rocket engine burnt out
 * @param linear_acceleration Linear Acceleration from accelerometer
 * @return vertical acceleration
*/
static double vertical_acceleration_from_lin(imu::Vector<3> linear_acceleration, imu::Matrix<3> transformation_matrix)
{
    imu::Matrix<3> trans_mat = transformation_matrix;
    imu::Vector<3> accel = linear_acceleration;
    imu::Vector<3> inertial_accel;
    for (int i = 0; i < 3; i++)
    {
        imu::Vector<3> row = trans_mat.row_to_vector(i);
        inertial_accel[i] = row.dot(accel);
    }
    return inertial_accel[2];
}

#endif