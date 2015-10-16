//
// Created by luciano on 10/10/15.
//


#include "Controlador.h"

Controlador::Controlador(int cantidadAmarres) {

    //Creamos los archivos que se van a usar para los controles de concurrencia
    int result = 0;
    //Semaforos
    result+=mknod(semaforoAmarresFile,S_IFREG|0777,0);
    result+=mknod(semaforoGruasLibresFile,S_IFREG|0777,0);
    result+=mknod(semaforoCamionesLibresFile,S_IFREG|0777,0);
    result+=mknod(semaforoBarcosLibresFile,S_IFREG|0777,0);


    //Locks
    result+=mknod(lockLecturaCargasABarcosFile,S_IFREG|0777,0);
    result+=mknod(lockLecturaCargasACamionesFile,S_IFREG|0777,0);
    result+=mknod(lockLecturaTrabajosAGruasFile,S_IFREG|0777,0);
    result+=mknod(lockEntradaFile,S_IFREG|0777,0);


    if(result < 0) Logger::getInstance()->log("[CONTROLADOR] Error al crear archivos para metodos de concurrencia");

    this->semaforoAmarres = new Semaforo(semaforoAmarresFile,cantidadAmarres);
    this->semaforoGruasLibres = new Semaforo(semaforoGruasLibresFile,0);
    this->semaforoCamionesLibres = new Semaforo(semaforoCamionesLibresFile,0);
    this->semaforoBarcosLibres = new Semaforo(semaforoBarcosLibresFile,0);

    this->lecturaCargasABarcos = new LockFile(lockLecturaCargasABarcosFile);
    this->lecturaCargasACamiones = new LockFile(lockLecturaCargasACamionesFile);
    this->lecturaTrabajosAGruas = new LockFile(lockLecturaTrabajosAGruasFile);
    this->entrada = new LockFile(lockEntradaFile);

    this->tareasAGruaEscritura = new FifoEscritura(tareasAGruaFile);
    this->tareasAGruaEscritura->abrir();
    this->tareasAGruaLectura = new FifoLectura(tareasAGruaFile);
    this->tareasAGruaLectura->abrir();
    this->barcosVaciosEscritura = new FifoEscritura(barcosVaciosFile);
    this->barcosVaciosEscritura->abrir();
    this->barcosVaciosLectura = new FifoLectura(barcosVaciosFile);
    this->barcosVaciosLectura->abrir();
    this->camionesVaciosEscritura = new FifoEscritura(camionesVaciosFile);
    this->camionesVaciosEscritura->abrir();
    this->camionesVaciosLectura = new FifoLectura(camionesVaciosFile);
    this->camionesVaciosLectura->abrir();




}


Controlador::~Controlador() {


    delete this->semaforoAmarres;
    delete this->semaforoBarcosLibres;
    delete this->semaforoCamionesLibres;
    delete this->semaforoGruasLibres;


    delete this->entrada;
    delete this->lecturaCargasABarcos;
    delete this->lecturaCargasACamiones;
    delete this->lecturaTrabajosAGruas;

    delete this->tareasAGruaLectura;
    delete this->tareasAGruaEscritura;
    delete this->barcosVaciosLectura;
    delete this->barcosVaciosEscritura;
    delete this->camionesVaciosEscritura;
    delete this->camionesVaciosLectura;


}
void Controlador::destruir(){

    this->semaforoAmarres->eliminar();
    this->semaforoGruasLibres->eliminar();
    this->semaforoCamionesLibres->eliminar();
    this->semaforoBarcosLibres->eliminar();

    this->tareasAGruaEscritura->eliminar();
    this->tareasAGruaLectura->eliminar();
    this->barcosVaciosEscritura->eliminar();
    this->barcosVaciosLectura->eliminar();
    this->camionesVaciosLectura->eliminar();
    this->camionesVaciosEscritura->eliminar();

    unlink(semaforoAmarresFile);
    unlink(semaforoGruasLibresFile);
    unlink(semaforoCamionesLibresFile);
    unlink(semaforoBarcosLibresFile);


    //Locks
    unlink(lockLecturaCargasABarcosFile);
    unlink(lockLecturaCargasACamionesFile);
    unlink(lockLecturaTrabajosAGruasFile);
    unlink(lockEntradaFile);

}

/*##################################################################################################
 * #################################################################################################
 *
 *                                  BARCOS
 *
 * ##################################################################################################
 * ###################################################################################################
 */



void Controlador::cederAmarre(){
    int res = this->semaforoAmarres->p();
    if (res < 0) {
        std::string err = strerror(errno);
        Logger::getInstance()->log("Error cediendo amarre!" + err);
    }
}


void Controlador::dejarPasarBarco(){

    int tomarLockRes = this->entrada->tomarLock();
    if (tomarLockRes != 0) {
        Logger::getInstance()->log("Error al tomar el lock de la entrada");
        return;
    }

}

void Controlador::liberarEntrada(){

    this->entrada->liberarLock();
}



void Controlador::atenderBarcoAmarrado(struct trabajo trabajo){

    //Primero tiene que checkear que haya una grua disponible
    this->semaforoGruasLibres->p(); //Las gruas haran el v();

    //Si hay gruas disponibles, debe escribir su trabajo a la cola de trabajos a gruas
    this->tareasAGruaEscritura->escribir(&trabajo,sizeof(trabajo));


    //no deberia ser capaz de continuar si no hay un camion vacio
    this->semaforoCamionesLibres->p(); //Un camion libre hara el v().

    //Si llega a este punto es porque hay una grua asignada y un camion vacio


}


void Controlador::agregarBarcoAFlota(int barcoPid){

    //Primero hay que avisarle a los camiones que hay un barco libre
    this->semaforoBarcosLibres->v();

    //Primero hay que abrir un fifo para que pueda recibir la carga
    std::string path = "/tmp/" + std::to_string(barcoPid);
    cargaLectura = new FifoLectura(path);

}


struct trabajo Controlador::darCargaABarco(){

    struct trabajo trabajo;
    //Hay que quedarse esperando a que alguien escriba en el fifo de cargas para el barco.
    this->cargaLectura->leer(&trabajo,sizeof(trabajo));

    return trabajo;

}



/*##################################################################################################
 * #################################################################################################
 *
 *                                  GRUAS
 *
 * ##################################################################################################
 * ###################################################################################################
 */



struct trabajo Controlador::asignarTrabajoAGrua(){

    //TODO:Pedir lock de lectura de trabajos

    //Avisar que hay una grua libre para trabajar
    this->semaforoGruasLibres->v();

    struct trabajo trabajo;
    this->tareasAGruaLectura->leer(&trabajo,sizeof(trabajo));

    return trabajo;


}

void Controlador::descargarGrua(struct trabajo trabajo){


}




/*##################################################################################################
 * #################################################################################################
 *
 *                                  CAMIONES
 *
 * ##################################################################################################
 * ###################################################################################################
 */






void Controlador::atenderCamionCargado(struct trabajo trabajo){

    //Primero tiene que checkear que haya una grua disponible
    this->semaforoGruasLibres->p(); //Las gruas haran el v();

    //Si hay gruas disponibles, debe escribir su trabajo a la cola de trabajos a gruas
    this->tareasAGruaEscritura->escribir(&trabajo,sizeof(trabajo));

    //no deberia ser capaz de continuar si no hay un barco vacio
    this->semaforoBarcosLibres->p(); //Un barco libre hara el v().

    //Si llega a este punto es porque hay una grua asignada y un barco vacio



}

void Controlador::agregarCamionAFlota(int camionPid){

    //Primero hay que avisarle a los barcos que hay un camion libre
    this->semaforoCamionesLibres->v();

    std::string path = "/tmp/" + std::to_string(camionPid);

    this->cargaLectura = new FifoLectura(path);
}

struct trabajo Controlador::darCargaACamion() {

    struct trabajo trabajo;

    //Hay que leer del fifo de cargas
    this->cargaLectura->leer(&trabajo,sizeof(trabajo));

    return trabajo;

};
