#ifndef VISUALIZADORDFAUNION_H
#define VISUALIZADORDFAUNION_H

#include <string>

#include "automata/DFAUnion.h"

class VisualizadorDFAUnion {
private:
    bool esEstadoInicial(const DFAUnion& dfaUnion,
                         const std::string& estadoDFA1,
                         const std::string& estadoDFA2) const;
    bool esEstadoFinal(const DFAUnion& dfaUnion,
                       const std::string& estadoDFA1,
                       const std::string& estadoDFA2) const;
    bool buscarDestino(const DFAUnion& dfaUnion,
                       const std::string& origenDFA1,
                       const std::string& origenDFA2,
                       const std::string& simbolo,
                       std::string& destinoDFA1,
                       std::string& destinoDFA2) const;
    void mostrarMarcador(bool inicial, bool esFinal) const;

public:
    void mostrarComponentes(const DFAUnion& dfaUnion) const;
    void mostrarTablaTransiciones(const DFAUnion& dfaUnion) const;
    void mostrarCompleto(const DFAUnion& dfaUnion) const;
};

#endif
