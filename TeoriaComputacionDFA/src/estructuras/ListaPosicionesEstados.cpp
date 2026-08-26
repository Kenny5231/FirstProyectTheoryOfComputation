#include "estructuras/ListaPosicionesEstados.h"

ListaPosicionesEstados::ListaPosicionesEstados() : primero(nullptr) {}

ListaPosicionesEstados::~ListaPosicionesEstados() {
    limpiar();
}

void ListaPosicionesEstados::limpiar() {
    while (primero != nullptr) {
        NodoPosicionEstado* siguiente = primero->siguiente;
        delete primero;
        primero = siguiente;
    }
}

void ListaPosicionesEstados::agregar(const std::string& estado, double x, double y) {
    NodoPosicionEstado* nuevo = new NodoPosicionEstado(estado, x, y);
    if (primero == nullptr) {
        primero = nuevo;
        return;
    }
    NodoPosicionEstado* actual = primero;
    while (actual->siguiente != nullptr) actual = actual->siguiente;
    actual->siguiente = nuevo;
}

const NodoPosicionEstado* ListaPosicionesEstados::buscar(const std::string& estado) const {
    const NodoPosicionEstado* actual = primero;
    while (actual != nullptr) {
        if (actual->estado == estado) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

const NodoPosicionEstado* ListaPosicionesEstados::obtenerPrimero() const { return primero; }

int ListaPosicionesEstados::cantidad() const {
    int total = 0;
    const NodoPosicionEstado* actual = primero;
    while (actual != nullptr) { ++total; actual = actual->siguiente; }
    return total;
}
