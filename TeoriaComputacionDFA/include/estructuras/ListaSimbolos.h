#ifndef LISTASIMBOLOS_H
#define LISTASIMBOLOS_H

#include <string>

#include "estructuras/NodoSimbolo.h"

class ListaSimbolos {
private:
    NodoSimbolo* primero;

public:
    ListaSimbolos();
    ~ListaSimbolos();
    ListaSimbolos(const ListaSimbolos&) = delete;
    ListaSimbolos& operator=(const ListaSimbolos&) = delete;

    bool estaVacia() const;
    bool existe(const std::string& simbolo) const;
    bool insertar(const std::string& simbolo);
    bool eliminar(const std::string& simbolo);
    int cantidad() const;
    void mostrar() const;
    const NodoSimbolo* obtenerPrimero() const;
};

#endif
