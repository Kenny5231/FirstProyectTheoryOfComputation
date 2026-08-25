#ifndef NODOERROR_H
#define NODOERROR_H

#include <string>

struct NodoError {
    std::string mensaje;
    NodoError* siguiente;

    explicit NodoError(const std::string& mensajeError)
        : mensaje(mensajeError), siguiente(nullptr) {}
};

#endif
