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

static const char* const semaforoAmarresFile = "/tmp/semaforoAmarres.tmp";
static const char* const semaforoGruasFile = "/tmp/semaforoGruas.tmp";
static const char* const semaforoCamionesFile = "/tmp/semaforoCamiones.tmp";
static const char* const lockEntradaFile = "/tmp/lockEntrada.tmp";
static const char* const tareasGruasFile = "/tmp/tareasGruas.tmp";
static const char* const cargasCamionesFile = "/tmp/cargasCamiones.tmp";
static const char* const cargasBarcosFile = "/tmp/cargasBarcos.tmp";

enum {DESCARGAR_BARCO = 0, DESCARGAR_CAMION};

struct trabajo{

    int carga;
    short tipoTrabajo;
    pid_t proceso;

};




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
        void atenderBarcoAmarrado(struct trabajo trabajo);
        void liberarGrua();
        void asignarTrabajoAGrua();


};


#endif //CONCU_TP1_CONTROLADOR_H
