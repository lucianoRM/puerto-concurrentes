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


