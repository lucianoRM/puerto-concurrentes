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
#include "SharedMemory.h"
#include "EndProcessException.h"

static const char* const semaforoAmarresFile = "/tmp/semaforoAmarres.tmp";
static const char* const lockLecturaCargasABarcosFile = "/tmp/lockLecturaCargasABarcos.tmp";
static const char* const lockLecturaCargasACamionesFile = "/tmp/lockLecturaCargasACamiones.tmp";
static const char* const lockLecturaTrabajosAGruasFile = "/tmp/lockLecturaTrabajosAGruas.tmp";
static const char* const lockEntradaFile = "/tmp/lockEntrada.tmp";
static const char* const lockSalidaFile = "/tmp/lockSalida.tmp";
static const char* const tareasAGruaFile = "/tmp/tareasAGrua.tmp";
static const char* const camionesVaciosFile = "/tmp/camionesVacios.tmp";
static const char* const barcosVaciosFile = "/tmp/barcosVacios.tmp";
static const char* const cajaFile = "/tmp/caja.tmp";

//NO CAMBIAR PORQUE SE NECESITA QUE SEAN ESTOS NUMEROS--------------------------------------

enum {DESCARGAR_BARCO = 0, DESCARGAR_CAMION};
enum {BARCO = 0,CAMION};

//NO CAMBIAR PORQUE SE NECESITA QUE SEAN ESTOS NUMEROS--------------------------------------

struct trabajo{

    int carga;
    short tipoTrabajo;
    pid_t proceso;

};




class Controlador {

    private:
        Semaforo* semaforoAmarres;

        LockFile* entrada;
        LockFile* salida;
        LockFile* lecturaCargasABarcos;
        LockFile* lecturaCargasACamiones;
        LockFile* lecturaTrabajosAGruas;

        FifoLectura* tareasAGruaLectura;
        FifoEscritura* tareasAGruaEscritura;
        FifoEscritura* barcosVaciosEscritura;
        FifoLectura* barcosVaciosLectura;
        FifoEscritura* camionesVaciosEscritura;
        FifoLectura* camionesVaciosLectura;

        //Canales a barcos y a camiones se van a abrir para cada proceso, pero es necesario que la clase controlador los tenga
        FifoEscritura* cargaEscritura;
        FifoLectura* cargaLectura;

        FifoEscritura* esperarTrabajoTerminado;
        FifoLectura* avisarTrabajoTerminado;

        SharedMemory<float>* smCaja;
    public:
        Controlador(int cantidadAmarres);
        ~Controlador();
        void destruir(); //destruye todos los elementos de control de concurrencia que tiene.
        void destruirControlesEspecificos(); //Destruye los fifos de cargas que cada barco o camion abrio con su pid y los de aviso.


        //Para comunicacion entre camion y barco para avisarse cuando ya terminaron sus cargas y descargas
        void bloquearHastaTerminar();
        void notificarTransferenciaCompleta(pid_t pidFuenteDeCarga);


        //Barcos
        void cederAmarre();
        void dejarPasarBarco();
        void liberarEntrada();
        void atenderBarcoAmarrado(struct trabajo trabajo); //Toma trabajo de barco
        void  agregarBarcoAFlota(pid_t barcoPid); //Agrega barco a la flota de barcos disponibles para envios
        struct trabajo darCargaABarco();
        void adaptarseABarco(); //Abre los fifos y pipes correspondientes del modo necesario
        void dejarSalirBarco();
        void notificarSalida();

        //Camiones
        void atenderCamionCargado(struct trabajo trabajo);
        void agregarCamionAFlota(pid_t camionPid); //Agrega el camion a la flota de camiones disponibles para envios
        struct trabajo darCargaACamion(); //Toma la carga para el camion y la devuelve.
        void adaptarseACamion(); //Abre los fifos y pipes correspondientes del modo necesario


        //Gruas
        struct trabajo asignarTrabajoAGrua();
        void descargarGrua(struct trabajo trabajo,pid_t pidTransporte);
        pid_t tomarTransporteVacio(int transporte);
        void adaptarseAGrua(); //Abre los fifos y pipes correspondientes del modo necesario

        // Caja
        void cargarCaja(float);
        float valorCaja();
};


#endif //CONCU_TP1_CONTROLADOR_H
