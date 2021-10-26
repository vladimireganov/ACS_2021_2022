#include <string>
#include <iostream>
#include <fstream>
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
    void write_to_file();
    void create_table_names();
    void create_log_file();
    void log_error();
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

void File_write::create_table_names(){
    data_file.open(name_of_the_file);
    data_file << "Writing this to a file.\n";//prints name of the columns in the csv file
    data_file.close();
}

void File_write::write_to_file(){
    data_file.open(name_of_the_file);
    data_file << "Writing this to a file.\n";// prints data into the file
    data_file.close();
}

void File_write::create_log_file(){ // creates log file
    log_file.open(log_file_name);
    log_file << "Created log file\n";
    log_file.close();
}

void File_write::log_error(){ // logs error to the log
    log_file.open(log_file_name);
    log_file << "error\n";
    log_file.close();
}

