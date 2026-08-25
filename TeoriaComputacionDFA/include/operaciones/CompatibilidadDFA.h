#ifndef COMPATIBILIDADDFA_H
#define COMPATIBILIDADDFA_H

#include "automata/DFA.h"
#include "validacion/ListaErrores.h"

class CompatibilidadDFA {
public:
    bool compararAlfabetos(const DFA& dfa1, const DFA& dfa2,
                           ListaErrores& errores) const;
};

#endif
