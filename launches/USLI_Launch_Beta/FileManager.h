/**
 * @file FileManager.h
 * @author Kazybek Mizam (kzm0099@auburn.edu)
 * @brief Extended version of file management.
 * 
 * @version 0.1
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

#include "ArduinoTimer.h"

#define FILE_MANAGER_BASE_DIRECTORY "/home/pi/USLI_Launch_Beta/data"
#define FILE_MANAGER_SETTINGS_FILE_NAME "filemanager_settings.txt"
#define FILE_MANAGER_LOG_FILE_NAME "filemanager_log.txt"

class FileManager {
    bool correctlyInitialized = true;

    const std::string baseDirectory = FILE_MANAGER_BASE_DIRECTORY;
    const std::string settingsFileName = FILE_MANAGER_SETTINGS_FILE_NAME;
    const std::string logFileName = FILE_MANAGER_LOG_FILE_NAME;
    std::ofstream logFile;
    std::string uniqueFileID;

    void getUniqueFileID();
    void openLogFile();

public:
    bool start();
    std::ofstream * createFile(std::string fileName);
};

#endif