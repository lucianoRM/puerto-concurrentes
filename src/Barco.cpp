//
// Created by luciano on 10/10/15.
//

#include "Barco.h"

Barco::Barco(Controlador* controlador){

    this->controlador =controlador;
    carga = 100;

}

Barco::~Barco(){
}

void Barco::amarrar(){
    std::cout << getpid() << " :Voy a pedir amarre" << std::endl;
    this->controlador->cederAmarre();
    std::cout << getpid() << " :tengo amarre, voy a entrar" << std::endl;
    this->controlador->dejarPasarBarco();
    std::cout << getpid() << " :Estoy entrando" << std::endl;
    sleep(3);
    this->controlador->liberarEntrada();
    std::cout << getpid() << " :Amarre" << std::endl;

}


