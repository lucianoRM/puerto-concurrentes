//
// Created by luciano on 10/10/15.
//

#include "Controlador.h"

Controlador::Controlador() {

    this->semaforoAmarres = new Semaforo("Semaforo.cpp",5);
    this->entrada = new LockFile("LockFile.cpp");


}
Controlador::~Controlador() {

    delete this->semaforoAmarres;
    delete this->entrada;
}

void Controlador::cederAmarre(){

    this->semaforoAmarres->p();
}


void Controlador::dejarPasarBarco(){

    this->entrada->tomarLock();

}

void Controlador::liberarEntrada(){

    this->entrada->liberarLock();
}

void Controlador::liberarGrua(){

    this->semaforoGruas->v();

}

void Controlador::asignarGrua(){



}
