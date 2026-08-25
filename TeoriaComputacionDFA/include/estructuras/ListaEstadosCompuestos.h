#ifndef LISTAESTADOSCOMPUESTOS_H
#define LISTAESTADOSCOMPUESTOS_H

#include <string>

#include "estructuras/NodoEstadoCompuesto.h"

class ListaEstadosCompuestos {
private:
    NodoEstadoCompuesto* primero;

public:
    ListaEstadosCompuestos();
    ~ListaEstadosCompuestos();
    ListaEstadosCompuestos(const ListaEstadosCompuestos&) = delete;
    ListaEstadosCompuestos& operator=(const ListaEstadosCompuestos&) = delete;

    bool estaVacia() const;
    bool existe(const std::string& estadoDFA1, const std::string& estadoDFA2) const;
    bool insertar(const std::string& estadoDFA1, const std::string& estadoDFA2);
    int cantidad() const;
    void mostrar() const;
    void limpiar();
    const NodoEstadoCompuesto* obtenerPrimero() const;
};

#endif
