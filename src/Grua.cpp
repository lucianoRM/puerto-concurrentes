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
    Logger::getInstance()->log("[GRUA] Voy a empezar mi trabajo");

    trabajo = controlador->asignarTrabajoAGrua();

    if (trabajo.tipoTrabajo == DESCARGAR_CAMION) {
        transporte = "CAMION";
    } else {
        transporte = "BARCO";
    }

    Logger::getInstance()->log("[GRUA] Tome la carga del " + transporte + ": " + std::to_string(trabajo.proceso) + ", tenia carga de: " + std::to_string(trabajo.carga));

    pid_t pidTransporte = controlador->tomarTransporteVacio(1 - trabajo.tipoTrabajo);

    Logger::getInstance()->log("[GRUA] Tengo asignado a: " + std::to_string(pidTransporte) + " para cargarlo");

    controlador->descargarGrua(trabajo,pidTransporte);
}
