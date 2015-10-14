//
// Created by Martin Gonzalez on 14/10/15.
//

#ifndef CONCU_LOGGER_H
#define CONCU_LOGGER_H

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

#include "LockFile.h"

class Logger {
public:
    static Logger* getInstance();
    void log(std::string message);
    static void destroy();
private:
    static Logger* instance;
    static const std::string fileName;
    LockFile logFile;
    Logger();
    ~Logger();
    std::string getFormattedTime();
};

#endif //CONCU_LOGGER_H
