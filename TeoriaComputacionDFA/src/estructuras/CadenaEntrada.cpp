#include "estructuras/CadenaEntrada.h"

#include <iostream>

CadenaEntrada::CadenaEntrada() : primero(nullptr) {}

CadenaEntrada::~CadenaEntrada() {
    limpiar();
}

void CadenaEntrada::agregarSimbolo(const std::string& simbolo) {
    NodoSimboloCadena* nuevo = new NodoSimboloCadena(simbolo);

    if (primero == nullptr) {
        primero = nuevo;
        return;
    }

    NodoSimboloCadena* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
}

bool CadenaEntrada::estaVacia() const {
    return primero == nullptr;
}

int CadenaEntrada::cantidad() const {
    int contador = 0;
    const NodoSimboloCadena* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

void CadenaEntrada::limpiar() {
    NodoSimboloCadena* actual = primero;

    while (actual != nullptr) {
        NodoSimboloCadena* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

const NodoSimboloCadena* CadenaEntrada::obtenerPrimero() const {
    return primero;
}

void CadenaEntrada::mostrar() const {
    std::cout << "Cadena: ";

    if (primero == nullptr) {
        std::cout << "ε" << std::endl;
        return;
    }

    const NodoSimboloCadena* actual = primero;
    while (actual != nullptr) {
        std::cout << actual->simbolo;

        if (actual->siguiente != nullptr) {
            std::cout << " ";
        }

        actual = actual->siguiente;
    }

    std::cout << std::endl;
}
