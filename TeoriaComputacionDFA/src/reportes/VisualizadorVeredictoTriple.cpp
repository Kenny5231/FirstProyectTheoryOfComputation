#include "reportes/VisualizadorVeredictoTriple.h"

#include <iostream>

void VisualizadorVeredictoTriple::mostrar(
    const CadenaEntrada& cadena,
    const ResultadoTriple& resultado) const {
    std::cout << "========================================" << std::endl;
    std::cout << "VEREDICTO DE CADENA" << std::endl;
    std::cout << "===================" << std::endl;
    std::cout << "Cadena:" << std::endl;
    cadena.mostrar();

    if (!resultado.procesable) {
        std::cout << "CADENA NO PROCESABLE" << std::endl;
        std::cout << "Simbolo fuera del alfabeto:" << std::endl;
        if (resultado.simboloInvalido == "") {
            std::cout << "NO IDENTIFICADO" << std::endl;
        } else {
            std::cout << resultado.simboloInvalido << std::endl;
        }
        return;
    }

    std::cout << "DFA 1:" << std::endl;
    std::cout << (resultado.aceptadaDFA1 ? "ACEPTADA" : "RECHAZADA")
              << std::endl;
    std::cout << "Estado alcanzado:" << std::endl;
    std::cout << resultado.estadoFinalDFA1 << std::endl;

    std::cout << "DFA 2:" << std::endl;
    std::cout << (resultado.aceptadaDFA2 ? "ACEPTADA" : "RECHAZADA")
              << std::endl;
    std::cout << "Estado alcanzado:" << std::endl;
    std::cout << resultado.estadoFinalDFA2 << std::endl;

    std::cout << "DFA UNION:" << std::endl;
    std::cout << (resultado.aceptadaUnion ? "ACEPTADA" : "RECHAZADA")
              << std::endl;
    std::cout << "Estado alcanzado:" << std::endl;
    std::cout << "(" << resultado.estadoFinalUnionDFA1 << ","
              << resultado.estadoFinalUnionDFA2 << ")" << std::endl;

    std::cout << "Comprobacion de union:" << std::endl;
    std::cout << (resultado.unionConsistente ? "CORRECTA" : "INCORRECTA")
              << std::endl;
    std::cout << "DFA Union = DFA1 OR DFA2" << std::endl;
}
