#include "estructuras/ListaPosicionesEstadosCompuestos.h"

ListaPosicionesEstadosCompuestos::ListaPosicionesEstadosCompuestos() : primero(nullptr) {}

ListaPosicionesEstadosCompuestos::~ListaPosicionesEstadosCompuestos() { limpiar(); }

void ListaPosicionesEstadosCompuestos::limpiar() {
    while (primero != nullptr) {
        NodoPosicionEstadoCompuesto* siguiente = primero->siguiente;
        delete primero;
        primero = siguiente;
    }
}

void ListaPosicionesEstadosCompuestos::agregar(const std::string& estadoDFA1,
                                                const std::string& estadoDFA2,
                                                double x, double y) {
    NodoPosicionEstadoCompuesto* nuevo = new NodoPosicionEstadoCompuesto(estadoDFA1, estadoDFA2, x, y);
    if (primero == nullptr) { primero = nuevo; return; }
    NodoPosicionEstadoCompuesto* actual = primero;
    while (actual->siguiente != nullptr) actual = actual->siguiente;
    actual->siguiente = nuevo;
}

const NodoPosicionEstadoCompuesto* ListaPosicionesEstadosCompuestos::buscar(
    const std::string& estadoDFA1, const std::string& estadoDFA2) const {
    const NodoPosicionEstadoCompuesto* actual = primero;
    while (actual != nullptr) {
        if (actual->estadoDFA1 == estadoDFA1 && actual->estadoDFA2 == estadoDFA2) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

const NodoPosicionEstadoCompuesto* ListaPosicionesEstadosCompuestos::obtenerPrimero() const { return primero; }

int ListaPosicionesEstadosCompuestos::cantidad() const {
    int total = 0;
    const NodoPosicionEstadoCompuesto* actual = primero;
    while (actual != nullptr) { ++total; actual = actual->siguiente; }
    return total;
}
