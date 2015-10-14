//
// Created by luciano on 10/10/15.
//

#ifndef CONCU_TP1_CONTROLADOR_H
#define CONCU_TP1_CONTROLADOR_H

#include <stdio.h>
#include <iostream>

#include "Semaforo.h"
#include "Fifo.h"
#include "FifoEscritura.h"
#include "FifoLectura.h"
#include "LockFile.h"
#include <stdio.h>
#include "Logger.h"


>>>>>>> 07580b15928ea9a54f72cf27034bfba18d9ef563

static const char* const semaforoAmarresFile = "/tmp/semaforoAmarres.tmp";
static const char* const semaforoGruasFile = "/tmp/semaforoGruas.tmp";
static const char* const semaforoCamionesFile = "/tmp/semaforoCamiones.tmp";
static const char* const lockEntradaFile = "/tmp/lockEntrada.tmp";
static const char* const tareasGruasFile = "/tmp/tareasGruas.tmp";
static const char* const cargasACamionesFile = "/tmp/cargasACamiones.tmp";
static const char* const cargasABarcosFile = "/tmp/cargasABarcos.tmp";

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
        FifoEscritura* cargasACamiones;
        FifoLectura* cargasDeBarcos;
        FifoEscritura* cargasABarcos;
        FifoLectura* cargasDeCamiones;

    public:
        Controlador(int cantidadAmarres);
        ~Controlador();

        //Barcos
        void cederAmarre();
        void dejarPasarBarco();
        void liberarEntrada();
        void atenderBarcoAmarrado(struct trabajo trabajo); //Toma trabajo de barco
        void agregarBarcoAFlota(); //Agrega barco a la flota de barcos disponibles para envios

        //Camiones
        void atenderCamionCargado(struct trabajo trabajo);
        void agregarCamionAFlota(); //Agrega el camion a la flota de camiones disponibles para envios

        //Gruas
        void liberarGrua();
        void asignarTrabajoAGrua();


};


#endif //CONCU_TP1_CONTROLADOR_H
