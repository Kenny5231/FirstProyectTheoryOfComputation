#ifndef NODOPOSICIONESTADO_H
#define NODOPOSICIONESTADO_H

#include <string>

struct NodoPosicionEstado {
    std::string estado;
    double x;
    double y;
    NodoPosicionEstado* siguiente;

    NodoPosicionEstado(const std::string& nombre, double posicionX, double posicionY)
        : estado(nombre), x(posicionX), y(posicionY), siguiente(nullptr) {}
};

#endif
