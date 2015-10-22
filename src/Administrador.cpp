//
// Created by agu on 10/18/15.
//

#include "Administrador.h"

Administrador::Administrador(float logTime):Process("ADMINISTRADOR"){
    this->logTime = logTime;


}

void Administrador::initialize(Controlador *controlador) { }


void Administrador::run(Controlador *c) {

    Logger::getInstance()->log("[ADMIN] Iniciado");
    std::ostringstream ss;
    ss << c->valorCaja();
    std::string s = ss.str();
    Logger::getInstance()->log("[ADMIN] Valor de la caja: " + s);
    usleep(logTime);

}
