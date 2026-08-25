#ifndef GENERADORTRANSICIONESCOMPUESTAS_H
#define GENERADORTRANSICIONESCOMPUESTAS_H

#include <string>

#include "automata/DFA.h"
#include "estructuras/ListaEstadosCompuestos.h"
#include "estructuras/ListaTransicionesCompuestas.h"

class GeneradorTransicionesCompuestas {
public:
    bool generar(const DFA& dfa1, const DFA& dfa2,
                 const ListaEstadosCompuestos& estadosCompuestos,
                 ListaTransicionesCompuestas& resultado) const;

private:
    bool buscarDestino(const ListaTransiciones& transiciones,
                      const std::string& origen,
                      const std::string& simbolo,
                      std::string& destino) const;
};

#endif
