//
// Created by luciano on 12/10/15.
//

#include "Barco.h"
#include "Controlador.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

static int cantidadBarcos = 10;
static int cantidadAmarres = 5;


int main(){

    Controlador* controlador = new Controlador(cantidadAmarres);
    for(int i = 0;i < cantidadBarcos ;i++) {
        pid_t pid = fork();
        if (pid == 0) {
            Barco *barco = new Barco();
            barco->amarrar(controlador);
            controlador->atenderBarcoAmarrado(barco->getTrabajo());
            delete barco;
            exit(0);
        }
    }
    for(int i = 0;i < cantidadBarcos; i++){
        wait(NULL);
    }
    delete controlador;
    std::cout << getpid() << ": Termine" <<std::endl;
    return 0;
}