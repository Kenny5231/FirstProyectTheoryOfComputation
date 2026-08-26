#ifndef LISTAPOSICIONESESTADOSCOMPUESTOS_H
#define LISTAPOSICIONESESTADOSCOMPUESTOS_H

#include "estructuras/NodoPosicionEstadoCompuesto.h"

class ListaPosicionesEstadosCompuestos {
private:
    NodoPosicionEstadoCompuesto* primero;

public:
    ListaPosicionesEstadosCompuestos();
    ~ListaPosicionesEstadosCompuestos();
    ListaPosicionesEstadosCompuestos(const ListaPosicionesEstadosCompuestos&) = delete;
    ListaPosicionesEstadosCompuestos& operator=(const ListaPosicionesEstadosCompuestos&) = delete;

    void limpiar();
    void agregar(const std::string& estadoDFA1, const std::string& estadoDFA2, double x, double y);
    const NodoPosicionEstadoCompuesto* buscar(const std::string& estadoDFA1, const std::string& estadoDFA2) const;
    const NodoPosicionEstadoCompuesto* obtenerPrimero() const;
    int cantidad() const;
};

#endif
