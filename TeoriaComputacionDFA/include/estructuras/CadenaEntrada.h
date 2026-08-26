#ifndef CADENAENTRADA_H
#define CADENAENTRADA_H

#include <string>

#include "estructuras/NodoSimboloCadena.h"

class CadenaEntrada {
private:
    NodoSimboloCadena* primero;

public:
    CadenaEntrada();
    ~CadenaEntrada();
    CadenaEntrada(const CadenaEntrada&) = delete;
    CadenaEntrada& operator=(const CadenaEntrada&) = delete;

    void agregarSimbolo(const std::string& simbolo);
    bool estaVacia() const;
    int cantidad() const;
    void limpiar();
    const NodoSimboloCadena* obtenerPrimero() const;
    void mostrar() const;
};

#endif
