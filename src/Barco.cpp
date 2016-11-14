//
// Created by luciano on 10/10/15.
//

#include <sys/time.h>
#include "Barco.h"

Barco::Barco():Process("BARCO") {

    generarCarga();

}

Barco::~Barco(){
}

void Barco::amarrar(Controlador* controlador){
    Logger::getInstance()->log("[BARCO] Voy a pedir amarre y me quedo esperando que me dejen entrar");
    controlador->cederAmarre();
    Logger::getInstance()->log("[BARCO] Tengo amarre, voy a entrar");
    controlador->dejarPasarBarco();
    Logger::getInstance()->log("[BARCO] Estoy entrando");
    controlador->liberarEntrada();
    Logger::getInstance()->log("[BARCO] Amarre");
    controlador->cargarCaja((float)carga*2/100);
    Logger::getInstance()->log("[BARCO] Pague");
}



void Barco::partir(Controlador* controlador){
    Logger::getInstance()->log("[BARCO] Voy a pedir para salir del puerto");
    controlador->dejarSalirBarco();
    Logger::getInstance()->log("[BARCO] Estoy saliendo");
    controlador->notificarSalida();
    Logger::getInstance()->log("[BARCO] Me fui del puerto");
}


struct trabajo Barco::getTrabajo(){

    pid_t pid = getpid();

    short tipo;
    if(this->carga > 0)
        tipo = DESCARGAR_BARCO;


    struct trabajo trabajoBarco;
    trabajoBarco.carga = this->carga;
    trabajoBarco.tipoTrabajo = tipo;
    trabajoBarco.proceso = pid;

    return trabajoBarco;
}


void Barco::generarCarga(){

    struct timeval tv;
    gettimeofday(&tv,NULL);
    srand(tv.tv_usec);
    this->carga = rand() % 1000;
}


void Barco::setCarga(int nuevaCarga){

    this->carga = nuevaCarga;

}
int Barco::getCarga(){

    return this->carga;

}

void Barco::initialize(Controlador* controlador) {
    controlador->adaptarseABarco(); //Setea todos los metodos de concurrencia del controlador que corresponden al barco
}

void Barco::run(Controlador* controlador) {

    this->amarrar(controlador); //Permite que el barco amarre. Tiene en cuenta la cantidad de amarres libres y que entren de a uno por vez.
    struct trabajo trabajo = this->getTrabajo();

    controlador->atenderBarcoAmarrado(trabajo); //Le da el trabajo a realizar a la grua

    controlador->bloquearHastaTerminar(); //bloquea el barco hasta que el camion se cargue.

    if(!(getpid() % 2)) { //Si el pid del barco es par, se queda esperando una carga,sino se va vacio
        controlador->agregarBarcoAFlota(getpid());
        struct trabajo trabajo = controlador->darCargaABarco();
        Logger::getInstance()->log("[BARCO] Me cargaron con una carga de: " + std::to_string(trabajo.carga) + " del camion: " + std::to_string(trabajo.proceso));
        this->carga = trabajo.carga;
    }
    this->partir(controlador);

    this->shouldRun = false;
}
