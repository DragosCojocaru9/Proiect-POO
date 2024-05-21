#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <errno.h>
class Logger
{
    //std::ofstream logfile;
    FILE* pfile;
    errno_t err;
public:
    Logger() {
        err = fopen_s(&pfile, "logger.txt", "a");
        if (err != 0) {
            perror("Error opening file");
        }
        //logfile.open("logger.txt", std::ios::out | std::ios::app);
        ///*if (!logfile.is_open()) {
        //    std::cerr << "Error opening file: " << "logger.txt" << std::endl;
        //}*/
    }

    ~Logger() {
        /*if (logfile.is_open()) {
            logfile.close();
        }*/
        fclose(pfile);
    }

    void log(const char* message) {
       /* if (logfile.is_open()) {
            logfile << message << std::endl;
            std::cout << message << std::endl;
        }
        else {
            std::cerr << "Log file is not open!" << std::endl;
        }*/
        fprintf(pfile, message);
    }
};