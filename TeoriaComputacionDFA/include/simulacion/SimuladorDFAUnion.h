#ifndef SIMULADORDFAUNION_H
#define SIMULADORDFAUNION_H

#include <string>

#include "automata/DFAUnion.h"
#include "estructuras/CadenaEntrada.h"

class SimuladorDFAUnion {
private:
    bool buscarDestino(const DFAUnion& dfaUnion,
                       const std::string& origenDFA1,
                       const std::string& origenDFA2,
                       const std::string& simbolo,
                       std::string& destinoDFA1,
                       std::string& destinoDFA2) const;

public:
    bool simular(const DFAUnion& dfaUnion,
                 const CadenaEntrada& cadena,
                 bool& aceptada,
                 std::string& estadoFinalDFA1,
                 std::string& estadoFinalDFA2) const;
};

#endif
