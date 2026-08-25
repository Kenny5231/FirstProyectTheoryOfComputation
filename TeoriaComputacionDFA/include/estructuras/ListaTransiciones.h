#ifndef LISTATRANSICIONES_H
#define LISTATRANSICIONES_H

#include <string>

#include "estructuras/NodoTransicion.h"

class ListaTransiciones {
private:
    NodoTransicion* primero;

public:
    ListaTransiciones();
    ~ListaTransiciones();
    ListaTransiciones(const ListaTransiciones&) = delete;
    ListaTransiciones& operator=(const ListaTransiciones&) = delete;

    bool estaVacia() const;
    bool insertar(const std::string& origen, const std::string& simbolo,
                  const std::string& destino);
    bool existeTransicionExacta(const std::string& origen, const std::string& simbolo,
                                const std::string& destino) const;
    int cantidadPorPar(const std::string& origen, const std::string& simbolo) const;
    int cantidad() const;
    bool eliminarTransicionExacta(const std::string& origen, const std::string& simbolo,
                                  const std::string& destino);
    void mostrar() const;
    const NodoTransicion* obtenerPrimero() const;
};

#endif
