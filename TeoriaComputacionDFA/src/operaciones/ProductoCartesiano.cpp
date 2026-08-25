#include "operaciones/ProductoCartesiano.h"

void ProductoCartesiano::generar(const DFA& dfa1, const DFA& dfa2,
                                 ListaEstadosCompuestos& resultado) const {
    resultado.limpiar();

    const NodoEstado* estadoDFA1 = dfa1.obtenerEstados().obtenerPrimero();
    while (estadoDFA1 != nullptr) {
        const NodoEstado* estadoDFA2 = dfa2.obtenerEstados().obtenerPrimero();

        while (estadoDFA2 != nullptr) {
            resultado.insertar(estadoDFA1->nombre, estadoDFA2->nombre);
            estadoDFA2 = estadoDFA2->siguiente;
        }

        estadoDFA1 = estadoDFA1->siguiente;
    }
}
