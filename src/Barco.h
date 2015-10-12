//
// Created by luciano on 10/10/15.
//

#ifndef CONCU_TP1_BARCO_H
#define CONCU_TP1_BARCO_H

#include <iostream>
#include "Controlador.h"

enum {DESCARGAR = 0, CARGAR};

struct trabajoBarco{

    int carga;
    short tipoTrabajo;
    pid_t procesoBarco;

};

class Barco {

    private:
        int carga;

    public:
        Barco();
        ~Barco();
        void amarrar(Controlador* controlador);
        struct trabajoBarco getTrabajo();
        void setCarga(int nuevaCarga);
        int getCarga();



};


#endif //CONCU_TP1_BARCO_H
