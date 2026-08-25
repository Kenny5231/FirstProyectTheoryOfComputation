#include "estructuras/ListaTransicionesCompuestas.h"

#include <iostream>

ListaTransicionesCompuestas::ListaTransicionesCompuestas() : primero(nullptr) {}

ListaTransicionesCompuestas::~ListaTransicionesCompuestas() {
    limpiar();
}

bool ListaTransicionesCompuestas::estaVacia() const {
    return primero == nullptr;
}

bool ListaTransicionesCompuestas::existeTransicionExacta(
    const std::string& origenDFA1, const std::string& origenDFA2,
    const std::string& simbolo, const std::string& destinoDFA1,
    const std::string& destinoDFA2) const {
    const NodoTransicionCompuesta* actual = primero;

    while (actual != nullptr) {
        if (actual->origenDFA1 == origenDFA1 && actual->origenDFA2 == origenDFA2 &&
            actual->simbolo == simbolo && actual->destinoDFA1 == destinoDFA1 &&
            actual->destinoDFA2 == destinoDFA2) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

bool ListaTransicionesCompuestas::insertar(const std::string& origenDFA1,
                                           const std::string& origenDFA2,
                                           const std::string& simbolo,
                                           const std::string& destinoDFA1,
                                           const std::string& destinoDFA2) {
    if (existeTransicionExacta(origenDFA1, origenDFA2, simbolo, destinoDFA1,
                               destinoDFA2)) {
        return false;
    }

    NodoTransicionCompuesta* nueva = new NodoTransicionCompuesta(
        origenDFA1, origenDFA2, simbolo, destinoDFA1, destinoDFA2);

    if (primero == nullptr) {
        primero = nueva;
        return true;
    }

    NodoTransicionCompuesta* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nueva;
    return true;
}

int ListaTransicionesCompuestas::cantidad() const {
    int contador = 0;
    const NodoTransicionCompuesta* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

void ListaTransicionesCompuestas::mostrar() const {
    std::cout << "Transiciones compuestas:" << std::endl;

    const NodoTransicionCompuesta* actual = primero;
    while (actual != nullptr) {
        std::cout << "(" << actual->origenDFA1 << "," << actual->origenDFA2 << ")"
                  << " --" << actual->simbolo << "--> "
                  << "(" << actual->destinoDFA1 << "," << actual->destinoDFA2 << ")"
                  << std::endl;

        actual = actual->siguiente;
    }
}

void ListaTransicionesCompuestas::limpiar() {
    NodoTransicionCompuesta* actual = primero;

    while (actual != nullptr) {
        NodoTransicionCompuesta* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

const NodoTransicionCompuesta* ListaTransicionesCompuestas::obtenerPrimero() const {
    return primero;
}
