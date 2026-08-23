#include "estructuras/ListaTransiciones.h"

#include <iostream>

ListaTransiciones::ListaTransiciones() : primero(nullptr) {}

ListaTransiciones::~ListaTransiciones() {
    NodoTransicion* actual = primero;

    while (actual != nullptr) {
        NodoTransicion* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }

    primero = nullptr;
}

bool ListaTransiciones::estaVacia() const {
    return primero == nullptr;
}

bool ListaTransiciones::insertar(const std::string& origen, const std::string& simbolo,
                                 const std::string& destino) {
    NodoTransicion* nueva = new NodoTransicion(origen, simbolo, destino);

    if (primero == nullptr) {
        primero = nueva;
        return true;
    }

    NodoTransicion* actual = primero;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }

    actual->siguiente = nueva;
    return true;
}

bool ListaTransiciones::existeTransicionExacta(const std::string& origen,
                                               const std::string& simbolo,
                                               const std::string& destino) const {
    const NodoTransicion* actual = primero;

    while (actual != nullptr) {
        if (actual->origen == origen && actual->simbolo == simbolo &&
            actual->destino == destino) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

int ListaTransiciones::cantidadPorPar(const std::string& origen,
                                      const std::string& simbolo) const {
    int contador = 0;
    const NodoTransicion* actual = primero;

    while (actual != nullptr) {
        if (actual->origen == origen && actual->simbolo == simbolo) {
            contador++;
        }

        actual = actual->siguiente;
    }

    return contador;
}

int ListaTransiciones::cantidad() const {
    int contador = 0;
    const NodoTransicion* actual = primero;

    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

bool ListaTransiciones::eliminarTransicionExacta(const std::string& origen,
                                                 const std::string& simbolo,
                                                 const std::string& destino) {
    if (primero == nullptr) {
        return false;
    }

    if (primero->origen == origen && primero->simbolo == simbolo &&
        primero->destino == destino) {
        NodoTransicion* nodoAEliminar = primero;
        primero = primero->siguiente;
        delete nodoAEliminar;
        return true;
    }

    NodoTransicion* anterior = primero;
    NodoTransicion* actual = primero->siguiente;

    while (actual != nullptr) {
        if (actual->origen == origen && actual->simbolo == simbolo &&
            actual->destino == destino) {
            anterior->siguiente = actual->siguiente;
            delete actual;
            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    return false;
}

void ListaTransiciones::mostrar() const {
    std::cout << "Transiciones:" << std::endl;

    const NodoTransicion* actual = primero;
    while (actual != nullptr) {
        std::cout << actual->origen << " --" << actual->simbolo << "--> "
                  << actual->destino << std::endl;
        actual = actual->siguiente;
    }
}

const NodoTransicion* ListaTransiciones::obtenerPrimero() const {
    return primero;
}
