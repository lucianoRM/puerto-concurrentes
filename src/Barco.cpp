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
    std::cout << getpid() << " :Voy a pedir amarre" << std::endl;
    controlador->cederAmarre();
    std::cout << getpid() << " :tengo amarre, voy a entrar" << std::endl;
    controlador->dejarPasarBarco();
    std::cout << getpid() << " :Estoy entrando" << std::endl;
    controlador->liberarEntrada();
    std::cout << getpid() << " :Amarre" << std::endl;
    sleep(5);

}


struct trabajoBarco Barco::getTrabajo(){

    pid_t pid = getpid();

    short tipo;
    if(this->carga > 0){
        tipo = DESCARGAR;
    }else{
        tipo = CARGAR;
    }

    struct trabajoBarco trabajoBarco;
    trabajoBarco.carga = this->carga;
    trabajoBarco.tipoTrabajo = tipo;
    trabajoBarco.procesoBarco = pid;

    return trabajoBarco;
}


void Barco::setCarga(int nuevaCarga){

    this->carga = nuevaCarga;

}
int Barco::getCarga(){

    return this->carga;

}


