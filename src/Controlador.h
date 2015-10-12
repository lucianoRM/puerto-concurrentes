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
#include <stdio.h>

static const char* semaforoAmarresFile = "/tmp/semaforoAmarres.tmp";
static const char* semaforoGruasFile = "/tmp/semaforoGruas.tmp";
static const char* semaforoCamionesFile = "/tmp/semaforoCamiones.tmp";
static const char* lockEntradaFile = "/tmp/lockEntrada.tmp";
static const char* tareasGruasFile = "/tmp/tareasGruas.tmp";
static const char* cargasCamionesFile = "/tmp/cargasCamiones.tmp";
static const char* cargasBarcosFile = "/tmp/cargasBarcos.tmp";


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
