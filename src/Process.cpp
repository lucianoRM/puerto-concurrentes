//
// Created by Martin Gonzalez on 13/10/15.
//

#include "Process.h"

Process::Process():shouldRun(true) {}



void Process::destroy(Controlador *controlador) {

    controlador->destruirControlesEspecificos();
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
            this->run(controlador);
        }

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
