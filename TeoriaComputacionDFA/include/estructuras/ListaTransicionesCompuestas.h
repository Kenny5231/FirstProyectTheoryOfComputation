#ifndef LISTATRANSICIONESCOMPUESTAS_H
#define LISTATRANSICIONESCOMPUESTAS_H

#include <string>

#include "estructuras/NodoTransicionCompuesta.h"

class ListaTransicionesCompuestas {
private:
    NodoTransicionCompuesta* primero;

public:
    ListaTransicionesCompuestas();
    ~ListaTransicionesCompuestas();
    ListaTransicionesCompuestas(const ListaTransicionesCompuestas&) = delete;
    ListaTransicionesCompuestas& operator=(const ListaTransicionesCompuestas&) = delete;

    bool estaVacia() const;
    bool existeTransicionExacta(const std::string& origenDFA1,
                                const std::string& origenDFA2,
                                const std::string& simbolo,
                                const std::string& destinoDFA1,
                                const std::string& destinoDFA2) const;
    bool insertar(const std::string& origenDFA1,
                  const std::string& origenDFA2,
                  const std::string& simbolo,
                  const std::string& destinoDFA1,
                  const std::string& destinoDFA2);
    int cantidad() const;
    void mostrar() const;
    void limpiar();
    const NodoTransicionCompuesta* obtenerPrimero() const;
};

#endif
