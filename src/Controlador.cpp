//
// Created by luciano on 10/10/15.
//


#include "Controlador.h"

Controlador::Controlador(int cantidadAmarres) {

    //Creamos los archivos que se van a usar para los controles de concurrencia
    int result = 0;
    result+=mknod(semaforoAmarresFile,S_IFREG|0777,0);
    result+=mknod(semaforoGruasFile,S_IFREG|0777,0);
    result+=mknod(semaforoCamionesFile,S_IFREG|0777,0);
    result+=mknod(lockEntradaFile,S_IFREG|0777,0);
    result+=mknod(cargasACamionesFile,S_IFREG|0777,0);
    result+=mknod(cargasABarcosFile,S_IFREG|0777,0);

    if(result < 0) Logger::getInstance()->log("[CONTROLADOR] Error al crear archivos para metodos de concurrencia");

    this->semaforoAmarres = new Semaforo(semaforoAmarresFile,cantidadAmarres);
    this->entrada = new LockFile(lockEntradaFile);
    this->tareasAGrua = new FifoEscritura(tareasGruasFile);
    this->tareasGruaPendientes = new FifoLectura(tareasGruasFile);
    this->cargasABarcos = new FifoEscritura(cargasABarcosFile);
    this->cargasDeBarcos = new FifoLectura(cargasACamionesFile);
    this->cargasACamiones = new FifoEscritura(cargasACamionesFile);
    this->cargasDeCamiones = new FifoLectura(cargasABarcosFile);




}
Controlador::~Controlador() {

    this->semaforoAmarres->eliminar();
    delete this->semaforoAmarres;
    delete this->entrada;
    this->tareasAGrua->eliminar();
    this->tareasGruaPendientes->eliminar();
    this->cargasABarcos->eliminar();
    this->cargasACamiones->eliminar();
    this->cargasDeBarcos->eliminar();
    this->cargasDeCamiones->eliminar();
    delete this->cargasABarcos;
    delete this->cargasDeCamiones;
    delete this->cargasDeBarcos;
    delete this->cargasACamiones;
    delete this->tareasAGrua;
    delete this->tareasGruaPendientes;
    unlink(semaforoAmarresFile);
    unlink(semaforoCamionesFile);
    unlink(semaforoGruasFile);
    unlink(lockEntradaFile);
    unlink(cargasABarcosFile);
    unlink(cargasACamionesFile);


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

    //TODO: SI EL BARCO NO TIENE TRABAJO DE DESCARGA FIJARSE SI HAY ALGUN CAMION Y SINO DEJARLO IRSE
    this->tareasAGrua->escribir(&trabajo,sizeof(trabajo));

}


void Controlador::agregarBarcoAFlota(){

    struct trabajo trabajo;
    this->cargasDeCamiones->leer(&trabajo,sizeof(trabajo));
    //TODO:realizar envio, logear la carga.

}


/*##################################################################################################
 * #################################################################################################
 *
 *                                  BARCOS
 *
 * ##################################################################################################
 * ###################################################################################################
 */



void Controlador::asignarTrabajoAGrua(){

    struct trabajo trabajo;
    this->tareasGruaPendientes->leer(&trabajo,sizeof(trabajo));

    //TODO:LEER EL TRABAJO Y ESCRIBIR EN EL FIFO CORRESPONDIENTE


}


void Controlador::liberarGrua(){

    this->semaforoGruas->v();

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

    this->tareasAGrua->escribir(&trabajo,sizeof(trabajo));

}
void Controlador::agregarCamionAFlota(){

    struct trabajo trabajo;
    this->cargasDeBarcos->leer(&trabajo,sizeof(trabajo));
    //TODO:realizar envio, logear la carga.

}
