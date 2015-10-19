//
// Created by Martin Gonzalez on 14/10/15.
//

#include "Logger.h"

Logger* Logger::instance = NULL;
const std::string Logger::fileName = "/tmp/simulacion.log";

Logger::Logger() :
    logFile((char*) fileName.c_str()) {
}

Logger* Logger::getInstance() {
    if (instance == NULL)
        instance = new Logger();
    return instance;
}

void Logger::destroy() {
    if (instance != NULL) {
        delete (instance);
        instance = NULL;
    }
}

std::string Logger::getFormattedTime() {
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer,80,"%Y%m%d %H:%M:%S",timeinfo);

    return std::string(buffer);
}

void Logger::log(std::string message) {

    std::string time = getFormattedTime();


    int tomarLockRes = logFile.tomarLock();

    std::stringstream ss;

    ss << "[" << time << "] [" << getpid() << "] " << message << std::endl;

    std::string const log_msg = ss.str();

    if (tomarLockRes != 0) {
        std::cerr << "Error al tomar el lock del log: " << fileName << ". Mensaje que iba a escribir: " << log_msg << std::endl;
        return;
    }

    int res = logFile.escribir(log_msg.c_str(), log_msg.length());

    if (res < 0) {
        std::cerr << "Ha ocurrido un error al escibir el mensaje: "<< log_msg << std::endl;
    }


    std::cout <<  ss.str() << std::endl;



    int liberarLockRes = logFile.liberarLock();

    if (liberarLockRes!= 0) {
        std::cerr << "Error al liberar el lock del log: " << fileName << ". Mensaje que escribi: " << log_msg << std::endl;
        return;
    }
}


void Logger::log(std::string message,int r){

    std::cout << "\033[1;31m" + message + "\033[0m" << std::endl;

}


Logger::~Logger() {

}
