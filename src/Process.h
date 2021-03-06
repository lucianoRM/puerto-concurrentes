//
// Created by Martin Gonzalez on 13/10/15.
//

#ifndef CONCU_PROCESS_H
#define CONCU_PROCESS_H

#include <unistd.h>
#include <stdlib.h>
#include "Logger.h"
#include "Controlador.h"
#include "SignalHandler.h"
#include "SIGINT_Handler.h"
#include "EndProcessException.h"
#include "Logger.h"

class Process {
private:
    std::string name;

protected:
    void log(std::string msg);
    bool shouldRun;
    virtual void initialize(Controlador* controlador) = 0;
    void destroy(Controlador* controlador);

public:
    Process(std::string name);
    ~Process();
    pid_t start(Controlador* controlador);
    virtual void run(Controlador* controlador) = 0;
};


#endif //CONCU_PROCESS_H
