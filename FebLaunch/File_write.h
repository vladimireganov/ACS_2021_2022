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
    ofstream data_file;
    ofstream log_file;

public:
    File_write(/* args */);
    ~File_write();
    void connect_data(Data& data);
    void write_to_file();
    void create_table_names();
    void create_log_file();
    void log_error(string msg);
    void log_info(string msg);
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
    if (!data_file.is_open()) {
        data_file.open(name_of_the_file);
    }

    // raw data
    data_file << "iterator,";
    data_file << "time,";
    data_file << "pressure,";
    data_file << "gyroscope_x,";
    data_file << "gyroscope_y,";
    data_file << "gyroscope_z,";
    data_file << "acceleration_x,";
    data_file << "acceleration_y,";
    data_file << "acceleration_z,";
    data_file << "magnetometer_x,";
    data_file << "magnetometer_y,";
    data_file << "magnetometer_z,";

    // derived data
    data_file << "altitude,";
    data_file << "vertical_velocity,";
    data_file << "vertical_acceleration,";
    data_file << "net_acceleration\n";
    data_file.flush();
}


void File_write::create_log_file(){ // creates log file
    log_file.open(log_file_name);
    log_file << "Created log file\n";
    data_file.flush();
    log_file.close();
}

void File_write::log_error(string msg){ // logs error to the log
    if(!log_file.is_open()) {
        log_file.open(log_file_name);
    }
    log_file << "ERROR\t" + msg + "\n";
    data_file.flush();
    log_file.close();
}

void File_write::log_info(string msg){ // logs error to the log
    if(!log_file.is_open()) {
        log_file.open(log_file_name);
    }
    log_file << "INFO\t" + msg + "\n";
    data_file.flush();
}

void File_write::save_data(){ // prints data into the file
    if (!data_file.is_open()) {
        data_file.open(name_of_the_file);
    }

    // raw data
    data_file << flight_data->iterator << ",";
    data_file << flight_data->current_time << ",";
    data_file << flight_data->pressure << ",";
    data_file << flight_data->gyroscope_x << ",";
    data_file << flight_data->gyroscope_y << ",";
    data_file << flight_data->gyroscope_z << ",";
    data_file << flight_data->acceleration_x << ",";
    data_file << flight_data->acceleration_y << ",";
    data_file << flight_data->acceleration_z << ",";
    data_file << flight_data->magnetometer_x << ",";
    data_file << flight_data->magnetometer_y << ",";
    data_file << flight_data->magnetometer_z << ",";
    

    // processed data
    data_file << flight_data->altitude << ",";
    data_file << flight_data->vertical_velocity << ",";
    data_file << flight_data->vertical_acceleration << ",";
    data_file << flight_data->net_acceleration << "\n";

    data_file.flush();
}

void File_write::close_files(){
    data_file.flush();
    log_file.flush();
    data_file.close();
    log_file.close();
}