//
// Created by Martin Gonzalez on 14/10/15.
//

#include "Logger.h"

Logger* Logger::instance = NULL;
const std::string Logger::fileName = "./simulacion.log";

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

void Logger::log(const std::string message) {
    int tomarLockRes = logFile.tomarLock();

    if (tomarLockRes != 0) {
        std::cerr << " [" << getpid() << "] Error al tomar el lock del log: " << fileName << ". Mensaje que iba a escribir: " << message << std::endl;
        return;
    }

    std::stringstream ss;

    ss << "[" << getpid() << "] " << message << std::endl;

    std::string const log_msg = ss.str();

    int res = logFile.escribir(log_msg.c_str(), log_msg.length());

    if (res < 0) {
        std::cerr << " [" << getpid() << "] Ha ocurrido un error al escibir el mensaje: "<< ss.str() << std::endl;
    }

    std::cout << ss.str() << std::endl;

    int liberarLockRes = logFile.liberarLock();

    if (liberarLockRes!= 0) {
        std::cerr << " [" << getpid() << "] Error al liberar el lock del log: " << fileName << ". Mensaje que escribi: " << message << std::endl;
        return;
    }
}

Logger::~Logger() {

}
