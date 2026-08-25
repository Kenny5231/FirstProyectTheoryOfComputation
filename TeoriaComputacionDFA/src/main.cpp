#include <iostream>

#include "automata/DFA.h"
#include "operaciones/CompatibilidadDFA.h"
#include "operaciones/ProductoCartesiano.h"
#include "estructuras/ListaEstadosCompuestos.h"
#include "validacion/ListaErrores.h"
#include "validacion/ValidadorDFA.h"

void construirDFAPrincipal1(DFA& dfa) {
    dfa.agregarEstado("q0");
    dfa.agregarEstado("q1");
    dfa.agregarSimbolo("a");
    dfa.agregarSimbolo("b");
    dfa.establecerEstadoInicial("q0");
    dfa.agregarEstadoFinal("q1");

    dfa.agregarTransicion("q0", "a", "q1");
    dfa.agregarTransicion("q0", "b", "q0");
    dfa.agregarTransicion("q1", "a", "q1");
    dfa.agregarTransicion("q1", "b", "q0");
}

void construirDFAPrincipal2(DFA& dfa) {
    dfa.agregarEstado("p0");
    dfa.agregarEstado("p1");
    dfa.agregarEstado("p2");
    dfa.agregarSimbolo("a");
    dfa.agregarSimbolo("b");
    dfa.establecerEstadoInicial("p0");
    dfa.agregarEstadoFinal("p2");

    dfa.agregarTransicion("p0", "a", "p1");
    dfa.agregarTransicion("p0", "b", "p2");
    dfa.agregarTransicion("p1", "a", "p0");
    dfa.agregarTransicion("p1", "b", "p2");
    dfa.agregarTransicion("p2", "a", "p2");
    dfa.agregarTransicion("p2", "b", "p0");
}

void construirDFAUnEstadoDos(DFA& dfa1, DFA& dfa2) {
    dfa1.agregarEstado("A");
    dfa1.agregarSimbolo("a");
    dfa1.establecerEstadoInicial("A");
    dfa1.agregarEstadoFinal("A");
    dfa1.agregarTransicion("A", "a", "A");

    dfa2.agregarEstado("X");
    dfa2.agregarEstado("Y");
    dfa2.agregarSimbolo("a");
    dfa2.establecerEstadoInicial("X");
    dfa2.agregarEstadoFinal("Y");
    dfa2.agregarTransicion("X", "a", "Y");
    dfa2.agregarTransicion("Y", "a", "X");
}

void construirDFAIncompatibles(DFA& dfa1, DFA& dfa2) {
    dfa1.agregarEstado("q0");
    dfa1.agregarSimbolo("a");
    dfa1.agregarSimbolo("b");
    dfa1.establecerEstadoInicial("q0");
    dfa1.agregarEstadoFinal("q0");
    dfa1.agregarTransicion("q0", "a", "q0");
    dfa1.agregarTransicion("q0", "b", "q0");

    dfa2.agregarEstado("p0");
    dfa2.agregarSimbolo("a");
    dfa2.agregarSimbolo("c");
    dfa2.establecerEstadoInicial("p0");
    dfa2.agregarEstadoFinal("p0");
    dfa2.agregarTransicion("p0", "a", "p0");
    dfa2.agregarTransicion("p0", "c", "p0");
}

void construirDFAInvalido(DFA& dfaValido, DFA& dfaInvalido) {
    dfaValido.agregarEstado("q0");
    dfaValido.agregarSimbolo("a");
    dfaValido.establecerEstadoInicial("q0");
    dfaValido.agregarEstadoFinal("q0");
    dfaValido.agregarTransicion("q0", "a", "q0");

    dfaInvalido.agregarEstado("p0");
    dfaInvalido.agregarSimbolo("a");
    dfaInvalido.establecerEstadoInicial("p99");
    dfaInvalido.agregarTransicion("p0", "a", "p0");
}

bool validarDFA(const std::string& nombre, const DFA& dfa,
                const ValidadorDFA& validador, ListaErrores& errores) {
    bool valido = validador.validar(dfa, errores);
    std::cout << nombre << " valido: " << (valido ? "SI" : "NO") << std::endl;

    if (!valido) {
        errores.mostrar();
    }

    return valido;
}

void ejecutarFlujoProducto(const std::string& titulo, const DFA& dfa1, const DFA& dfa2,
                           const ValidadorDFA& validador,
                           const CompatibilidadDFA& compatibilidad,
                           const ProductoCartesiano& producto,
                           ListaErrores& erroresValidacion,
                           ListaErrores& erroresCompatibilidad,
                           ListaEstadosCompuestos& estadosCompuestos,
                           bool mostrarVerificacionPrincipal) {
    std::cout << titulo << std::endl;

    bool dfa1Valido = validarDFA("DFA 1", dfa1, validador, erroresValidacion);
    bool dfa2Valido = validarDFA("DFA 2", dfa2, validador, erroresValidacion);

    if (!dfa1Valido || !dfa2Valido) {
        std::cout << "OPERACION BLOQUEADA: ambos DFA deben ser validos." << std::endl;
        std::cout << "PRODUCTO CARTESIANO NO GENERADO." << std::endl;
        std::cout << std::endl;
        return;
    }

    bool compatibles = compatibilidad.compararAlfabetos(dfa1, dfa2, erroresCompatibilidad);

    std::cout << "Resultado de compatibilidad: "
              << (compatibles ? "ALFABETOS COMPATIBLES" : "ALFABETOS INCOMPATIBLES")
              << std::endl;

    if (!compatibles) {
        erroresCompatibilidad.mostrar();
        std::cout << "PRODUCTO CARTESIANO NO GENERADO: alfabetos incompatibles."
                  << std::endl;
        std::cout << std::endl;
        return;
    }

    producto.generar(dfa1, dfa2, estadosCompuestos);

    std::cout << "PRODUCTO CARTESIANO GENERADO" << std::endl;
    estadosCompuestos.mostrar();
    std::cout << "Cantidad obtenida: " << estadosCompuestos.cantidad() << std::endl;

    if (mostrarVerificacionPrincipal) {
        int cantidadDFA1 = dfa1.obtenerEstados().cantidad();
        int cantidadDFA2 = dfa2.obtenerEstados().cantidad();

        std::cout << "Cantidad de estados DFA 1: " << cantidadDFA1 << std::endl;
        std::cout << "Cantidad de estados DFA 2: " << cantidadDFA2 << std::endl;
        std::cout << "Cantidad esperada (Q1 x Q2): "
                  << (cantidadDFA1 * cantidadDFA2) << std::endl;

        std::cout << "Existe (q0,p1): "
                  << (estadosCompuestos.existe("q0", "p1") ? "SI" : "NO")
                  << std::endl;
        std::cout << "Existe (q5,p1): "
                  << (estadosCompuestos.existe("q5", "p1") ? "SI" : "NO")
                  << std::endl;

        std::cout << "Estado inicial compuesto esperado (solo conceptual): ("
                  << dfa1.obtenerEstadoInicial() << ","
                  << dfa2.obtenerEstadoInicial() << ")" << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    ValidadorDFA validador;
    CompatibilidadDFA compatibilidad;
    ProductoCartesiano producto;
    ListaErrores erroresValidacion;
    ListaErrores erroresCompatibilidad;
    ListaEstadosCompuestos estadosCompuestos;

    DFA dfa1Principal;
    DFA dfa2Principal;
    construirDFAPrincipal1(dfa1Principal);
    construirDFAPrincipal2(dfa2Principal);

    ejecutarFlujoProducto("PRUEBA PRINCIPAL - Q1={q0,q1}, Q2={p0,p1,p2}",
                          dfa1Principal, dfa2Principal, validador, compatibilidad,
                          producto, erroresValidacion, erroresCompatibilidad,
                          estadosCompuestos, true);

    DFA dfaUnEstado;
    DFA dfaDosEstados;
    construirDFAUnEstadoDos(dfaUnEstado, dfaDosEstados);

    ejecutarFlujoProducto("PRUEBA ADICIONAL - Q={A} y Q={X,Y}", dfaUnEstado,
                          dfaDosEstados, validador, compatibilidad, producto,
                          erroresValidacion, erroresCompatibilidad,
                          estadosCompuestos, false);

    DFA dfaIncompatible1;
    DFA dfaIncompatible2;
    construirDFAIncompatibles(dfaIncompatible1, dfaIncompatible2);

    ejecutarFlujoProducto("PRUEBA BLOQUEO - ALFABETOS INCOMPATIBLES",
                          dfaIncompatible1, dfaIncompatible2, validador,
                          compatibilidad, producto, erroresValidacion,
                          erroresCompatibilidad, estadosCompuestos, false);

    DFA dfaValido;
    DFA dfaInvalido;
    construirDFAInvalido(dfaValido, dfaInvalido);

    ejecutarFlujoProducto("PRUEBA BLOQUEO - DFA INVALIDO", dfaValido, dfaInvalido,
                          validador, compatibilidad, producto, erroresValidacion,
                          erroresCompatibilidad, estadosCompuestos, false);

    return 0;
}
