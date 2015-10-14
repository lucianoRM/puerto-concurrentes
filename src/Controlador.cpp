//
// Created by luciano on 10/10/15.
//

#include <iostream>
#include "Controlador.h"

Controlador::Controlador(int cantidadAmarres) {

    //Creamos los archivos que se van a usar para los controles de concurrencia
    mknod(semaforoAmarresFile,S_IFREG|0777,0);
    mknod(semaforoGruasFile,S_IFREG|0777,0);
    mknod(semaforoCamionesFile,S_IFREG|0777,0);
    mknod(lockEntradaFile,S_IFREG|0777,0);



    this->semaforoAmarres = new Semaforo(semaforoAmarresFile,cantidadAmarres);
    this->entrada = new LockFile(lockEntradaFile);
    this->tareasAGrua = new FifoEscritura(tareasGruasFile);
    this->tareasGruaPendientes = new FifoLectura(tareasGruasFile);


}
Controlador::~Controlador() {

    this->semaforoAmarres->eliminar();
    delete this->semaforoAmarres;
    delete this->entrada;
    this->tareasAGrua->eliminar();
    this->tareasGruaPendientes->eliminar();
    delete this->tareasAGrua;
    delete this->tareasGruaPendientes;
    unlink(semaforoAmarresFile);
    unlink(semaforoCamionesFile);
    unlink(semaforoGruasFile);
    unlink(lockEntradaFile);


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



void Controlador::atenderBarcoAmarrado(struct trabajo trabajo){

    //TODO: SI EL BARCO NO TIENE TRABAJO DE DESCARGA FIJARSE SI HAY ALGUN CAMION Y SINO DEJARLO IRSE
    this->tareasAGrua->escribir(&trabajo,sizeof(trabajo));

}

void Controlador::asignarTrabajoAGrua(){



}
