#ifndef LISTAPOSICIONESESTADOS_H
#define LISTAPOSICIONESESTADOS_H

#include "estructuras/NodoPosicionEstado.h"

class ListaPosicionesEstados {
private:
    NodoPosicionEstado* primero;

public:
    ListaPosicionesEstados();
    ~ListaPosicionesEstados();
    ListaPosicionesEstados(const ListaPosicionesEstados&) = delete;
    ListaPosicionesEstados& operator=(const ListaPosicionesEstados&) = delete;

    void limpiar();
    void agregar(const std::string& estado, double x, double y);
    const NodoPosicionEstado* buscar(const std::string& estado) const;
    const NodoPosicionEstado* obtenerPrimero() const;
    int cantidad() const;
};

#endif
