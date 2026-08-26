#ifndef LISTAPASOSDFA_H
#define LISTAPASOSDFA_H

#include <string>

#include "estructuras/NodoPasoDFA.h"

class ListaPasosDFA {
private:
    NodoPasoDFA* primero;

public:
    ListaPasosDFA();
    ~ListaPasosDFA();
    ListaPasosDFA(const ListaPasosDFA&) = delete;
    ListaPasosDFA& operator=(const ListaPasosDFA&) = delete;

    void agregarPaso(const std::string& origen,
                     const std::string& simbolo,
                     const std::string& destino);
    bool estaVacia() const;
    int cantidad() const;
    void limpiar();
    const NodoPasoDFA* obtenerPrimero() const;
};

#endif
