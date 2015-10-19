//
// Created by Martin Gonzalez on 13/10/15.
//

#include "Process.h"

Process::Process() {

}

pid_t Process::start(Controlador* controlador) {
    pid_t pid = fork();

    if (pid == 0) {
        pid = getpid();

        this->run(controlador);

        delete controlador;
        Logger::destroy();

        //Logger::getInstance()->log("Termino",1);
        exit ( 0 );
    }
    return pid;
}

Process::~Process() {

}
