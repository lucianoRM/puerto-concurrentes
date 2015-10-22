//
// Created by luciano on 12/10/15.
//

#include "Grua.h"

void Grua::initialize(Controlador* controlador) {
    controlador->adaptarseAGrua();
}

void Grua::run(Controlador* controlador){

    struct trabajo trabajo;
    std::string transporte;
    std::string destino;
    Logger::getInstance()->log("[GRUA] Voy a empezar mi trabajo");

    trabajo = controlador->asignarTrabajoAGrua();

    if (trabajo.tipoTrabajo == DESCARGAR_CAMION) {
        transporte = "CAMION";
        destino = "BARCO";
    } else {
        transporte = "BARCO";
        destino = "CAMION";
    }

    Logger::getInstance()->log("[GRUA] Tome la carga del " + transporte + ": " + std::to_string(trabajo.proceso) + ", tenia carga de: " + std::to_string(trabajo.carga));

    pid_t pidTransporte = controlador->tomarTransporteVacio(1 - trabajo.tipoTrabajo);



    Logger::getInstance()->log("[GRUA] Tengo asignado a " + destino + ": " + std::to_string(pidTransporte) + " para cargarlo");

    controlador->descargarGrua(trabajo,pidTransporte);
}
