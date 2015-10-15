//
// Created by luciano on 12/10/15.
//

#include "Camion.h"

Camion::Camion(){

    carga = 0;

}


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


void Camion::run(Controlador* controlador) {

    while(true){
        if(this->carga > 0){
            this->pedirDescarga(controlador);
        }
        this->enlistarseParaEnvio(controlador);
        this->realizarEnvio(controlador);
    }

}


void Camion::enlistarseParaEnvio(Controlador* controlador){

    Logger::getInstance()->log("[CAMION] Esperando para recibir carga...");
    struct trabajo trabajo = controlador->agregarCamionAFlota();
    this->carga = trabajo.carga;
    Logger::getInstance()->log("[CAMION] Fui cargado con una carga de: " + std::to_string(trabajo.carga) + " de barco: " + std::to_string(trabajo.proceso));


}

void Camion::pedirDescarga(Controlador* controlador){

    struct trabajo trabajo = this->getTrabajo();
    Logger::getInstance()->log("[CAMION] Estoy cargado con una carga de: " + std::to_string(trabajo.carga) + " ,pido que me descarguen");
    controlador->atenderCamionCargado(trabajo);


}


void Camion::realizarEnvio(Controlador* controlador){


}