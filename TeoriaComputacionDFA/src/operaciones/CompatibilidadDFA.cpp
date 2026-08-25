#include "operaciones/CompatibilidadDFA.h"

bool CompatibilidadDFA::compararAlfabetos(const DFA& dfa1, const DFA& dfa2,
                                          ListaErrores& errores) const {
    errores.limpiar();

    const NodoSimbolo* simboloDFA1 = dfa1.obtenerAlfabeto().obtenerPrimero();
    while (simboloDFA1 != nullptr) {
        if (!dfa2.obtenerAlfabeto().existe(simboloDFA1->simbolo)) {
            errores.insertar("El simbolo '" + simboloDFA1->simbolo +
                            "' existe en el DFA 1 pero no existe en el DFA 2.");
        }

        simboloDFA1 = simboloDFA1->siguiente;
    }

    const NodoSimbolo* simboloDFA2 = dfa2.obtenerAlfabeto().obtenerPrimero();
    while (simboloDFA2 != nullptr) {
        if (!dfa1.obtenerAlfabeto().existe(simboloDFA2->simbolo)) {
            errores.insertar("El simbolo '" + simboloDFA2->simbolo +
                            "' existe en el DFA 2 pero no existe en el DFA 1.");
        }

        simboloDFA2 = simboloDFA2->siguiente;
    }

    return errores.estaVacia();
}
