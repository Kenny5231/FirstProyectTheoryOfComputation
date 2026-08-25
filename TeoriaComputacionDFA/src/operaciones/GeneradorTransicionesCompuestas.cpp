#include "operaciones/GeneradorTransicionesCompuestas.h"

bool GeneradorTransicionesCompuestas::generar(
    const DFA& dfa1, const DFA& dfa2,
    const ListaEstadosCompuestos& estadosCompuestos,
    ListaTransicionesCompuestas& resultado) const {
    resultado.limpiar();

    const NodoEstadoCompuesto* estadoCompuesto = estadosCompuestos.obtenerPrimero();

    while (estadoCompuesto != nullptr) {
        const NodoSimbolo* simbolo = dfa1.obtenerAlfabeto().obtenerPrimero();

        while (simbolo != nullptr) {
            std::string destinoDFA1;
            std::string destinoDFA2;

            bool encontradoDFA1 =
                buscarDestino(dfa1.obtenerTransiciones(), estadoCompuesto->estadoDFA1,
                             simbolo->simbolo, destinoDFA1);

            bool encontradoDFA2 =
                buscarDestino(dfa2.obtenerTransiciones(), estadoCompuesto->estadoDFA2,
                             simbolo->simbolo, destinoDFA2);

            if (!encontradoDFA1 || !encontradoDFA2) {
                return false;
            }

            resultado.insertar(estadoCompuesto->estadoDFA1, estadoCompuesto->estadoDFA2,
                               simbolo->simbolo, destinoDFA1, destinoDFA2);

            simbolo = simbolo->siguiente;
        }

        estadoCompuesto = estadoCompuesto->siguiente;
    }

    return true;
}

bool GeneradorTransicionesCompuestas::buscarDestino(
    const ListaTransiciones& transiciones, const std::string& origen,
    const std::string& simbolo, std::string& destino) const {
    const NodoTransicion* actual = transiciones.obtenerPrimero();

    while (actual != nullptr) {
        if (actual->origen == origen && actual->simbolo == simbolo) {
            destino = actual->destino;
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}
