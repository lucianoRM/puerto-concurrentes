//
// Created by luciano on 12/10/15.
//

#ifndef CONCUPORT_GRUA_H
#define CONCUPORT_GRUA_H

#include "Process.h"


class Grua :public Process{

    private:
        struct trabajo trabajo;
    public:
        void initialize(Controlador* controlador);
        void run(Controlador* controlador);
};


#endif //CONCUPORT_GRUA_H
