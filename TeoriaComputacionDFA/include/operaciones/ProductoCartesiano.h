#ifndef PRODUCTOCARTESIANO_H
#define PRODUCTOCARTESIANO_H

#include "automata/DFA.h"
#include "estructuras/ListaEstadosCompuestos.h"

class ProductoCartesiano {
public:
    void generar(const DFA& dfa1, const DFA& dfa2,
                 ListaEstadosCompuestos& resultado) const;
};

#endif
