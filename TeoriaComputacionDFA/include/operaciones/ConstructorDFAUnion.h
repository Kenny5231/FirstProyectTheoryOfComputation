#ifndef CONSTRUCTORDFAUNION_H
#define CONSTRUCTORDFAUNION_H

#include "automata/DFA.h"
#include "automata/DFAUnion.h"

class ConstructorDFAUnion {
public:
    bool construir(const DFA& dfa1, const DFA& dfa2, DFAUnion& resultado) const;
};

#endif
