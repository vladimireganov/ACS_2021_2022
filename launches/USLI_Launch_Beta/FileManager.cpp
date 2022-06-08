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

    settingsFile.open(settingsFileName, std::ios::in);
    if (!settingsFile.is_open()) {
        std::cout << millis() << "\t[FileManager] Error opening settings file ❌" << std::endl;
        correctlyInitialized = false;
        return;
    }

    getline(settingsFile, line);

    uniqueFileID = line;
    integerUniqueFileID = stoi(uniqueFileID);
    integerUniqueFileID++;
    settingsFile.close();

    settingsFile.open(settingsFileName, std::ios::out);
    if (!settingsFile.is_open()) {
        std::cout << millis() << "\t[FileManager] Error opening settings file ❌" << std::endl;
        correctlyInitialized = false;
        return;
    }

    settingsFile << integerUniqueFileID;
    settingsFile.close();
}


void FileManager::openLogFile() {
    logFile.open(logFileName, std::ios::app);

    if (!logFile.is_open()) {
        correctlyInitialized = false;
        return;
    }

    std::cout << millis() << "\t[FileManager] Successfully opened log file ✔️\n";
}

bool FileManager::start() {
    this->getUniqueFileID();
    this->openLogFile();

    if (correctlyInitialized) {
        std::cout << millis() << "\t[FileManager] Successfully started ✔️\n";
    }

    return correctlyInitialized;
}

std::ofstream * FileManager::createFile(std::string fileName) {
    std::ofstream * newFile = new std::ofstream;
    std::string newFileNamePath = baseDirectory + "/" + uniqueFileID + "_" + fileName;
    std::cout << millis() << "\t[FileManager] Creating new file in path: \"" << newFileNamePath << "\"\n";
    newFile->open(newFileNamePath, std::ios::out);

    if (!newFile->is_open()) {
        logFile << millis() << "\t[FileManager] Error opening " << newFileNamePath << " file!" << std::endl;
        std::cout << millis() << "\t[FileManager] Error opening " << newFileNamePath << " file!" << std::endl;
    }
    else {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        std::string dateTimeString = oss.str();

        logFile << millis() << "\t[FileManager] Opened " << newFileNamePath << " at " << dateTimeString << std::endl;
        std::cout << millis() << "\t[FileManager] Opened " << newFileNamePath << " at " << dateTimeString << std::endl;
    }
    
    return newFile;
}