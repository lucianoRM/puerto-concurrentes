//
// Created by martin on 20/10/15.
//

#include "Main.h"

Main::Main():Process("MAIN") {}

Main::~Main() {}

void Main::initialize(Controlador *c) {

}

void Main::esperarHijos(std::string nombre, std::vector<pid_t> hijos) {
    std::vector<pid_t>::iterator it = hijos.begin();
    for (;it != hijos.end(); it++) {
        pid_t pid = *it;
        this->log("Soy el master y estoy esperando al [" + nombre + " : " + std::to_string(pid) + "]");
        waitpid( pid, NULL, 0 );
    }
}

void Main::terminarHijos(std::string nombre, std::vector<pid_t> hijos) {
    std::vector<pid_t>::iterator it = hijos.begin();
    for (;it != hijos.end(); it++) {
        pid_t pid = *it;
        this->log("Soy el master y estoy terminando a [" + nombre + " : " + std::to_string(pid) + "]");
        kill(pid, SIGINT);
    }
}

void Main::run(Controlador *c) {

    //Valores por default
    int cantidadBarcos = C::barcos;
    int cantidadAmarres = C::lugares;
    int cantidadGruas = C::gruas;
    int cantidadCamiones = C::camiones;

    this->log("[MAIN] Hay: " +std::to_string(cantidadBarcos) + " barcos, " + std::to_string(cantidadAmarres) + " amarres, " + std::to_string(cantidadCamiones) + " camiones y " + std::to_string(cantidadGruas) + " gruas");

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

    this->log("Soy el master y termine");

    this->shouldRun = false;

}
