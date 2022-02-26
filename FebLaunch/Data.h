// class for data storage
// currently stores raw data
// TO DO
// add raw data
// add functions for obtaining processed data

// add includes for sensors
#include <string.h>

class Data
{
private:
    /* data */
    int previous_time;

public:
    int iterator; // number of iteration

    int current_time; // time

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
    float amagnetometer_z;

    // section for processed data
    // altimeter derived data
    float altitude;
    float vertical_velocity;

    // imu derived data
    float vertical_acceleration;
    float net_acceleration;

    Data(/* args */);
    ~Data();
    void get_data();
    void process_data();
};

Data::Data(/* args */)
{
}

Data::~Data()
{
}

void Data::get_data(){
    // reading all the data
}

void Data::process_data(){
    // getting processed data
}