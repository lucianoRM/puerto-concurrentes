//
// Created by luciano on 10/10/15.
//

#ifndef CONCU_TP1_BARCO_H
#define CONCU_TP1_BARCO_H

#include <iostream>

#include "Controlador.h"
#include "Process.h"
#include "Logger.h"


class Barco: public Process {

    private:
        int carga;

    private:
        int amarrar(Controlador* controlador);
        int partir(Controlador* controlador);

    void generarCarga(); //Devuelve un valor aleatorio para la carga del barco

    public:
        Barco();
        ~Barco();
        void initialize(Controlador* controlador);
        void run(Controlador* controlador);
        struct trabajo getTrabajo();
        void setCarga(int nuevaCarga);
        int getCarga();



};


#endif //CONCU_TP1_BARCO_H
