//
// Created by luciano on 10/10/15.
//

#ifndef CONCU_TP1_CONTROLADOR_H
#define CONCU_TP1_CONTROLADOR_H

#include "Semaforo.h"
#include "Fifo.h"
#include "FifoEscritura.h"
#include "FifoLectura.h"
#include "LockFile.h"


class Controlador {

    private:
        Semaforo* semaforoAmarres;
        Semaforo* semaforoGruas;
        Semaforo* semaforoCamiones;
        LockFile* entrada;
        FifoLectura* tareasGruaPendientes;
        FifoEscritura* tareasAGrua;
        FifoLectura* camionesVacios;
        FifoEscritura* cargasACamiones;
        FifoLectura* barcosVacios;
        FifoEscritura* cargasABarcos;

    public:
        Controlador(int cantidadAmarres);
        ~Controlador();
        void cederAmarre();
        void dejarPasarBarco();
        void liberarEntrada();
        void liberarGrua();
        void asignarGrua();


};


#endif //CONCU_TP1_CONTROLADOR_H
