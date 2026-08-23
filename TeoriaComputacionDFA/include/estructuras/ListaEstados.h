#ifndef LISTAESTADOS_H
#define LISTAESTADOS_H

#include <string>

#include "estructuras/NodoEstado.h"

class ListaEstados {
private:
    NodoEstado* primero;

public:
    ListaEstados();
    ~ListaEstados();

    bool estaVacia() const;
    bool existe(const std::string& nombre) const;
    bool insertar(const std::string& nombre);
    bool eliminar(const std::string& nombre);
    int cantidad() const;
    void mostrar() const;
    const NodoEstado* obtenerPrimero() const;
};

#endif
