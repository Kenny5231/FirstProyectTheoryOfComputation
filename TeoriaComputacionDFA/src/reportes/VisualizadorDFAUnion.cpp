#include "reportes/VisualizadorDFAUnion.h"

#include <iomanip>
#include <iostream>

#include "estructuras/NodoEstadoCompuesto.h"
#include "estructuras/NodoSimbolo.h"
#include "estructuras/NodoTransicionCompuesta.h"

void VisualizadorDFAUnion::mostrarComponentes(const DFAUnion& dfaUnion) const {
    std::cout << "DFA UNION - COMPONENTES" << std::endl;
    std::cout << "Leyenda:" << std::endl;
    std::cout << "-> Estado inicial" << std::endl;
    std::cout << "*  Estado final" << std::endl;

    std::cout << "Estados QU:" << std::endl;
    const NodoEstadoCompuesto* estadoActual =
        dfaUnion.obtenerEstados().obtenerPrimero();

    while (estadoActual != nullptr) {
        bool inicial = esEstadoInicial(dfaUnion, estadoActual->estadoDFA1,
                                       estadoActual->estadoDFA2);
        bool esFinal = esEstadoFinal(dfaUnion, estadoActual->estadoDFA1,
                                     estadoActual->estadoDFA2);

        mostrarMarcador(inicial, esFinal);
        std::cout << "(" << estadoActual->estadoDFA1 << ","
                  << estadoActual->estadoDFA2 << ")" << std::endl;
        estadoActual = estadoActual->siguiente;
    }

    std::cout << "Alfabeto:" << std::endl;
    const NodoSimbolo* simboloActual =
        dfaUnion.obtenerAlfabeto().obtenerPrimero();

    while (simboloActual != nullptr) {
        std::cout << simboloActual->simbolo << std::endl;
        simboloActual = simboloActual->siguiente;
    }

    std::cout << "Estado inicial: ";
    if (dfaUnion.tieneEstadoInicial()) {
        std::cout << "(" << dfaUnion.obtenerEstadoInicialDFA1() << ","
                  << dfaUnion.obtenerEstadoInicialDFA2() << ")" << std::endl;
    } else {
        std::cout << "NO DEFINIDO" << std::endl;
    }

    std::cout << "Estados finales FU:" << std::endl;
    if (dfaUnion.obtenerEstadosFinales().estaVacia()) {
        std::cout << "FU = { }" << std::endl;
    } else {
        const NodoEstadoCompuesto* finalActual =
            dfaUnion.obtenerEstadosFinales().obtenerPrimero();

        while (finalActual != nullptr) {
            bool inicial = esEstadoInicial(dfaUnion, finalActual->estadoDFA1,
                                           finalActual->estadoDFA2);

            mostrarMarcador(inicial, true);
            std::cout << "(" << finalActual->estadoDFA1 << ","
                      << finalActual->estadoDFA2 << ")" << std::endl;
            finalActual = finalActual->siguiente;
        }
    }

    std::cout << "Cantidad de estados: " << dfaUnion.obtenerEstados().cantidad()
              << std::endl;
    std::cout << "Cantidad de simbolos: "
              << dfaUnion.obtenerAlfabeto().cantidad() << std::endl;
    std::cout << "Cantidad de estados finales: "
              << dfaUnion.obtenerEstadosFinales().cantidad() << std::endl;
    std::cout << "Cantidad de transiciones: "
              << dfaUnion.obtenerTransiciones().cantidad() << std::endl;
}

void VisualizadorDFAUnion::mostrarTablaTransiciones(
    const DFAUnion& dfaUnion) const {
    const int anchoEstado = 18;
    const int anchoColumna = 18;

    std::cout << "TABLA DE TRANSICIONES DFA UNION" << std::endl;
    std::cout << std::left << std::setw(anchoEstado) << "Estado";

    const NodoSimbolo* simboloEncabezado =
        dfaUnion.obtenerAlfabeto().obtenerPrimero();

    while (simboloEncabezado != nullptr) {
        std::cout << "| " << std::setw(anchoColumna)
                  << simboloEncabezado->simbolo;
        simboloEncabezado = simboloEncabezado->siguiente;
    }

    std::cout << std::endl;

    const NodoEstadoCompuesto* estadoActual =
        dfaUnion.obtenerEstados().obtenerPrimero();

    while (estadoActual != nullptr) {
        bool inicial = esEstadoInicial(dfaUnion, estadoActual->estadoDFA1,
                                       estadoActual->estadoDFA2);
        bool esFinal = esEstadoFinal(dfaUnion, estadoActual->estadoDFA1,
                                     estadoActual->estadoDFA2);
        std::string estado = "(" + estadoActual->estadoDFA1 + "," +
                             estadoActual->estadoDFA2 + ")";
        std::string estadoConMarcador;

        if (inicial && esFinal) {
            estadoConMarcador = "->* " + estado;
        } else if (inicial) {
            estadoConMarcador = "->  " + estado;
        } else if (esFinal) {
            estadoConMarcador = "*   " + estado;
        } else {
            estadoConMarcador = "    " + estado;
        }

        std::cout << std::left << std::setw(anchoEstado) << estadoConMarcador;

        const NodoSimbolo* simboloActual =
            dfaUnion.obtenerAlfabeto().obtenerPrimero();

        while (simboloActual != nullptr) {
            std::string destinoDFA1;
            std::string destinoDFA2;

            if (buscarDestino(dfaUnion, estadoActual->estadoDFA1,
                              estadoActual->estadoDFA2, simboloActual->simbolo,
                              destinoDFA1, destinoDFA2)) {
                std::string destino = "(" + destinoDFA1 + "," + destinoDFA2 + ")";
                std::cout << "| " << std::setw(anchoColumna) << destino;
            } else {
                std::cout << "| " << std::setw(anchoColumna) << "SIN TRANSICION";
            }

            simboloActual = simboloActual->siguiente;
        }

        std::cout << std::endl;
        estadoActual = estadoActual->siguiente;
    }
}

void VisualizadorDFAUnion::mostrarCompleto(const DFAUnion& dfaUnion) const {
    mostrarComponentes(dfaUnion);
    mostrarTablaTransiciones(dfaUnion);
}

bool VisualizadorDFAUnion::esEstadoInicial(
    const DFAUnion& dfaUnion,
    const std::string& estadoDFA1,
    const std::string& estadoDFA2) const {
    return dfaUnion.tieneEstadoInicial() &&
           dfaUnion.obtenerEstadoInicialDFA1() == estadoDFA1 &&
           dfaUnion.obtenerEstadoInicialDFA2() == estadoDFA2;
}

bool VisualizadorDFAUnion::esEstadoFinal(
    const DFAUnion& dfaUnion,
    const std::string& estadoDFA1,
    const std::string& estadoDFA2) const {
    return dfaUnion.obtenerEstadosFinales().existe(estadoDFA1, estadoDFA2);
}

bool VisualizadorDFAUnion::buscarDestino(const DFAUnion& dfaUnion,
                                         const std::string& origenDFA1,
                                         const std::string& origenDFA2,
                                         const std::string& simbolo,
                                         std::string& destinoDFA1,
                                         std::string& destinoDFA2) const {
    const NodoTransicionCompuesta* actual =
        dfaUnion.obtenerTransiciones().obtenerPrimero();

    while (actual != nullptr) {
        if (actual->origenDFA1 == origenDFA1 &&
            actual->origenDFA2 == origenDFA2 &&
            actual->simbolo == simbolo) {
            destinoDFA1 = actual->destinoDFA1;
            destinoDFA2 = actual->destinoDFA2;
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

void VisualizadorDFAUnion::mostrarMarcador(bool inicial, bool esFinal) const {
    if (inicial && esFinal) {
        std::cout << "->* ";
    } else if (inicial) {
        std::cout << "->  ";
    } else if (esFinal) {
        std::cout << "*   ";
    } else {
        std::cout << "    ";
    }
}
