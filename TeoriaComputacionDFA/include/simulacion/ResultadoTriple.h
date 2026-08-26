#ifndef RESULTADOTRIPLE_H
#define RESULTADOTRIPLE_H

#include <string>

struct ResultadoTriple {
    bool procesable;
    bool aceptadaDFA1;
    bool aceptadaDFA2;
    bool aceptadaUnion;
    bool unionConsistente;
    std::string estadoFinalDFA1;
    std::string estadoFinalDFA2;
    std::string estadoFinalUnionDFA1;
    std::string estadoFinalUnionDFA2;
    std::string simboloInvalido;

    ResultadoTriple()
        : procesable(false), aceptadaDFA1(false), aceptadaDFA2(false),
          aceptadaUnion(false), unionConsistente(false), estadoFinalDFA1(""),
          estadoFinalDFA2(""), estadoFinalUnionDFA1(""),
          estadoFinalUnionDFA2(""), simboloInvalido("") {}
};

#endif
