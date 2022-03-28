#include <string>
#include <iostream>
#include <fstream>
#include "Data.h"
using namespace std;

// currently it is a template for file work
// TO DO
// create names for the files
// write insides of the functions

class File_write
{
private:
    /* data */
    //string data_test; //testing git push
    string name_of_the_file;
    string log_file_name;
    ofstream flight_data_file;
    ofstream log_file;

public:
    File_write(/* args */);
    ~File_write();
    void connect_data(Data& data);
    void write_to_file();
    void create_table_names();
    void create_log_file();
    void log_error(string msg);
    void save_data();
    void close_files();
    Data* flight_data; // pointer to the data
};

File_write::File_write(/* args */)
{   
    name_of_the_file = "data.scv";
    log_file_name = "log.txt";
    //add choosing names for files
}

File_write::~File_write()
{
    //add making sure all files closed and will not be lost
}

void File_write::connect_data(Data& data){
    flight_data = &data;
}

void File_write::create_table_names(){ //prints name of the columns in the csv file
    flight_data_file.open(name_of_the_file);

    flight_data_file << "iterator,";
    flight_data_file << "iteration_time,";
    flight_data_file << "pressure,";
    flight_data_file << "linear_acceleration_x,";
    flight_data_file << "linear_acceleration_y,";
    flight_data_file << "linear_acceleration_z,";
    flight_data_file << "gyroscope_x,";
    flight_data_file << "gyroscope_y,";
    flight_data_file << "gyroscope_z,";
    flight_data_file << "acceleration_x,";
    flight_data_file << "acceleration_y,";
    flight_data_file << "acceleration_z,";
    flight_data_file << "gravity_x,";
    flight_data_file << "gravity_y,";
    flight_data_file << "gravity_z\n";
    flight_data_file.flush();

    flight_data_file.close();
}


void File_write::create_log_file(){ // creates log file
    log_file.open(log_file_name);
    log_file << "Created log file\n";
    flight_data_file.flush();
    log_file.close();
}

void File_write::log_error(string msg){ // logs error to the log
    log_file.open(log_file_name);
    log_file << msg + "\n";
    flight_data_file.flush();
    log_file.close();
}

void File_write::save_data(){ // prints data into the file
    flight_data_file.open(name_of_the_file);
    /*
    int iterator; // number of iteration

    int iteration_time; // time
    float pressure;
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

    */
    // raw data
    flight_data_file << flight_data->iterator << ",";
    flight_data_file << flight_data->iteration_time << ",";
    flight_data_file << flight_data->pressure << ",";
    flight_data_file << flight_data->linear_acceleration_x << ",";
    flight_data_file << flight_data->linear_acceleration_y << ",";
    flight_data_file << flight_data->linear_acceleration_z << ",";
    flight_data_file << flight_data->gyroscope_x << ",";
    flight_data_file << flight_data->gyroscope_y << ",";
    flight_data_file << flight_data->gyroscope_z << ",";
    flight_data_file << flight_data->acceleration_x << ",";
    flight_data_file << flight_data->acceleration_y << ",";
    flight_data_file << flight_data->acceleration_z << ",";
    flight_data_file << flight_data->gravity_x << ",";
    flight_data_file << flight_data->gravity_y << ",";
    flight_data_file << flight_data->gravity_z << "\n";

    // processed data

    flight_data_file.flush();
    flight_data_file.close();
}

void File_write::close_files(){
    flight_data_file.flush();
    log_file.flush();
    flight_data_file.close();
    log_file.close();
}