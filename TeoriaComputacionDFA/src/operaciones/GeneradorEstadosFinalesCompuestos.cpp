#include "operaciones/GeneradorEstadosFinalesCompuestos.h"

void GeneradorEstadosFinalesCompuestos::generar(
    const DFA& dfa1, const DFA& dfa2,
    const ListaEstadosCompuestos& estadosCompuestos,
    ListaEstadosCompuestos& estadosFinales) const {
    estadosFinales.limpiar();

    const NodoEstadoCompuesto* estadoActual = estadosCompuestos.obtenerPrimero();

    while (estadoActual != nullptr) {
        bool finalEnDFA1 = dfa1.obtenerEstadosFinales().existe(estadoActual->estadoDFA1);
        bool finalEnDFA2 = dfa2.obtenerEstadosFinales().existe(estadoActual->estadoDFA2);

        if (finalEnDFA1 || finalEnDFA2) {
            estadosFinales.insertar(estadoActual->estadoDFA1, estadoActual->estadoDFA2);
        }

        estadoActual = estadoActual->siguiente;
    }
}
