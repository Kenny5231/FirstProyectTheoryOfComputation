#ifndef VISUALIZADORTRAZABILIDAD_H
#define VISUALIZADORTRAZABILIDAD_H

#include <string>

#include "automata/DFA.h"
#include "automata/DFAUnion.h"
#include "estructuras/ListaPasosDFA.h"
#include "estructuras/ListaPasosDFAUnion.h"

class VisualizadorTrazabilidad {
public:
    void mostrarDFA(const DFA& dfa,
                    const ListaPasosDFA& pasos,
                    bool procesable,
                    bool aceptada,
                    const std::string& estadoFinal) const;
    void mostrarDFAUnion(const DFAUnion& dfaUnion,
                         const ListaPasosDFAUnion& pasos,
                         bool procesable,
                         bool aceptada,
                         const std::string& estadoFinalDFA1,
                         const std::string& estadoFinalDFA2) const;
};

#endif
