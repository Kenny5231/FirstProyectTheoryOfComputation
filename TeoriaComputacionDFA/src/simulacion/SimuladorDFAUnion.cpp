#include "simulacion/SimuladorDFAUnion.h"

#include "estructuras/NodoSimboloCadena.h"
#include "estructuras/NodoTransicionCompuesta.h"

bool SimuladorDFAUnion::simular(const DFAUnion& dfaUnion,
                                const CadenaEntrada& cadena,
                                bool& aceptada,
                                std::string& estadoFinalDFA1,
                                std::string& estadoFinalDFA2) const {
    aceptada = false;
    std::string estadoActualDFA1 = dfaUnion.obtenerEstadoInicialDFA1();
    std::string estadoActualDFA2 = dfaUnion.obtenerEstadoInicialDFA2();
    const NodoSimboloCadena* simboloActual = cadena.obtenerPrimero();

    while (simboloActual != nullptr) {
        if (!dfaUnion.obtenerAlfabeto().existe(simboloActual->simbolo)) {
            return false;
        }

        std::string destinoDFA1;
        std::string destinoDFA2;
        if (!buscarDestino(dfaUnion, estadoActualDFA1, estadoActualDFA2,
                           simboloActual->simbolo, destinoDFA1, destinoDFA2)) {
            return false;
        }

        estadoActualDFA1 = destinoDFA1;
        estadoActualDFA2 = destinoDFA2;
        simboloActual = simboloActual->siguiente;
    }

    estadoFinalDFA1 = estadoActualDFA1;
    estadoFinalDFA2 = estadoActualDFA2;
    aceptada = dfaUnion.obtenerEstadosFinales().existe(estadoFinalDFA1,
                                                       estadoFinalDFA2);
    return true;
}

bool SimuladorDFAUnion::simularConTraza(
    const DFAUnion& dfaUnion,
    const CadenaEntrada& cadena,
    ListaPasosDFAUnion& trazabilidad,
    bool& aceptada,
    std::string& estadoFinalDFA1,
    std::string& estadoFinalDFA2) const {
    trazabilidad.limpiar();
    aceptada = false;
    std::string estadoActualDFA1 = dfaUnion.obtenerEstadoInicialDFA1();
    std::string estadoActualDFA2 = dfaUnion.obtenerEstadoInicialDFA2();
    const NodoSimboloCadena* simboloActual = cadena.obtenerPrimero();

    while (simboloActual != nullptr) {
        if (!dfaUnion.obtenerAlfabeto().existe(simboloActual->simbolo)) {
            estadoFinalDFA1 = estadoActualDFA1;
            estadoFinalDFA2 = estadoActualDFA2;
            return false;
        }

        std::string destinoDFA1;
        std::string destinoDFA2;
        if (!buscarDestino(dfaUnion, estadoActualDFA1, estadoActualDFA2,
                           simboloActual->simbolo, destinoDFA1, destinoDFA2)) {
            estadoFinalDFA1 = estadoActualDFA1;
            estadoFinalDFA2 = estadoActualDFA2;
            return false;
        }

        trazabilidad.agregarPaso(estadoActualDFA1, estadoActualDFA2,
                                 simboloActual->simbolo, destinoDFA1,
                                 destinoDFA2);
        estadoActualDFA1 = destinoDFA1;
        estadoActualDFA2 = destinoDFA2;
        simboloActual = simboloActual->siguiente;
    }

    estadoFinalDFA1 = estadoActualDFA1;
    estadoFinalDFA2 = estadoActualDFA2;
    aceptada = dfaUnion.obtenerEstadosFinales().existe(estadoFinalDFA1,
                                                       estadoFinalDFA2);
    return true;
}

bool SimuladorDFAUnion::buscarDestino(const DFAUnion& dfaUnion,
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
