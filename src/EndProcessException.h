//
// Created by martin on 20/10/15.
//

#ifndef CONCU_ENDPROCESSEXCEPTION_H
#define CONCU_ENDPROCESSEXCEPTION_H

#include <iostream>
#include <exception>

class EndProcessException: public std::exception {
    virtual const char* what() const throw() {
        return "El processo debe terminar ahora";
    }
};
#endif //CONCU_ENDPROCESSEXCEPTION_H
