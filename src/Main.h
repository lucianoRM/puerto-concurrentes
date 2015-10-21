//
// Created by martin on 20/10/15.
//

#ifndef CONCU_MAIN_H
#define CONCU_MAIN_H

#include "Process.h"
#include "Barco.h"
#include "Controlador.h"
#include "C.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "Camion.h"
#include "Grua.h"
#include "Administrador.h"

class Main: public Process {
private:
    void esperarHijos(std::string nombre, std::vector<pid_t> hijos);
    void terminarHijos(std::string nombre, std::vector<pid_t> hijos);

public:
    Main();
    ~Main();
    void initialize(Controlador* c);
    void run(Controlador* c);
};


#endif //CONCU_MAIN_H
