#ifndef VISUALIZADORVEREDICTOTRIPLE_H
#define VISUALIZADORVEREDICTOTRIPLE_H

#include "estructuras/CadenaEntrada.h"
#include "simulacion/ResultadoTriple.h"

class VisualizadorVeredictoTriple {
public:
    void mostrar(const CadenaEntrada& cadena,
                 const ResultadoTriple& resultado) const;
};

#endif
