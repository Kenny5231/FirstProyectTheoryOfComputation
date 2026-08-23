#ifndef NODOESTADO_H
#define NODOESTADO_H

#include <string>

struct NodoEstado {
    std::string nombre;
    NodoEstado* siguiente;

    explicit NodoEstado(const std::string& nombreEstado)
        : nombre(nombreEstado), siguiente(nullptr) {}
};

#endif
