#include "simulacion/SimuladorDFA.h"

#include "estructuras/NodoSimboloCadena.h"
#include "estructuras/NodoTransicion.h"

bool SimuladorDFA::simular(const DFA& dfa,
                           const CadenaEntrada& cadena,
                           bool& aceptada,
                           std::string& estadoFinal) const {
    aceptada = false;
    std::string estadoActual = dfa.obtenerEstadoInicial();
    const NodoSimboloCadena* simboloActual = cadena.obtenerPrimero();

    while (simboloActual != nullptr) {
        if (!dfa.obtenerAlfabeto().existe(simboloActual->simbolo)) {
            return false;
        }

        std::string destino;
        if (!buscarDestino(dfa, estadoActual, simboloActual->simbolo, destino)) {
            return false;
        }

        estadoActual = destino;
        simboloActual = simboloActual->siguiente;
    }

    estadoFinal = estadoActual;
    aceptada = dfa.obtenerEstadosFinales().existe(estadoActual);
    return true;
}

bool SimuladorDFA::buscarDestino(const DFA& dfa,
                                 const std::string& origen,
                                 const std::string& simbolo,
                                 std::string& destino) const {
    const NodoTransicion* actual = dfa.obtenerTransiciones().obtenerPrimero();

    while (actual != nullptr) {
        if (actual->origen == origen && actual->simbolo == simbolo) {
            destino = actual->destino;
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}
