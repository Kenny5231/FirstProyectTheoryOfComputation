#ifndef SIMULADORDFA_H
#define SIMULADORDFA_H

#include <string>

#include "automata/DFA.h"
#include "estructuras/CadenaEntrada.h"

class SimuladorDFA {
private:
    bool buscarDestino(const DFA& dfa,
                       const std::string& origen,
                       const std::string& simbolo,
                       std::string& destino) const;

public:
    bool simular(const DFA& dfa,
                 const CadenaEntrada& cadena,
                 bool& aceptada,
                 std::string& estadoFinal) const;
};

#endif
