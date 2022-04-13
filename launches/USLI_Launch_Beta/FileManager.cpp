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

#include "FileManager.h"

void FileManager::getUniqueFileID() {
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


void FileManager::openLogFile() {
    logFile.open(baseDirectory + "/" + logFileName);

    if (!logFile.is_open()) {
        correctlyInitialized = false;
    }
}

bool FileManager::start() {
    this->getUniqueFileID();
    this->openLogFile();

    return correctlyInitialized;
}

std::ofstream * FileManager::createFile(std::string fileName) {
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