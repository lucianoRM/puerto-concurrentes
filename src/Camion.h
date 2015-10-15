//
// Created by luciano on 12/10/15.
//

#ifndef CONCUPORT_CAMION_H
#define CONCUPORT_CAMION_H

#include "Process.h"


class Camion : public Process {

    private:
        int carga;

    private:
        void enlistarseParaEnvio(Controlador* controlador); //Cuando el camion esta vacio
        void pedirDescarga(Controlador* controlador); //Cuando el camion esta lleno

        void realizarEnvio(Controlador* controlador);

    public:
        Camion();
        ~Camion();
        void run(Controlador* controlador);
        struct trabajo getTrabajo();
        void setCarga(int nuevaCarga);
        int getCarga();


};


#endif //CONCUPORT_CAMION_H
