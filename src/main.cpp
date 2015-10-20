    //
// Created by luciano on 12/10/15.
//

#include "Barco.h"
#include "Controlador.h"
#include "Logger.h"
#include "C.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "Camion.h"
#include "Grua.h"
#include "Administrador.h"


int main(){

    //Valores por default
    int cantidadBarcos = C::barcos;
    int cantidadAmarres = C::lugares;
    int cantidadGruas = C::gruas;
    int cantidadCamiones = C::camiones;

    int cantidadProcesosHijos = cantidadBarcos + cantidadGruas + cantidadCamiones + 1;// + 1 = Administrador
    if(cantidadProcesosHijos < 4) Logger::getInstance()->log("[MAIN] El valor de los parametros no es suficiente  para realizar simulacion");

    else {


        Logger::getInstance()->log("\n\n\n*************NEW RUN*************");
        Logger::getInstance()->log("Soy el master y estoy empezando la joda");

        Controlador *controlador = new Controlador(cantidadAmarres);
        for (int i = 0; i < cantidadBarcos; i++) {
            Barco barco;
            barco.start(controlador);
        }

        for (int i = 0; i < cantidadGruas; i++) {
            Grua grua;
            grua.start(controlador);
        }

        for (int i = 0; i < cantidadCamiones; i++) {
            Camion camion;
            camion.start(controlador);
        }

        //Administrador admin;
        //admin.start(controlador);

        for (int i = 0; i < cantidadProcesosHijos - 1; i++) {
            wait(NULL);
        }

        controlador->destruir();
        delete controlador;

        Logger::getInstance()->log("Soy el master y termine");

    }
    Logger::destroy();
    return 0;
}
