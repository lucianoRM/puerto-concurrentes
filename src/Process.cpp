//
// Created by Martin Gonzalez on 13/10/15.
//

#include "Process.h"

Process::Process(std::string n):shouldRun(true) {
    this->name = n;
}

void Process::destroy(Controlador *controlador) {

    controlador->destruirControlesEspecificos();
}

void Process::log(std::string msg) {
    Logger::getInstance()->log("["+this->name+"] " + msg);
}

pid_t Process::start(Controlador* controlador) {
    pid_t pid = fork();

    if (pid == 0) {
        pid = getpid();

        // event handler para la senial SIGINT (-2)
        SIGINT_Handler sigint_handler;

        // se registra el event handler declarado antes
        SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );

        //Give the process a chance to init itself before running
        this->initialize(controlador);

        // mientras no se reciba la senial SIGINT, el proceso realiza su trabajo
        while (shouldRun && sigint_handler.getGracefulQuit() == 0 ) {
            try {
                this->run(controlador);
            } catch (std::exception& e) {
                Logger::getInstance()->log(e.what());
                if (!sigint_handler.getGracefulQuit()) {
                    shouldRun = false;
                }
            }
        }


        this->destroy(controlador);

        SignalHandler :: destruir ();

        if (!this->shouldRun) {
            Logger::getInstance()->log("El proceso termina por que quiso");
        } else {
            Logger::getInstance()->log("Se recibio la senial SIGINT, el proceso termina");
        }


        delete controlador;
        Logger::destroy();

        exit ( 0 );
    }
    return pid;
}

Process::~Process() {

}
