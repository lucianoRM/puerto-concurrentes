//
// Created by luciano on 10/10/15.
//

#include "Barco.h"

Barco::Barco(){

    carga = 100;

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


void Barco::setCarga(int nuevaCarga){

    this->carga = nuevaCarga;

}
int Barco::getCarga(){

    return this->carga;

}


void Barco::run(Controlador* controlador) {

    this->amarrar(controlador);

    //sleep(10);

}
