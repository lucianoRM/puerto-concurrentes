//
// Created by luciano on 10/10/15.
//

#include "Barco.h"
#include <unistd.h>

Barco::Barco(){

    this->controlador = new Controlador();
    carga = 100;

}

Barco::~Barco(){
    delete controlador;
}

void Barco::amarrar(){
    std::cout << getpid() << " :Voy a pedir amarre" << std::endl;
    this->controlador->cederAmarre();
    std::cout << getpid() << " :tengo amarre, voy a entrar" << std::endl;
    this->controlador->dejarPasarBarco();
    std::cout << getpid() << " :Estoy entrando" << std::endl;
    sleep(1);
    std::cout << getpid() << " :Entre" << std::endl;
    this->controlador->liberarEntrada();
    std::cout << getpid() << " :Amarre" << std::endl;

}


int main(){
    std::cout << "Lo hace" << std::endl;
    for(int i = 0;i < 1;i++) {
        pid_t pid = fork();
        if (pid == 0) {
            Barco *barco = new Barco();
            barco->amarrar();
            delete barco;
        }
    }

    std::cout << getpid() << ": Termine" <<std::endl;
    return 0;
}