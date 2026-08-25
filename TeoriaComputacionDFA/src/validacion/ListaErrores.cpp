#include "validacion/ListaErrores.h"

#include <iostream>

ListaErrores::ListaErrores() : primero(nullptr) {}

ListaErrores::~ListaErrores() {
    limpiar();
}

void ListaErrores::insertar(const std::string& mensaje) {
    NodoError* nuevo = new NodoError(mensaje);

    if (primero == nullptr) {
        primero = nuevo;
        return;
    }

    NodoError* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
}

bool ListaErrores::estaVacia() const {
    return primero == nullptr;
}

int ListaErrores::cantidad() const {
    int contador = 0;
    const NodoError* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

void ListaErrores::mostrar() const {
    std::cout << "ERRORES ENCONTRADOS:" << std::endl;

    int indice = 1;
    const NodoError* actual = primero;
    while (actual != nullptr) {
        std::cout << indice << ". " << actual->mensaje << std::endl;
        indice++;
        actual = actual->siguiente;
    }
}

void ListaErrores::limpiar() {
    NodoError* actual = primero;

    while (actual != nullptr) {
        NodoError* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

const NodoError* ListaErrores::obtenerPrimero() const {
    return primero;
}
