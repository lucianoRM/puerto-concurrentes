//
// Created by luciano on 12/10/15.
//

#include "Barco.h"
#include "Controlador.h"
#include "Logger.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

static int cantidadBarcos = 5;
static int cantidadAmarres = 1;

int main(){
    Logger::getInstance()->log("Soy el master y estoy empezando la joda");

    Controlador* controlador = new Controlador(cantidadAmarres);
    for(int i = 0;i < cantidadBarcos ;i++) {
        Barco barco;
        barco.start(controlador);
    }

    for(int i = 0;i < cantidadBarcos; i++){
        wait(NULL);
    }

    delete controlador;

    Logger::getInstance()->log("Soy el master y termine");
    return 0;
}