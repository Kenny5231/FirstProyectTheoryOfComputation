#ifndef EVALUADORTRIPLE_H
#define EVALUADORTRIPLE_H

#include <string>

#include "automata/DFA.h"
#include "automata/DFAUnion.h"
#include "estructuras/CadenaEntrada.h"
#include "estructuras/ListaSimbolos.h"
#include "simulacion/ResultadoTriple.h"

class EvaluadorTriple {
private:
    bool buscarPrimerSimboloFueraDelAlfabeto(
        const CadenaEntrada& cadena,
        const ListaSimbolos& alfabeto,
        std::string& simboloInvalido) const;

public:
    bool evaluar(const DFA& dfa1,
                 const DFA& dfa2,
                 const DFAUnion& dfaUnion,
                 const CadenaEntrada& cadena,
                 ResultadoTriple& resultado) const;
};

#endif
