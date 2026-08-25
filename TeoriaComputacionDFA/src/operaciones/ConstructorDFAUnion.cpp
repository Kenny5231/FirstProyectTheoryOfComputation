#include "operaciones/ConstructorDFAUnion.h"

#include "operaciones/GeneradorEstadosFinalesCompuestos.h"
#include "operaciones/GeneradorTransicionesCompuestas.h"
#include "operaciones/ProductoCartesiano.h"

bool ConstructorDFAUnion::construir(const DFA& dfa1, const DFA& dfa2,
                                    DFAUnion& resultado) const {
    if (!resultado.estaVacio()) {
        return false;
    }

    ListaEstadosCompuestos estadosGenerados;
    ListaTransicionesCompuestas transicionesGeneradas;
    ListaEstadosCompuestos finalesGenerados;

    ProductoCartesiano producto;
    GeneradorTransicionesCompuestas generadorTransiciones;
    GeneradorEstadosFinalesCompuestos generadorFinales;

    producto.generar(dfa1, dfa2, estadosGenerados);

    bool transicionesGeneradasCorrectamente = generadorTransiciones.generar(
        dfa1, dfa2, estadosGenerados, transicionesGeneradas);

    if (!transicionesGeneradasCorrectamente) {
        return false;
    }

    generadorFinales.generar(dfa1, dfa2, estadosGenerados, finalesGenerados);

    const NodoEstadoCompuesto* estadoActual = estadosGenerados.obtenerPrimero();
    while (estadoActual != nullptr) {
        if (!resultado.agregarEstado(estadoActual->estadoDFA1,
                                     estadoActual->estadoDFA2)) {
            return false;
        }

        estadoActual = estadoActual->siguiente;
    }

    const NodoSimbolo* simboloActual = dfa1.obtenerAlfabeto().obtenerPrimero();
    while (simboloActual != nullptr) {
        if (!resultado.agregarSimbolo(simboloActual->simbolo)) {
            return false;
        }

        simboloActual = simboloActual->siguiente;
    }

    resultado.establecerEstadoInicial(dfa1.obtenerEstadoInicial(),
                                      dfa2.obtenerEstadoInicial());

    const NodoTransicionCompuesta* transicionActual =
        transicionesGeneradas.obtenerPrimero();
    while (transicionActual != nullptr) {
        if (!resultado.agregarTransicion(
                transicionActual->origenDFA1, transicionActual->origenDFA2,
                transicionActual->simbolo, transicionActual->destinoDFA1,
                transicionActual->destinoDFA2)) {
            return false;
        }

        transicionActual = transicionActual->siguiente;
    }

    const NodoEstadoCompuesto* estadoFinalActual = finalesGenerados.obtenerPrimero();
    while (estadoFinalActual != nullptr) {
        if (!resultado.agregarEstadoFinal(estadoFinalActual->estadoDFA1,
                                          estadoFinalActual->estadoDFA2)) {
            return false;
        }

        estadoFinalActual = estadoFinalActual->siguiente;
    }

    return true;
}
