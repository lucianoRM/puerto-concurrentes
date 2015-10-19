//
// Created by luciano on 12/10/15.
//

#include "Grua.h"


void Grua::run(Controlador* controlador){

    controlador->adaptarseAGrua();
    struct trabajo trabajo;
    std::string transporte;
    Logger::getInstance()->log("[GRUA] Voy a empezar mi trabajo");
    //while(true) {

        trabajo = controlador->asignarTrabajoAGrua();

        if (trabajo.tipoTrabajo == DESCARGAR_CAMION) {
            transporte = "CAMION";
        } else {
            transporte = "BARCO";
        }
        Logger::getInstance()->log("[GRUA] Tome la carga del " + transporte + ": " + std::to_string(trabajo.proceso) + ", tenia carga de: " + std::to_string(trabajo.carga));

        pid_t pidTransporte = controlador->tomarTransporteVacio(trabajo.tipoTrabajo - 1);

        Logger::getInstance()->log("[GRUA] Tengo asignado a: " + std::to_string(pidTransporte) + " para cargarlo");

        controlador->descargarGrua(trabajo,pidTransporte);

    //}

    this->shouldRun = false;
}
