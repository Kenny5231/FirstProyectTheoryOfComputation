#include <iostream>

#include "estructuras/ListaEstados.h"
#include "estructuras/ListaSimbolos.h"
#include "estructuras/ListaTransiciones.h"

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
    std::cout << std::endl;

    ListaSimbolos simbolos;

    std::cout << "FASE 3 - PRUEBA LISTA DE SIMBOLOS" << std::endl;
    std::cout << std::endl;

    std::cout << "Lista inicialmente vacia: "
              << (simbolos.estaVacia() ? "SI" : "NO") << std::endl;
    std::cout << std::endl;

    std::cout << "Insertando a: " << (simbolos.insertar("a") ? "OK" : "ERROR")
              << std::endl;
    std::cout << "Insertando b: " << (simbolos.insertar("b") ? "OK" : "ERROR")
              << std::endl;
    std::cout << "Insertando c: " << (simbolos.insertar("c") ? "OK" : "ERROR")
              << std::endl;
    std::cout << std::endl;

    simbolos.mostrar();
    std::cout << std::endl;

    std::cout << "Cantidad: " << simbolos.cantidad() << std::endl;
    std::cout << std::endl;

    std::cout << "Buscar b: " << (simbolos.existe("b") ? "ENCONTRADO" : "NO ENCONTRADO")
              << std::endl;
    std::cout << "Buscar x: " << (simbolos.existe("x") ? "ENCONTRADO" : "NO ENCONTRADO")
              << std::endl;
    std::cout << std::endl;

    std::cout << "Intentando insertar b nuevamente:" << std::endl;
    if (simbolos.insertar("b")) {
        std::cout << "INSERTADO" << std::endl;
    } else {
        std::cout << "SIMBOLO DUPLICADO - NO INSERTADO" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Eliminando b:" << std::endl;
    std::cout << (simbolos.eliminar("b") ? "OK" : "NO EXISTE") << std::endl;
    std::cout << std::endl;

    simbolos.mostrar();
    std::cout << std::endl;

    std::cout << "Cantidad: " << simbolos.cantidad() << std::endl;
    std::cout << std::endl;

    std::cout << "Intentando eliminar x:" << std::endl;
    std::cout << (simbolos.eliminar("x") ? "OK" : "NO EXISTE") << std::endl;
    std::cout << std::endl;

    ListaSimbolos casosBorde;

    std::cout << "CASOS BORDE LISTA DE SIMBOLOS" << std::endl;
    std::cout << std::endl;

    std::cout << "Eliminar en lista vacia: "
              << (casosBorde.eliminar("z") ? "OK" : "NO EXISTE") << std::endl;

    casosBorde.insertar("u");
    casosBorde.insertar("v");
    casosBorde.insertar("w");

    std::cout << "Eliminar primer simbolo (u): "
              << (casosBorde.eliminar("u") ? "OK" : "NO EXISTE") << std::endl;
    std::cout << "Eliminar ultimo simbolo (w): "
              << (casosBorde.eliminar("w") ? "OK" : "NO EXISTE") << std::endl;
    std::cout << "Eliminar simbolo restante (v): "
              << (casosBorde.eliminar("v") ? "OK" : "NO EXISTE") << std::endl;

    std::cout << "Lista vacia despues de eliminar todo: "
              << (casosBorde.estaVacia() ? "SI" : "NO") << std::endl;

    std::cout << "Insertar despues de vaciar lista (k): "
              << (casosBorde.insertar("k") ? "OK" : "ERROR") << std::endl;
    std::cout << "Cantidad final casos borde: " << casosBorde.cantidad() << std::endl;
    std::cout << std::endl;

    ListaTransiciones transiciones;

    std::cout << "FASE 4 - PRUEBA LISTA DE TRANSICIONES" << std::endl;
    std::cout << std::endl;

    std::cout << "Lista inicialmente vacia: "
              << (transiciones.estaVacia() ? "SI" : "NO") << std::endl;
    std::cout << std::endl;

    transiciones.insertar("q0", "a", "q1");
    transiciones.insertar("q0", "b", "q0");
    transiciones.insertar("q1", "a", "q2");
    transiciones.insertar("q1", "b", "q0");

    transiciones.mostrar();
    std::cout << std::endl;

    std::cout << "Cantidad total: " << transiciones.cantidad() << std::endl;
    std::cout << std::endl;

    std::cout << "Existe q0 --a--> q1: "
              << (transiciones.existeTransicionExacta("q0", "a", "q1") ? "SI" : "NO")
              << std::endl;
    std::cout << "Existe q0 --a--> q5: "
              << (transiciones.existeTransicionExacta("q0", "a", "q5") ? "SI" : "NO")
              << std::endl;
    std::cout << std::endl;

    std::cout << "Cantidad por par (q0,a): "
              << transiciones.cantidadPorPar("q0", "a") << std::endl;
    std::cout << std::endl;

    std::cout << "Insertando intencionalmente q0 --a--> q2" << std::endl;
    transiciones.insertar("q0", "a", "q2");
    std::cout << "Cantidad por par (q0,a): "
              << transiciones.cantidadPorPar("q0", "a") << std::endl;
    std::cout << std::endl;

    std::cout << "Eliminando exactamente q0 --a--> q2: "
              << (transiciones.eliminarTransicionExacta("q0", "a", "q2") ? "OK" : "NO EXISTE")
              << std::endl;
    std::cout << "Cantidad por par (q0,a): "
              << transiciones.cantidadPorPar("q0", "a") << std::endl;
    std::cout << std::endl;

    std::cout << "Eliminar transicion inexistente q2 --b--> q1: "
              << (transiciones.eliminarTransicionExacta("q2", "b", "q1") ? "OK" : "NO EXISTE")
              << std::endl;

    std::cout << "Eliminar primer nodo q0 --a--> q1: "
              << (transiciones.eliminarTransicionExacta("q0", "a", "q1") ? "OK" : "NO EXISTE")
              << std::endl;

    std::cout << "Eliminar ultimo nodo q1 --b--> q0: "
              << (transiciones.eliminarTransicionExacta("q1", "b", "q0") ? "OK" : "NO EXISTE")
              << std::endl;
    std::cout << std::endl;

    transiciones.mostrar();
    std::cout << "Cantidad final transiciones: " << transiciones.cantidad() << std::endl;

    return 0;
}
