//
// Created by agu on 10/18/15.
//

#ifndef CONCU_ADMINISTRADOR_H
#define CONCU_ADMINISTRADOR_H


#include "Process.h"
#include "MemoriaCompartida2.h"

class Administrador : public Process {
    void run(Controlador*);
};

#endif //CONCU_ADMINISTRADOR_H