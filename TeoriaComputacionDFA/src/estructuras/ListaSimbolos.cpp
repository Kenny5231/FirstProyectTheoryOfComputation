#include "estructuras/ListaSimbolos.h"

#include <iostream>

ListaSimbolos::ListaSimbolos() : primero(nullptr) {}

ListaSimbolos::~ListaSimbolos() {
    NodoSimbolo* actual = primero;

    while (actual != nullptr) {
        NodoSimbolo* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

bool ListaSimbolos::estaVacia() const {
    return primero == nullptr;
}

bool ListaSimbolos::existe(const std::string& simbolo) const {
    const NodoSimbolo* actual = primero;

    while (actual != nullptr) {
        if (actual->simbolo == simbolo) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

bool ListaSimbolos::insertar(const std::string& simbolo) {
    if (existe(simbolo)) {
        return false;
    }

    NodoSimbolo* nuevo = new NodoSimbolo(simbolo);

    if (primero == nullptr) {
        primero = nuevo;
        return true;
    }

    NodoSimbolo* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
    return true;
}

bool ListaSimbolos::eliminar(const std::string& simbolo) {
    if (primero == nullptr) {
        return false;
    }

    if (primero->simbolo == simbolo) {
        NodoSimbolo* nodoAEliminar = primero;
        primero = primero->siguiente;
        delete nodoAEliminar;
        return true;
    }

    NodoSimbolo* anterior = primero;
    NodoSimbolo* actual = primero->siguiente;

    while (actual != nullptr) {
        if (actual->simbolo == simbolo) {
            anterior->siguiente = actual->siguiente;
            delete actual;
            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    return false;
}

int ListaSimbolos::cantidad() const {
    int contador = 0;
    const NodoSimbolo* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

void ListaSimbolos::mostrar() const {
    std::cout << "Alfabeto:" << std::endl;

    const NodoSimbolo* actual = primero;
    while (actual != nullptr) {
        std::cout << actual->simbolo << std::endl;
        actual = actual->siguiente;
    }
}

const NodoSimbolo* ListaSimbolos::obtenerPrimero() const {
    return primero;
}
