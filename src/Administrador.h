//
// Created by agu on 10/18/15.
//

#ifndef CONCU_ADMINISTRADOR_H
#define CONCU_ADMINISTRADOR_H


#include "Process.h"
#include "MemoriaCompartida2.h"

class Administrador : public Process {
private:
    float logTime;
public:
    Administrador(float logTime);
    void initialize(Controlador* controlador);
    void run(Controlador* controlador);
};

#endif //CONCU_ADMINISTRADOR_H
