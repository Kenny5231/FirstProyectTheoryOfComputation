#include "estructuras/ListaEstadosCompuestos.h"

#include <iostream>

ListaEstadosCompuestos::ListaEstadosCompuestos() : primero(nullptr) {}

ListaEstadosCompuestos::~ListaEstadosCompuestos() {
    limpiar();
}

bool ListaEstadosCompuestos::estaVacia() const {
    return primero == nullptr;
}

bool ListaEstadosCompuestos::existe(const std::string& estadoDFA1,
                                    const std::string& estadoDFA2) const {
    const NodoEstadoCompuesto* actual = primero;

    while (actual != nullptr) {
        if (actual->estadoDFA1 == estadoDFA1 && actual->estadoDFA2 == estadoDFA2) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

bool ListaEstadosCompuestos::insertar(const std::string& estadoDFA1,
                                      const std::string& estadoDFA2) {
    if (existe(estadoDFA1, estadoDFA2)) {
        return false;
    }

    NodoEstadoCompuesto* nuevo = new NodoEstadoCompuesto(estadoDFA1, estadoDFA2);

    if (primero == nullptr) {
        primero = nuevo;
        return true;
    }

    NodoEstadoCompuesto* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
    return true;
}

int ListaEstadosCompuestos::cantidad() const {
    int contador = 0;
    const NodoEstadoCompuesto* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

void ListaEstadosCompuestos::mostrar() const {
    std::cout << "Estados compuestos:" << std::endl;

    const NodoEstadoCompuesto* actual = primero;
    while (actual != nullptr) {
        std::cout << "(" << actual->estadoDFA1 << "," << actual->estadoDFA2 << ")"
                  << std::endl;
        actual = actual->siguiente;
    }
}

void ListaEstadosCompuestos::limpiar() {
    NodoEstadoCompuesto* actual = primero;

    while (actual != nullptr) {
        NodoEstadoCompuesto* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

const NodoEstadoCompuesto* ListaEstadosCompuestos::obtenerPrimero() const {
    return primero;
}
