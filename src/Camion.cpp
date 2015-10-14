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


}


void Camion::enlistarseParaEnvio(Controlador* controlador){
    

}

void Camion::pedirDescarga(Controlador* controlador){



}
void Camion::pedirTrabajo(Controlador* controlador){

    if(this->carga > 0){
        this->pedirDescarga(controlador);
    }else{
        this->enlistarseParaEnvio(controlador);
    }


}
void Camion::realizarEnvio(Controlador* controlador){


}