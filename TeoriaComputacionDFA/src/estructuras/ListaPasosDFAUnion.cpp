#include "estructuras/ListaPasosDFAUnion.h"

ListaPasosDFAUnion::ListaPasosDFAUnion() : primero(nullptr) {}

ListaPasosDFAUnion::~ListaPasosDFAUnion() {
    limpiar();
}

void ListaPasosDFAUnion::agregarPaso(const std::string& origenDFA1,
                                     const std::string& origenDFA2,
                                     const std::string& simbolo,
                                     const std::string& destinoDFA1,
                                     const std::string& destinoDFA2) {
    NodoPasoDFAUnion* nuevo = new NodoPasoDFAUnion(
        origenDFA1, origenDFA2, simbolo, destinoDFA1, destinoDFA2);

    if (primero == nullptr) {
        primero = nuevo;
        return;
    }

    NodoPasoDFAUnion* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
}

bool ListaPasosDFAUnion::estaVacia() const {
    return primero == nullptr;
}

int ListaPasosDFAUnion::cantidad() const {
    int contador = 0;
    const NodoPasoDFAUnion* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

void ListaPasosDFAUnion::limpiar() {
    NodoPasoDFAUnion* actual = primero;

    while (actual != nullptr) {
        NodoPasoDFAUnion* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

const NodoPasoDFAUnion* ListaPasosDFAUnion::obtenerPrimero() const {
    return primero;
}
