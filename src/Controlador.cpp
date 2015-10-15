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

    //Fifos
    result+=mknod(cargasACamionesFile,S_IFREG|0777,0);
    result+=mknod(cargasABarcosFile,S_IFREG|0777,0);
    result+=mknod(tareasGruasFile,S_IFREG|0777,0);


    if(result < 0) Logger::getInstance()->log("[CONTROLADOR] Error al crear archivos para metodos de concurrencia");

    this->semaforoAmarres = new Semaforo(semaforoAmarresFile,cantidadAmarres);
    this->semaforoGruasLibres = new Semaforo(semaforoGruasLibresFile,0);
    this->semaforoCamionesLibres = new Semaforo(semaforoCamionesLibresFile,0);
    this->semaforoBarcosLibres = new Semaforo(semaforoBarcosLibresFile,0);

    this->lecturaCargasABarcos = new LockFile(lockLecturaCargasABarcosFile);
    this->lecturaCargasACamiones = new LockFile(lockLecturaCargasACamionesFile);
    this->lecturaTrabajosAGruas = new LockFile(lockLecturaTrabajosAGruasFile);
    this->entrada = new LockFile(lockEntradaFile);

    this->tareasAGrua = new FifoEscritura(tareasGruasFile);
    this->tareasGruaPendientes = new FifoLectura(tareasGruasFile);
    this->cargasABarcos = new FifoEscritura(cargasABarcosFile);
    this->cargasDeBarcos = new FifoLectura(cargasACamionesFile);
    this->cargasACamiones = new FifoEscritura(cargasACamionesFile);
    this->cargasDeCamiones = new FifoLectura(cargasABarcosFile);




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

    delete this->cargasABarcos;
    delete this->cargasDeCamiones;
    delete this->cargasDeBarcos;
    delete this->cargasACamiones;
    delete this->tareasAGrua;
    delete this->tareasGruaPendientes;


}
void Controlador::destruir(){

    this->semaforoAmarres->eliminar();
    this->semaforoGruasLibres->eliminar();
    this->semaforoCamionesLibres->eliminar();
    this->semaforoBarcosLibres->eliminar();

    this->tareasAGrua->eliminar();
    this->tareasGruaPendientes->eliminar();
    this->cargasABarcos->eliminar();
    this->cargasACamiones->eliminar();
    this->cargasDeBarcos->eliminar();
    this->cargasDeCamiones->eliminar();

    unlink(semaforoAmarresFile);
    unlink(semaforoGruasLibresFile);
    unlink(semaforoCamionesLibresFile);
    unlink(semaforoBarcosLibresFile);


    //Locks
    unlink(lockLecturaCargasABarcosFile);
    unlink(lockLecturaCargasACamionesFile);
    unlink(lockLecturaTrabajosAGruasFile);
    unlink(lockEntradaFile);

    //Fifos
    unlink(cargasACamionesFile);
    unlink(cargasABarcosFile);
    unlink(tareasGruasFile);

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
    this->tareasAGrua->escribir(&trabajo,sizeof(trabajo));

    //no deberia ser capaz de continuar si no hay un camion vacio
    this->semaforoCamionesLibres->p(); //Un camion libre hara el v().

    //Si llega a este punto es porque hay una grua asignada y un camion vacio


}


struct trabajo Controlador::agregarBarcoAFlota(){

    struct trabajo trabajo;
    //Primero hay que avisarle a los camiones que hay un barco libre
    this->semaforoBarcosLibres->v();

    //TODO:TOMAR LOCK PARA LEER LA CARGA

    //Una vez avisado, se va a bloquear para leer la carga del camion
    this->cargasDeCamiones->leer(&trabajo,sizeof(trabajo));

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
    this->tareasGruaPendientes->leer(&trabajo,sizeof(trabajo));

    return trabajo;


}

void Controlador::descargarGrua(struct trabajo trabajo){

    //Leer el trabajo y cargarlo en el fifo correspondiente
    if(trabajo.tipoTrabajo == DESCARGAR_CAMION){
        this->cargasABarcos->escribir(&trabajo,sizeof(trabajo));
    }else{
        this->cargasACamiones->escribir(&trabajo,sizeof(trabajo));
    }

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
    this->tareasAGrua->escribir(&trabajo,sizeof(trabajo));

    //no deberia ser capaz de continuar si no hay un barco vacio
    this->semaforoBarcosLibres->p(); //Un barco libre hara el v().

    //Si llega a este punto es porque hay una grua asignada y un barco vacio



}
struct trabajo Controlador::agregarCamionAFlota(){

    struct trabajo trabajo;
    //Primero hay que avisarle a los barcos que hay un camion libre
    this->semaforoCamionesLibres->v();

    //TODO:TOMAR LOCK PARA LEER LA CARGA

    //Una vez avisado, se va a bloquear para leer la carga del barco
    this->cargasDeBarcos->leer(&trabajo,sizeof(trabajo));

    return trabajo;


}
