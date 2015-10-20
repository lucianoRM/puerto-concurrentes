//
// Created by luciano on 12/10/15.
//

#include "Camion.h"

Camion::Camion():carga(0) {}

Camion::~Camion(){}

struct trabajo Camion::getTrabajo(){

    pid_t pid = getpid();

    short tipo;
    if(this->carga > 0)
        tipo = DESCARGAR_CAMION;


    struct trabajo trabajoCamion;
    trabajoCamion.carga = this->carga;
    trabajoCamion.tipoTrabajo = tipo;
    trabajoCamion.proceso = pid;

    return trabajoCamion;
}


void Camion::setCarga(int nuevaCarga){

    this->carga = nuevaCarga;

}
int Camion::getCarga(){

    return this->carga;

}

void Camion::initialize(Controlador* controlador) {
    controlador->adaptarseACamion();
}

void Camion::run(Controlador* controlador) {
    if(this->carga > 0){
        Logger::getInstance()->log("[CAMION] Tengo carga de: " + std::to_string(this->carga) + ", pido que me descarguen");
        this->pedirDescarga(controlador);
        Logger::getInstance()->log("[CAMION] Me descargaron.");
    }
    Logger::getInstance()->log("[CAMION] Me voy a enlistar para envio");
    this->enlistarseParaEnvio(controlador);
    this->realizarEnvio(controlador);
    Logger::getInstance()->log("[CAMION] Envio realizado");
}


void Camion::enlistarseParaEnvio(Controlador* controlador){

    controlador->agregarCamionAFlota(getpid());
    Logger::getInstance()->log("[CAMION] Esperando para recibir carga...");

    struct trabajo trabajo;
    trabajo = controlador->darCargaACamion();
    //Hay que avisar al barco que ya termino la descarga
    controlador->notificarTransferenciaCompleta(trabajo.proceso);
    Logger::getInstance()->log("[CAMION] Fui cargado con una carga de: " + std::to_string(trabajo.carga) + " de barco: " + std::to_string(trabajo.proceso));

}

void Camion::pedirDescarga(Controlador* controlador){

    struct trabajo trabajo = this->getTrabajo();
    Logger::getInstance()->log("[CAMION] Estoy cargado con una carga de: " + std::to_string(trabajo.carga) + " ,pido que me descarguen");
    controlador->atenderCamionCargado(trabajo);

    controlador->bloquearHastaTerminar();


}


void Camion::realizarEnvio(Controlador* controlador){

    srand(time(NULL));
    int vieneConCarga = rand() % 2;
    //if(vieneConCarga) {
    //    this->carga = rand() % 1000;
    //}

    this->carga = 0;

}