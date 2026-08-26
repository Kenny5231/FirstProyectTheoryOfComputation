#ifndef NODOPOSICIONESTADOCOMPUESTO_H
#define NODOPOSICIONESTADOCOMPUESTO_H

#include <string>

struct NodoPosicionEstadoCompuesto {
    std::string estadoDFA1;
    std::string estadoDFA2;
    double x;
    double y;
    NodoPosicionEstadoCompuesto* siguiente;

    NodoPosicionEstadoCompuesto(const std::string& estado1, const std::string& estado2,
                                double posicionX, double posicionY)
        : estadoDFA1(estado1), estadoDFA2(estado2), x(posicionX), y(posicionY), siguiente(nullptr) {}
};

#endif
