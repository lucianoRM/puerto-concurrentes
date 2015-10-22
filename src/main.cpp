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
#include "Main.h"
#include "SIGINT_Handler_main.h"

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

    //int cantidadProcesosHijos = cantidadBarcos + cantidadGruas + cantidadCamiones + 1;// + 1 = Administrador

    std::vector<pid_t> barcos;
    std::vector<pid_t> gruas;
    std::vector<pid_t> camiones;

    std::vector<pid_t> hijos;

    Controlador* controlador = new Controlador(cantidadAmarres);
    for(int i = 0;i < cantidadBarcos ;i++) {
        Barco barco;
        pid_t pid = barco.start(controlador);
        if (getpid()==pid) return 0;
        barcos.push_back(pid);
    }

    for(int i=0;i < cantidadGruas; i++){
        Grua grua;
        pid_t pid = grua.start(controlador);
        if (getpid()==pid) return 0;
        gruas.push_back(pid);
    }

    for(int i=0;i < cantidadCamiones; i++){
        Camion camion;
        pid_t pid = camion.start(controlador);
        if (getpid()==pid) return 0;
        camiones.push_back(pid);
    }

    //Administrador admin;
    //admin.start(controlador);
    hijos.insert(hijos.end(), barcos.begin(), barcos.end());
    hijos.insert(hijos.end(), gruas.begin(), gruas.end());
    hijos.insert(hijos.end(), camiones.begin(), camiones.end());

    // event handler para la senial SIGINT (-2)
    SIGINT_Handler_main sigint_handler(hijos);

    // se registra el event handler declarado antes
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );

    esperarHijos("BARCO", barcos);

    esperarHijos("CAMION", camiones);

    esperarHijos("GRUA", gruas);

    controlador->destruir();

    delete controlador;

    SignalHandler :: destruir ();

    Logger::getInstance()->log("Soy el master y termine");

    Logger::destroy();

    return 0;
}

/*int main2() {
    Logger::getInstance()->log("\n\n\n*************NEW RUN*************");
    Logger::getInstance()->log("Soy el master y estoy empezando la joda");

    Main main;

    main.start(NULL);
    return 0;
}
*/