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

#define FILE_MANAGER_BASE_DIRECTORY ""
#define FILE_MANAGER_SETTINGS_FILE_NAME "filemanager_settings.txt"
#define FILE_MANAGER_LOG_FILE_NAME "filemanager_log.txt"

class FileManager {
    bool correctlyInitialized = true;

    const std::string baseDirectory = FILE_MANAGER_BASE_DIRECTORY;
    const std::string settingsFileName = FILE_MANAGER_SETTINGS_FILE_NAME;
    const std::string logFileName = FILE_MANAGER_LOG_FILE_NAME;
    std::ofstream logFile;
    std::string uniqueFileID;

    bool isCorrectlyInitialized() {
        return correctlyInitialized;
    }

    void getUniqueFileID() {
        std::fstream settingsFile;
        std::string line;
        int integerUniqueFileID;

        settingsFile.open(baseDirectory + "/" + settingsFileName, std::ios::in);
        if (!settingsFile.is_open()) {
            std::cout << seconds() << "Error opening FileManager settings file" << std::endl;
            correctlyInitialized = false;
            return;
        }

        getline(settingsFile, line);

        uniqueFileID = line;
        integerUniqueFileID = stoi(uniqueFileID);
        integerUniqueFileID++;
        settingsFile.close();

        settingsFile.open(baseDirectory + "/" + settingsFileName, std::ios::out);
        if (!settingsFile.is_open()) {
            std::cout << seconds() << "Error opening FileManager settings file" << std::endl;
            correctlyInitialized = false;
            return;
        }

        settingsFile << integerUniqueFileID;
        settingsFile.close();
    }


    void openLogFile() {
        logFile.open(baseDirectory + "/" + logFileName);

        if (!logFile.is_open()) {
            correctlyInitialized = false;
        }
    }

public:
    FileManager() {
        getUniqueFileID();
        openLogFile();
    }

    std::ofstream * createFile(std::string fileName) {
        std::ofstream * newFile;
        newFile->open(baseDirectory + "/" + fileName + "_" + uniqueFileID);

        if (!newFile->is_open()) {
            logFile << seconds() << "Error opening " << fileName << "_" << uniqueFileID << " file!" << std::endl;
            std::cout << seconds() << "Error opening " << fileName << "_" << uniqueFileID << " file!" << std::endl;
        }
        else {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            std::ostringstream oss;
            oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
            std::string dateTimeString = oss.str();

            logFile << seconds() << "Opened " << fileName << "_" << uniqueFileID << " at " << dateTimeString << std::endl;
            std::cout << seconds() << "Opened " << fileName << "_" << uniqueFileID << " at " << dateTimeString << std::endl;
        }
        
        return newFile;
    }
};

#endif;