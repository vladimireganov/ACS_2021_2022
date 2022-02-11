// class for data storage
// currently stores raw data
// TO DO
// add raw data
// add functions for obtaining processed data

// add includes for sensors


class Data
{
private:
    /* data */

public:
    int iterator; // number of iteration

    int iteration_time; // time
    float pressure;
    float temperature;
    float linear_acceleration_x;
    float linear_acceleration_y;
    float linear_acceleration_z;
    float gyroscope_x;
    float gyroscope_y;
    float gyroscope_z;
    float acceleration_x;
    float acceleration_y;
    float acceleration_z;
    float magnetometer_x;
    float magnetometer_y;
    float magnetometer_z;
    float gravity_x;
    float gravity_y;
    float gravity_z;

    // section for processed data
    //float altitude;

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