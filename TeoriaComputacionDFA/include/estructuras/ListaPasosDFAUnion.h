#ifndef LISTAPASOSDFAUNION_H
#define LISTAPASOSDFAUNION_H

#include <string>

#include "estructuras/NodoPasoDFAUnion.h"

class ListaPasosDFAUnion {
private:
    NodoPasoDFAUnion* primero;

public:
    ListaPasosDFAUnion();
    ~ListaPasosDFAUnion();
    ListaPasosDFAUnion(const ListaPasosDFAUnion&) = delete;
    ListaPasosDFAUnion& operator=(const ListaPasosDFAUnion&) = delete;

    void agregarPaso(const std::string& origenDFA1,
                     const std::string& origenDFA2,
                     const std::string& simbolo,
                     const std::string& destinoDFA1,
                     const std::string& destinoDFA2);
    bool estaVacia() const;
    int cantidad() const;
    void limpiar();
    const NodoPasoDFAUnion* obtenerPrimero() const;
};

#endif
