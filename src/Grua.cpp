//
// Created by luciano on 12/10/15.
//

#include "Grua.h"


void Grua::run(Controlador* controlador){

    struct trabajo trabajo;
    std::string transporte;
    while(true){

        Logger::getInstance()->log("[GRUA] Tome la carga del " + transporte + ": " + std::to_string(trabajo.proceso) + ", tenia carga de: " + std::to_string(trabajo.carga));

        trabajo = controlador->asignarTrabajoAGrua();
        if(trabajo.tipoTrabajo == DESCARGAR_CAMION){
            transporte = "CAMION";
        }else{
            transporte = "BARCO";
        }
        Logger::getInstance()->log("[GRUA] Tome la carga del " + transporte + ": " + std::to_string(trabajo.proceso) + ", tenia carga de: " + std::to_string(trabajo.carga));
        controlador->descargarGrua(trabajo);

    }


}
