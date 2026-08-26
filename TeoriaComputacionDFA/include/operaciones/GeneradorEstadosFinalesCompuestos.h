#ifndef GENERADORESTADOSFINALESCOMPUESTOS_H
#define GENERADORESTADOSFINALESCOMPUESTOS_H

#include "automata/DFA.h"
#include "estructuras/ListaEstadosCompuestos.h"

class GeneradorEstadosFinalesCompuestos {
public:
    void generar(const DFA& dfa1, const DFA& dfa2,
                 const ListaEstadosCompuestos& estadosCompuestos,
                 ListaEstadosCompuestos& estadosFinales) const;
};

#endif
