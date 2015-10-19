//
// Created by agu on 10/18/15.
//

#include "Administrador.h"

void Administrador::run(Controlador *c) {
    Logger* l = Logger::getInstance();
    l->log("[ADMIN] Iniciado");
    while (true) {
        std::ostringstream ss;
        ss << c->valorCaja();
        std::string s = ss.str();
        l->log("[ADMIN] Valor de la caja: " + s);
        sleep(3);
    }
}
