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
    string flight_data_file_name;
    string log_file_name;
    ofstream flight_data_file;
    ofstream log_file;

public:
    File_write(/* args */);
    ~File_write();
    void connect_data(Data& data);
    void write_to_file();
    bool create_table_names();
    void create_log_file();
    bool open_data_file();
    void log_error(string msg);
    void log_info(string msg);
    bool save_data();
    void close_files();
    Data* flight_data; // pointer to the data
};

File_write::File_write(/* args */)
{   
    flight_data_file_name = "data.csv";
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

bool File_write::open_data_file() {
    for (int i = 0; i < 10 && !flight_data_file.is_open(); i++) {
        flight_data_file.open(flight_data_file_name);
    }

    if (flight_data_file.is_open()) {
        return true;
    }

    return false;
}

bool File_write::create_table_names(){ //prints name of the columns in the csv file
    if (!flight_data_file.is_open()) {
        return false;
    }

    // raw data
    flight_data_file << "iterator,";
    flight_data_file << "time,";
    flight_data_file << "pressure,";
    flight_data_file << "temperature,";
    flight_data_file << "gyroscope_x,";
    flight_data_file << "gyroscope_y,";
    flight_data_file << "gyroscope_z,";
    flight_data_file << "acceleration_x,";
    flight_data_file << "acceleration_y,";
    flight_data_file << "acceleration_z,";
    flight_data_file << "magnetometer_x,";
    flight_data_file << "magnetometer_y,";
    flight_data_file << "magnetometer_z,";

    // derived data
    flight_data_file << "altitude,";
    flight_data_file << "vertical_velocity,";
    flight_data_file << "vertical_acceleration,";
    flight_data_file << "net_acceleration,";
    flight_data_file << "relative_altitude\n";
    flight_data_file.flush();

    return true;
}


void File_write::create_log_file(){ // creates log file
    log_file.open(log_file_name);
    log_file << "Created log file\n";
    flight_data_file.flush();
    log_file.close();
}

void File_write::log_error(string msg){ // logs error to the log
    if(!log_file.is_open()) {
        log_file.open(log_file_name);
    }
    log_file << "ERROR\t" + msg + "\n";
    flight_data_file.flush();
    log_file.close();
}

void File_write::log_info(string msg){ // logs error to the log
    if(!log_file.is_open()) {
        log_file.open(log_file_name);
    }
    log_file << "INFO\t" + msg + "\n";
    flight_data_file.flush();
}

bool File_write::save_data(){ // prints data into the file
    // check if file is open
    if (!flight_data_file.is_open()) {
        return false;
    }

    // raw data
    flight_data_file << flight_data->iterator << ",";
    flight_data_file << flight_data->current_time << ",";
    flight_data_file << flight_data->pressure << ",";
    flight_data_file << flight_data->temperature << ",";
    flight_data_file << flight_data->gyroscope_x << ",";
    flight_data_file << flight_data->gyroscope_y << ",";
    flight_data_file << flight_data->gyroscope_z << ",";
    flight_data_file << flight_data->acceleration_x << ",";
    flight_data_file << flight_data->acceleration_y << ",";
    flight_data_file << flight_data->acceleration_z << ",";
    flight_data_file << flight_data->magnetometer_x << ",";
    flight_data_file << flight_data->magnetometer_y << ",";
    flight_data_file << flight_data->magnetometer_z << ",";
    

    // processed data
    flight_data_file << flight_data->altitude << ",";
    flight_data_file << flight_data->vertical_velocity << ",";
    flight_data_file << flight_data->vertical_acceleration << ",";
    flight_data_file << flight_data->net_acceleration << ",";
    flight_data_file << flight_data->relative_altitude << "\n";

    flight_data_file.flush();

    return true;
}

void File_write::close_files(){
    flight_data_file.flush();
    log_file.flush();
    flight_data_file.close();
    log_file.close();
}