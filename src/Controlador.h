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
#include <errno.h>
#include <string>


static const char* const semaforoAmarresFile = "/tmp/semaforoAmarres.tmp";
static const char* const semaforoGruasLibresFile = "/tmp/semaforoGruasLibres.tmp";
static const char* const semaforoCamionesLibresFile = "/tmp/semaforoCamionesLibres.tmp";
static const char* const semaforoBarcosLibresFile = "/tmp/semaforoBarcosLibres.tmp";
static const char* const lockLecturaCargasABarcosFile = "/tmp/lockLecturaCargasABarcos.tmp";
static const char* const lockLecturaCargasACamionesFile = "/tmp/lockLecturaCargasACamiones.tmp";
static const char* const lockLecturaTrabajosAGruasFile = "/tmp/lockLecturaTrabajosAGruas.tmp";
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
        Semaforo* semaforoGruasLibres;
        Semaforo* semaforoCamionesLibres;
        Semaforo* semaforoBarcosLibres;
        LockFile* entrada;
        LockFile* lecturaCargasABarcos;
        LockFile* lecturaCargasACamiones;
        LockFile* lecturaTrabajosAGruas;
        FifoLectura* tareasGruaPendientes;
        FifoEscritura* tareasAGrua;
        FifoEscritura* cargasACamiones;
        FifoLectura* cargasDeBarcos;
        FifoEscritura* cargasABarcos;
        FifoLectura* cargasDeCamiones;

    public:
        Controlador(int cantidadAmarres);
        ~Controlador();
        void destruir(); //destruye todos los elementos de control de concurrencia que tiene.

        //Barcos
        void cederAmarre();
        void dejarPasarBarco();
        void liberarEntrada();
        void atenderBarcoAmarrado(struct trabajo trabajo); //Toma trabajo de barco
        struct trabajo agregarBarcoAFlota(); //Agrega barco a la flota de barcos disponibles para envios

        //Camiones
        void atenderCamionCargado(struct trabajo trabajo);
        struct trabajo agregarCamionAFlota(); //Agrega el camion a la flota de camiones disponibles para envios

        //Gruas
        struct trabajo asignarTrabajoAGrua();
        void descargarGrua(struct trabajo trabajo);


};


#endif //CONCU_TP1_CONTROLADOR_H
