//
// Created by luciano on 12/10/15.
//

#include "Grua.h"


void Grua::run(Controlador* controlador){

    //controlador->abrir();
    struct trabajo trabajo;
    std::string transporte;
    //while(true){

        trabajo = controlador->asignarTrabajoAGrua();

        Logger::getInstance()->log("[GRUA] Tome la carga del " + transporte + ": " + std::to_string(trabajo.proceso) + ", tenia carga de: " + std::to_string(trabajo.carga));

        if(trabajo.tipoTrabajo == DESCARGAR_CAMION){
            transporte = "CAMION";
        }else{
            transporte = "BARCO";
        }
        Logger::getInstance()->log("[GRUA] Tome la carga del " + transporte + ": " + std::to_string(trabajo.proceso) + ", tenia carga de: " + std::to_string(trabajo.carga));
        //controlador->descargarGrua(trabajo);




}
