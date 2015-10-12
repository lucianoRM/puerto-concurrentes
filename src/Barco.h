//
// Created by luciano on 10/10/15.
//

#ifndef CONCU_TP1_BARCO_H
#define CONCU_TP1_BARCO_H

#include <iostream>
#include "Controlador.h"


class Barco {

    private:
        int carga;

    public:
        Barco();
        ~Barco();
        void amarrar(Controlador* controlador);
        struct trabajo getTrabajo();
        void setCarga(int nuevaCarga);
        int getCarga();



};


#endif //CONCU_TP1_BARCO_H
