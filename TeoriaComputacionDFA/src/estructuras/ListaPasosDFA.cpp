#include "estructuras/ListaPasosDFA.h"

ListaPasosDFA::ListaPasosDFA() : primero(nullptr) {}

ListaPasosDFA::~ListaPasosDFA() {
    limpiar();
}

void ListaPasosDFA::agregarPaso(const std::string& origen,
                                const std::string& simbolo,
                                const std::string& destino) {
    NodoPasoDFA* nuevo = new NodoPasoDFA(origen, simbolo, destino);

    if (primero == nullptr) {
        primero = nuevo;
        return;
    }

    NodoPasoDFA* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
}

bool ListaPasosDFA::estaVacia() const {
    return primero == nullptr;
}

int ListaPasosDFA::cantidad() const {
    int contador = 0;
    const NodoPasoDFA* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

void ListaPasosDFA::limpiar() {
    NodoPasoDFA* actual = primero;

    while (actual != nullptr) {
        NodoPasoDFA* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

const NodoPasoDFA* ListaPasosDFA::obtenerPrimero() const {
    return primero;
}
