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
    result+=mknod(lockSalidaFile,S_IFREG|0777,0);

    if(result < 0) Logger::getInstance()->log("[CONTROLADOR] Error al crear archivos para metodos de concurrencia");

    this->semaforoAmarres = new Semaforo(semaforoAmarresFile,cantidadAmarres);
    this->semaforoGruasLibres = new Semaforo(semaforoGruasLibresFile,0);
    this->semaforoCamionesLibres = new Semaforo(semaforoCamionesLibresFile,0);
    this->semaforoBarcosLibres = new Semaforo(semaforoBarcosLibresFile,0);

    this->lecturaCargasABarcos = new LockFile(lockLecturaCargasABarcosFile);
    this->lecturaCargasACamiones = new LockFile(lockLecturaCargasACamionesFile);
    this->lecturaTrabajosAGruas = new LockFile(lockLecturaTrabajosAGruasFile);
    this->entrada = new LockFile(lockEntradaFile);
    this->salida = new LockFile(lockSalidaFile);

    this->tareasAGruaEscritura = new FifoEscritura(tareasAGruaFile);
    this->tareasAGruaLectura = new FifoLectura(tareasAGruaFile);
    this->barcosVaciosEscritura = new FifoEscritura(barcosVaciosFile);
    this->barcosVaciosLectura = new FifoLectura(barcosVaciosFile);
    this->camionesVaciosEscritura = new FifoEscritura(camionesVaciosFile);
    this->camionesVaciosLectura = new FifoLectura(camionesVaciosFile);

    //Necesarios segun valgrind
    this->cargaEscritura = NULL;
    this->cargaLectura = NULL;
    this->esperarTrabajoTerminado = NULL;
    this->avisarTrabajoTerminado = NULL;
    //Necesarios segun valgrind


    this->smCaja = new SharedMemory<float>(cajaFile, 'C');
    smCaja->escribir(0);
}


Controlador::~Controlador() {


    delete this->semaforoAmarres;
    delete this->semaforoBarcosLibres;
    delete this->semaforoCamionesLibres;
    delete this->semaforoGruasLibres;


    delete this->entrada;
    delete this->salida;
    delete this->lecturaCargasABarcos;
    delete this->lecturaCargasACamiones;
    delete this->lecturaTrabajosAGruas;

    delete this->tareasAGruaLectura;
    delete this->tareasAGruaEscritura;
    delete this->barcosVaciosLectura;
    delete this->barcosVaciosEscritura;
    delete this->camionesVaciosEscritura;
    delete this->camionesVaciosLectura;

    if(this->cargaLectura)
        delete this->cargaLectura;
    if(this->cargaEscritura)
        delete this->cargaEscritura;

    delete this->smCaja;
}


void Controlador::bloquearHastaTerminar() {

    std::string path = "/tmp/" + std::to_string(getpid()) + ".tmp"; //No deberia haber problemas pero agrego .tmp para diferenciarlo de los fifos creados para cargas que tambien se generan a partir del pid.
    this->esperarTrabajoTerminado = new FifoEscritura(path);
    this->esperarTrabajoTerminado->abrir();
    //Aca se va a bloquear hasta que se abra del otro lado. Cuando se desbloquee ya no va a ser necesario el fifo
    this->esperarTrabajoTerminado->cerrar();
    this->esperarTrabajoTerminado->eliminar();

    delete this->esperarTrabajoTerminado;

}


void Controlador::notificarTransferenciaCompleta(pid_t pidFuenteDeCarga) {


    std::string path = "/tmp/" + std::to_string(pidFuenteDeCarga) + ".tmp";
    this->avisarTrabajoTerminado = new FifoLectura(path);
    this->avisarTrabajoTerminado->abrir();
    //Se va a bloquear hasta que se abra el fifo del otro lado.Cuando se desbloque ya no va a ser necesario
    this->avisarTrabajoTerminado->cerrar();
    this->avisarTrabajoTerminado->eliminar();

    delete this->avisarTrabajoTerminado;


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
    unlink(lockSalidaFile);

    unlink(cajaFile);
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
    //this->semaforoGruasLibres->p(); //Las gruas haran el v();


    //Si hay gruas disponibles, debe escribir su trabajo a la cola de trabajos a gruas
    int res = this->tareasAGruaEscritura->escribir(&trabajo,sizeof(trabajo));
    //if(res <= 0) Logger::getInstance()->log("Escribiendo en tareasAGruaBarco",1);


    //no deberia ser capaz de continuar si no hay un camion vacio
    //this->semaforoCamionesLibres->p(); //Un camion libre hara el v().

    //Si llega a este punto es porque hay una grua asignada y un camion vacio


}


void Controlador::agregarBarcoAFlota(pid_t barcoPid){

    this->barcosVaciosEscritura->escribir(&barcoPid,sizeof(barcoPid));
    //Primero hay que abrir un fifo para que pueda recibir la carga
    std::string path = "/tmp/" + std::to_string(barcoPid);
    cargaLectura = new FifoLectura(path);
    cargaLectura->abrir();

}


struct trabajo Controlador::darCargaABarco(){

    struct trabajo trabajo;
    //Hay que quedarse esperando a que alguien escriba en el fifo de cargas para el barco.
    int res = this->cargaLectura->leer(&trabajo,sizeof(trabajo));
    //if(res <= 0) Logger::getInstance()->log("Leyendo de cargaLecturaBarco",1);

    return trabajo;

}

void Controlador::adaptarseABarco(){

    //TODO:CHECKEAR EL ORDEN
    this->tareasAGruaEscritura->abrir();
    this->barcosVaciosEscritura->abrir();


}

void Controlador::dejarSalirBarco() {
    this->salida->tomarLock();
}

void Controlador::notificarSalida() {
    this->salida->liberarLock();
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
    //this->semaforoGruasLibres->v();

    struct trabajo trabajo;
    int res = this->tareasAGruaLectura->leer(&trabajo,sizeof(trabajo));
    //if(res <= 0) Logger::getInstance()->log("Leyendo en tareasAGruaGrua",1);

    return trabajo;


}

void Controlador::descargarGrua(struct trabajo trabajo, pid_t pidTransporte){

    //Debo abrir un fifo para escribir la carga en el transporte
    std::string path = "/tmp/" + std::to_string(pidTransporte);

    //Creo el fifo
    this->cargaEscritura = new FifoEscritura(path);

    //Abro el fifo
    //Logger::getInstance()->log("Grua, antes de abrir carga",1);
    this->cargaEscritura->abrir();
    //Logger::getInstance()->log("Grua, despues de abrir carga",1);

    //Escribo la carga
    int res = this->cargaEscritura->escribir(&trabajo,sizeof(trabajo));
    //if(res <= 0) Logger::getInstance()->log("Escribiendo cargaEscrituraGrua",1);

    //TODO:DESTRUIR EL FIFO PARA QUE NO QUEDE ABIERTO, HAY QUE SABER QUE YA FUE LEIDO.


}


pid_t Controlador::tomarTransporteVacio(int transporte) {

    pid_t pidTransporte;
    if(transporte == BARCO) {
        //Debo leer del fifo de los barcos vacios
        int res = this->barcosVaciosLectura->leer(&pidTransporte,sizeof(pidTransporte));
        //if(res <= 0) Logger::getInstance()->log("Leyendo barcosVaciosGrua",1);
    }else{
        //Logger::getInstance()->log("Grua,antes de leer camiones vacios",1);
        int res = this->camionesVaciosLectura->leer(&pidTransporte,sizeof(pidTransporte));
        //Logger::getInstance()->log("Grua,despues de leer camiones vacios",1);
        //if(res <= 0) Logger::getInstance()->log("Leyendo camionesVaciosGrua",1);
        perror(NULL);
    }


    return pidTransporte;

}


void Controlador::adaptarseAGrua() {


    //TODO:CHECKEAR EL ORDEN
    this->tareasAGruaLectura->abrir();
    this->barcosVaciosLectura->abrir();
    this->camionesVaciosLectura->abrir();

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
    //this->semaforoGruasLibres->p(); //Las gruas haran el v();

    //Si hay gruas disponibles, debe escribir su trabajo a la cola de trabajos a gruas
    int res = this->tareasAGruaEscritura->escribir(&trabajo,sizeof(trabajo));
    //if(res <= 0) Logger::getInstance()->log("Escribiendo tareasAGruaCamion",1);
    //no deberia ser capaz de continuar si no hay un barco vacio
    //this->semaforoBarcosLibres->p(); //Un barco libre hara el v().

    //Si llega a este punto es porque hay una grua asignada y un barco vacio



}

void Controlador::agregarCamionAFlota(pid_t camionPid){

    //Primero hay que avisarle a los barcos que hay un camion libre
    //this->semaforoCamionesLibres->v();

    this->camionesVaciosEscritura->escribir(&camionPid,sizeof(camionPid));

    std::string path = "/tmp/" + std::to_string(camionPid);

    this->cargaLectura = new FifoLectura(path);
    //Logger::getInstance()->log("Camion, antes de abrir carga",1);
    this->cargaLectura->abrir();
    //Logger::getInstance()->log("Camion, despues de abrir carga",1);
}

struct trabajo Controlador::darCargaACamion() {

    struct trabajo trabajo;

    //Hay que leer del fifo de cargas
    int res = this->cargaLectura->leer(&trabajo,sizeof(trabajo));
    //if(res <= 0) Logger::getInstance()->log("Leyendo cargaCamion",1);
    perror(NULL);

    return trabajo;

}


void Controlador::adaptarseACamion() {


    //TODO:CHECKEAR EL ORDEN
    this->tareasAGruaEscritura->abrir();
    this->camionesVaciosEscritura->abrir();

}

float Controlador::valorCaja() {
    return smCaja->leer();
}

void Controlador::cargarCaja(float v) {
    float valor = valorCaja();
    valor += v;
    smCaja->escribir(valor);
}
