#include <iostream>

#include "automata/DFA.h"
#include "operaciones/CompatibilidadDFA.h"
#include "operaciones/GeneradorTransicionesCompuestas.h"
#include "operaciones/ProductoCartesiano.h"
#include "estructuras/ListaEstadosCompuestos.h"
#include "estructuras/ListaTransicionesCompuestas.h"
#include "validacion/ListaErrores.h"
#include "validacion/ValidadorDFA.h"

void construirDFAFase9_1(DFA& dfa) {
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

void construirDFAFase9_2(DFA& dfa) {
    dfa.agregarEstado("p0");
    dfa.agregarEstado("p1");
    dfa.agregarEstado("p2");
    dfa.agregarSimbolo("a");
    dfa.agregarSimbolo("b");
    dfa.establecerEstadoInicial("p0");
    dfa.agregarEstadoFinal("p2");

    dfa.agregarTransicion("p0", "a", "p1");
    dfa.agregarTransicion("p0", "b", "p0");
    dfa.agregarTransicion("p1", "a", "p2");
    dfa.agregarTransicion("p1", "b", "p0");
    dfa.agregarTransicion("p2", "a", "p2");
    dfa.agregarTransicion("p2", "b", "p1");
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

bool verificarValidezYCompatibilidad(const DFA& dfa1, const DFA& dfa2,
                                     const ValidadorDFA& validador,
                                     const CompatibilidadDFA& compatibilidad,
                                     ListaErrores& erroresValidacion,
                                     ListaErrores& erroresCompatibilidad,
                                     bool& bloqueoPorInvalidez,
                                     bool& bloqueoPorIncompatibilidad) {
    bloqueoPorInvalidez = false;
    bloqueoPorIncompatibilidad = false;

    bool dfa1Valido = validarDFA("DFA 1", dfa1, validador, erroresValidacion);
    bool dfa2Valido = validarDFA("DFA 2", dfa2, validador, erroresValidacion);

    if (!dfa1Valido || !dfa2Valido) {
        std::cout << "OPERACION BLOQUEADA: ambos DFA deben ser validos." << std::endl;
        bloqueoPorInvalidez = true;
        return false;
    }

    bool compatibles = compatibilidad.compararAlfabetos(dfa1, dfa2, erroresCompatibilidad);

    std::cout << "Resultado de compatibilidad: "
              << (compatibles ? "ALFABETOS COMPATIBLES" : "ALFABETOS INCOMPATIBLES")
              << std::endl;

    if (!compatibles) {
        erroresCompatibilidad.mostrar();
        bloqueoPorIncompatibilidad = true;
        return false;
    }

    return true;
}

void ejecutarPruebaPrincipalFase9(const DFA& dfa1, const DFA& dfa2,
                                  const ValidadorDFA& validador,
                                  const CompatibilidadDFA& compatibilidad,
                                  const ProductoCartesiano& producto,
                                  const GeneradorTransicionesCompuestas& generador,
                                  ListaErrores& erroresValidacion,
                                  ListaErrores& erroresCompatibilidad,
                                  ListaEstadosCompuestos& estadosCompuestos,
                                  ListaTransicionesCompuestas& transicionesCompuestas) {
    std::cout << "PRUEBA PRINCIPAL FASE 9" << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    if (!verificarValidezYCompatibilidad(dfa1, dfa2, validador, compatibilidad,
                                         erroresValidacion, erroresCompatibilidad,
                                         bloqueoPorInvalidez,
                                         bloqueoPorIncompatibilidad)) {
        std::cout << "PRODUCTO CARTESIANO NO GENERADO." << std::endl;
        std::cout << "TRANSICIONES COMPUESTAS NO GENERADAS." << std::endl;
        std::cout << std::endl;
        return;
    }

    producto.generar(dfa1, dfa2, estadosCompuestos);

    std::cout << "Estados compuestos generados:" << std::endl;
    estadosCompuestos.mostrar();

    bool transicionesGeneradas =
        generador.generar(dfa1, dfa2, estadosCompuestos, transicionesCompuestas);

    if (!transicionesGeneradas) {
        std::cout << "ERROR INTERNO: no se encontro una transicion esperada."
                  << std::endl;
        std::cout << std::endl;
        return;
    }

    std::cout << "Transiciones compuestas generadas:" << std::endl;
    transicionesCompuestas.mostrar();

    int cantidadEstadosCompuestos = estadosCompuestos.cantidad();
    int cantidadSimbolos = dfa1.obtenerAlfabeto().cantidad();
    int cantidadEsperada = cantidadEstadosCompuestos * cantidadSimbolos;
    int cantidadObtenida = transicionesCompuestas.cantidad();

    std::cout << "Cantidad de estados compuestos: " << cantidadEstadosCompuestos
              << std::endl;
    std::cout << "Cantidad de simbolos: " << cantidadSimbolos << std::endl;
    std::cout << "Cantidad esperada de transiciones: " << cantidadEsperada
              << std::endl;
    std::cout << "Cantidad obtenida de transiciones: " << cantidadObtenida
              << std::endl;

    std::cout << "Existe (q0,p0) --a--> (q1,p1): "
              << (transicionesCompuestas.existeTransicionExacta("q0", "p0", "a",
                                                               "q1", "p1")
                      ? "SI"
                      : "NO")
              << std::endl;
    std::cout << "Existe (q0,p0) --a--> (q0,p0): "
              << (transicionesCompuestas.existeTransicionExacta("q0", "p0", "a",
                                                               "q0", "p0")
                      ? "SI"
                      : "NO")
              << std::endl;
    std::cout << "Existe (q1,p2) --b--> (q0,p1): "
              << (transicionesCompuestas.existeTransicionExacta("q1", "p2", "b",
                                                               "q0", "p1")
                      ? "SI"
                      : "NO")
              << std::endl;

    std::cout << std::endl;
}

void ejecutarPruebaBloqueo(const std::string& titulo, const DFA& dfa1,
                           const DFA& dfa2, const ValidadorDFA& validador,
                           const CompatibilidadDFA& compatibilidad,
                           ListaErrores& erroresValidacion,
                           ListaErrores& erroresCompatibilidad) {
    std::cout << titulo << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    bool sePuedeAvanzar = verificarValidezYCompatibilidad(
        dfa1, dfa2, validador, compatibilidad, erroresValidacion,
        erroresCompatibilidad, bloqueoPorInvalidez, bloqueoPorIncompatibilidad);

    if (!sePuedeAvanzar) {
        if (bloqueoPorIncompatibilidad) {
            std::cout << "PRODUCTO CARTESIANO NO GENERADO: alfabetos incompatibles."
                      << std::endl;
        } else {
            std::cout << "PRODUCTO CARTESIANO NO GENERADO." << std::endl;
        }

        std::cout << "TRANSICIONES COMPUESTAS NO GENERADAS." << std::endl;
        std::cout << std::endl;
        return;
    }

    std::cout << "La prueba de bloqueo no debia avanzar, revisar datos." << std::endl;

    std::cout << std::endl;
}

int main() {
    ValidadorDFA validador;
    CompatibilidadDFA compatibilidad;
    ProductoCartesiano producto;
    GeneradorTransicionesCompuestas generador;
    ListaErrores erroresValidacion;
    ListaErrores erroresCompatibilidad;
    ListaEstadosCompuestos estadosCompuestos;
    ListaTransicionesCompuestas transicionesCompuestas;

    DFA dfa1Principal;
    DFA dfa2Principal;
    construirDFAFase9_1(dfa1Principal);
    construirDFAFase9_2(dfa2Principal);

    ejecutarPruebaPrincipalFase9(dfa1Principal, dfa2Principal, validador,
                                 compatibilidad, producto, generador,
                                 erroresValidacion, erroresCompatibilidad,
                                 estadosCompuestos, transicionesCompuestas);

    DFA dfaIncompatible1;
    DFA dfaIncompatible2;
    construirDFAIncompatibles(dfaIncompatible1, dfaIncompatible2);

    ejecutarPruebaBloqueo("PRUEBA BLOQUEO - ALFABETOS INCOMPATIBLES",
                          dfaIncompatible1, dfaIncompatible2, validador,
                          compatibilidad, erroresValidacion,
                          erroresCompatibilidad);

    DFA dfaValido;
    DFA dfaInvalido;
    construirDFAInvalido(dfaValido, dfaInvalido);

    ejecutarPruebaBloqueo("PRUEBA BLOQUEO - DFA INVALIDO", dfaValido,
                          dfaInvalido, validador, compatibilidad,
                          erroresValidacion, erroresCompatibilidad);

    return 0;
}
