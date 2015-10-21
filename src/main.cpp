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
    Logger::getInstance()->log("\n\n\n*************NEW RUN*************");
    Logger::getInstance()->log("Soy el master y estoy empezando la joda");

    //Valores por default
    int cantidadBarcos = C::barcos;
    int cantidadAmarres = C::lugares;
    int cantidadGruas = C::gruas;
    int cantidadCamiones = C::camiones;

    Logger::getInstance()->log("[MAIN] Hay: " +std::to_string(cantidadBarcos) + " barcos, " + std::to_string(cantidadAmarres) + " amarres, " + std::to_string(cantidadCamiones) + " camiones y " + std::to_string(cantidadGruas) + " gruas");

    int cantidadProcesosHijos = cantidadBarcos + cantidadGruas + cantidadCamiones + 1;// + 1 = Administrador

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
    std::vector<pid_t> hijos;
    hijos.insert(hijos.end(), barcos.begin(), barcos.end());
    hijos.insert(hijos.end(), gruas.begin(), gruas.end());
    hijos.insert(hijos.end(), camiones.begin(), camiones.end());

    esperarHijos("BARCO", barcos);

    esperarHijos("CAMION", camiones);

    terminarHijos("GRUA", gruas);


    controlador->destruir();
    delete controlador;

    Logger::getInstance()->log("Soy el master y termine");

    Logger::destroy();
    return 0;
}
