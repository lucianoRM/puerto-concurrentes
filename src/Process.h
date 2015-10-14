//
// Created by Martin Gonzalez on 13/10/15.
//

#ifndef CONCU_PROCESS_H
#define CONCU_PROCESS_H

#include <unistd.h>
#include <stdlib.h>
#include "Controlador.h"

class Process {

public:
    Process();
    ~Process();
    pid_t start(Controlador* controlador);
    virtual void run(Controlador* controlador) = 0;
};


#endif //CONCU_PROCESS_H
