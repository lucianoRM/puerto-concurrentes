//
// Created by luciano on 10/10/15.
//

#include "Controlador.h"

Controlador::Controlador(int cantidadAmarres) {


    int fd;
    //Creamos los archivos que se van a usar para los controles de concurrencia
    fd = open(semaforoAmarresFile,O_CREAT|O_WRONLY,0777);
    close(fd);
    fd = open(semaforoGruasFile,O_CREAT|O_WRONLY,0777);
    close(fd);
    fd = open(semaforoCamionesFile,O_CREAT|O_WRONLY,0777);
    close(fd);
    fd = open(lockEntradaFile,O_CREAT|O_WRONLY,0777);
    close(fd);
    fd = open(tareasGruasFile,O_CREAT|O_WRONLY,0777);
    close(fd);
    fd = open(cargasCamionesFile,O_CREAT|O_WRONLY,0777);
    close(fd);
    fd = open(cargasBarcosFile,O_CREAT|O_WRONLY,0777);
    close(fd);










    this->semaforoAmarres = new Semaforo(semaforoAmarresFile,cantidadAmarres);
    this->entrada = new LockFile(lockEntradaFile);
    this->tareasAGrua = new FifoEscritura(tareasGruasFile);
    this->tareasGruaPendientes = new FifoLectura(tareasGruasFile);


}
Controlador::~Controlador() {

    delete this->semaforoAmarres;
    delete this->entrada;
    delete this->tareasAGrua;
    delete this->tareasGruaPendientes;
    remove(semaforoAmarresFile);
    remove(semaforoCamionesFile);
    remove(semaforoGruasFile);
    remove(lockEntradaFile);
    remove(tareasGruasFile);
    remove(cargasBarcosFile);
    remove(cargasCamionesFile);


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
