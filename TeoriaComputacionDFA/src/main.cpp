#include <iostream>

#include "estructuras/ListaEstados.h"

int main() {
    ListaEstados estados;

    std::cout << "FASE 2 - PRUEBA LISTA DE ESTADOS" << std::endl;
    std::cout << std::endl;

    std::cout << "Lista inicialmente vacia: "
              << (estados.estaVacia() ? "SI" : "NO") << std::endl;
    std::cout << std::endl;

    std::cout << "Insertando q0: " << (estados.insertar("q0") ? "OK" : "ERROR")
              << std::endl;
    std::cout << "Insertando q1: " << (estados.insertar("q1") ? "OK" : "ERROR")
              << std::endl;
    std::cout << "Insertando q2: " << (estados.insertar("q2") ? "OK" : "ERROR")
              << std::endl;
    std::cout << std::endl;

    estados.mostrar();
    std::cout << std::endl;

    std::cout << "Cantidad: " << estados.cantidad() << std::endl;
    std::cout << std::endl;

    std::cout << "Buscar q1: " << (estados.existe("q1") ? "ENCONTRADO" : "NO ENCONTRADO")
              << std::endl;
    std::cout << "Buscar q5: " << (estados.existe("q5") ? "ENCONTRADO" : "NO ENCONTRADO")
              << std::endl;
    std::cout << std::endl;

    std::cout << "Intentando insertar q1 nuevamente:" << std::endl;
    if (estados.insertar("q1")) {
        std::cout << "INSERTADO" << std::endl;
    } else {
        std::cout << "ESTADO DUPLICADO - NO INSERTADO" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Eliminando q1:" << std::endl;
    std::cout << (estados.eliminar("q1") ? "OK" : "NO EXISTE") << std::endl;
    std::cout << std::endl;

    estados.mostrar();
    std::cout << std::endl;

    std::cout << "Cantidad: " << estados.cantidad() << std::endl;
    std::cout << std::endl;

    std::cout << "Intentando eliminar q5:" << std::endl;
    std::cout << (estados.eliminar("q5") ? "OK" : "NO EXISTE") << std::endl;

    return 0;
}
