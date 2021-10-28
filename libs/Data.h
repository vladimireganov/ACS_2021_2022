// class for data storage
// currently stores raw data
// TO DO
// add processed data
// add functions for obtaining processed data

class Data
{
private:
    /* data */

public:
    int iterator; // number of iteration

    int iteration_time; // time
    float pressure;
    //float altitude;
    float linear_acceleration_x;
    float linear_acceleration_y;
    float linear_acceleration_z;
    float gyroscope_x;
    float gyroscope_y;
    float gyroscope_z;
    float acceleration_x;
    float acceleration_y;
    float acceleration_z;
    float gravity_x;
    float gravity_y;
    float gravity_z;

    Data(/* args */);
    ~Data();
};

Data::Data(/* args */)
{
}

Data::~Data()
{
}
