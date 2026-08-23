#ifndef LISTAERRORES_H
#define LISTAERRORES_H

#include <string>

#include "validacion/NodoError.h"

class ListaErrores {
private:
    NodoError* primero;

public:
    ListaErrores();
    ~ListaErrores();
    ListaErrores(const ListaErrores&) = delete;
    ListaErrores& operator=(const ListaErrores&) = delete;

    void insertar(const std::string& mensaje);
    bool estaVacia() const;
    int cantidad() const;
    void mostrar() const;
    void limpiar();
    const NodoError* obtenerPrimero() const;
};

#endif
