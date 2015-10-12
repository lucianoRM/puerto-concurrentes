//
// Created by luciano on 10/10/15.
//

#ifndef CONCU_TP1_BARCO_H
#define CONCU_TP1_BARCO_H

#include <iostream>
#include "Controlador.h"


class Barco {

    private:
        Controlador* controlador;
        int carga;

    public:
        Barco(Controlador* controlador);
        ~Barco();
        void amarrar();



};


#endif //CONCU_TP1_BARCO_H
