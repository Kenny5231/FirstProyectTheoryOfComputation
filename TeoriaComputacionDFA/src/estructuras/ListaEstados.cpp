#include "estructuras/ListaEstados.h"

#include <iostream>

ListaEstados::ListaEstados() : primero(nullptr) {}

ListaEstados::~ListaEstados() {
    NodoEstado* actual = primero;

    while (actual != nullptr) {
        NodoEstado* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

bool ListaEstados::estaVacia() const {
    return primero == nullptr;
}

bool ListaEstados::existe(const std::string& nombre) const {
    const NodoEstado* actual = primero;

    while (actual != nullptr) {
        if (actual->nombre == nombre) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

bool ListaEstados::insertar(const std::string& nombre) {
    if (existe(nombre)) {
        return false;
    }

    NodoEstado* nuevo = new NodoEstado(nombre);

    if (primero == nullptr) {
        primero = nuevo;
        return true;
    }

    NodoEstado* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
    return true;
}

bool ListaEstados::eliminar(const std::string& nombre) {
    if (primero == nullptr) {
        return false;
    }

    if (primero->nombre == nombre) {
        NodoEstado* nodoAEliminar = primero;
        primero = primero->siguiente;
        delete nodoAEliminar;
        return true;
    }

    NodoEstado* anterior = primero;
    NodoEstado* actual = primero->siguiente;

    while (actual != nullptr) {
        if (actual->nombre == nombre) {
            anterior->siguiente = actual->siguiente;
            delete actual;
            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    return false;
}

int ListaEstados::cantidad() const {
    int contador = 0;
    const NodoEstado* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

void ListaEstados::mostrar() const {
    std::cout << "Estados:" << std::endl;

    const NodoEstado* actual = primero;
    while (actual != nullptr) {
        std::cout << actual->nombre << std::endl;
        actual = actual->siguiente;
    }
}

const NodoEstado* ListaEstados::obtenerPrimero() const {
    return primero;
}
