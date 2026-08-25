#ifndef NODOESTADOCOMPUESTO_H
#define NODOESTADOCOMPUESTO_H

#include <string>

struct NodoEstadoCompuesto {
    std::string estadoDFA1;
    std::string estadoDFA2;
    NodoEstadoCompuesto* siguiente;

    NodoEstadoCompuesto(const std::string& estado1, const std::string& estado2)
        : estadoDFA1(estado1), estadoDFA2(estado2), siguiente(nullptr) {}
};

#endif
