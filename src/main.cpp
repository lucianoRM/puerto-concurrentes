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

static int cantidadBarcos = 1;
static int cantidadAmarres = 1;
static int cantidadGruas = 1;
static int cantidadCamiones = 1;
static int cantidadProcesosHijos = cantidadBarcos + cantidadGruas + cantidadCamiones + 1;// + 1 = Administrador

void esperarHijos(std::string nombre, std::vector<pid_t> hijos) {
    std::vector<pid_t>::iterator it = hijos.begin();
    for (;it != hijos.end(); it++) {
        pid_t pid = *it;
        Logger::getInstance()->log("Soy el master y estoy esperando al [" + nombre + " : " + std::to_string(pid) + "]");
        waitpid( pid, NULL, 0 );
    }
}

void terminarHijos(std::string nombre, std::vector<pid_t> hijos) {
    std::vector<pid_t>::iterator it = hijos.begin();
    for (;it != hijos.end(); it++) {
        pid_t pid = *it;
        Logger::getInstance()->log("Soy el master y estoy terminando a [" + nombre + " : " + std::to_string(pid) + "]");
        kill(pid, SIGINT);
    }
}

int main(){
    Logger::getInstance()->log("Soy el master y estoy empezando la joda");

    std::vector<pid_t> barcos;
    std::vector<pid_t> gruas;
    std::vector<pid_t> camiones;

    Controlador* controlador = new Controlador(cantidadAmarres);
    for(int i = 0;i < cantidadBarcos ;i++) {
        Barco barco;
        pid_t pid = barco.start(controlador);
        barcos.push_back(pid);
    }

    for(int i=0;i < cantidadGruas; i++){
        Grua grua;
        pid_t pid = grua.start(controlador);
        gruas.push_back(pid);
    }

    for(int i=0;i < cantidadCamiones; i++){
        Camion camion;
        pid_t pid = camion.start(controlador);
        camiones.push_back(pid);
    }

    //Administrador admin;
    //admin.start(controlador);
    Logger::getInstance()->log("Soy el master y estoy esperando a los barcos");
    esperarHijos("BARCO", barcos);

    Logger::getInstance()->log("Soy el master y voy a terminar a las gruas");
    terminarHijos("GRUA", gruas);

    Logger::getInstance()->log("Soy el master y voy a terminar a los camiones");
    terminarHijos("CAMION", camiones);

    controlador->destruir();
    delete controlador;

    Logger::getInstance()->log("Soy el master y termine");
    Logger::destroy();

    std::cout << C::barcos;
    return 0;
}
