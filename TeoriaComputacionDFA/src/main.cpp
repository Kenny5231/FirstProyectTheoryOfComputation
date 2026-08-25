#include <iostream>

#include "automata/DFA.h"
#include "operaciones/CompatibilidadDFA.h"
#include "validacion/ListaErrores.h"
#include "validacion/ValidadorDFA.h"

void construirDFAUnEstado(DFA& dfa, const std::string& simbolo1,
                          const std::string& simbolo2, const std::string& simbolo3,
                          int cantidadSimbolos) {
    dfa.agregarEstado("q0");
    dfa.establecerEstadoInicial("q0");
    dfa.agregarEstadoFinal("q0");

    if (cantidadSimbolos >= 1) {
        dfa.agregarSimbolo(simbolo1);
        dfa.agregarTransicion("q0", simbolo1, "q0");
    }

    if (cantidadSimbolos >= 2) {
        dfa.agregarSimbolo(simbolo2);
        dfa.agregarTransicion("q0", simbolo2, "q0");
    }

    if (cantidadSimbolos >= 3) {
        dfa.agregarSimbolo(simbolo3);
        dfa.agregarTransicion("q0", simbolo3, "q0");
    }
}

bool validarAmbos(const DFA& dfa1, const DFA& dfa2, const ValidadorDFA& validador,
                  ListaErrores& erroresValidacion) {
    bool dfa1Valido = validador.validar(dfa1, erroresValidacion);
    std::cout << "DFA 1 valido: " << (dfa1Valido ? "SI" : "NO") << std::endl;
    if (!dfa1Valido) {
        erroresValidacion.mostrar();
    }

    bool dfa2Valido = validador.validar(dfa2, erroresValidacion);
    std::cout << "DFA 2 valido: " << (dfa2Valido ? "SI" : "NO") << std::endl;
    if (!dfa2Valido) {
        erroresValidacion.mostrar();
    }

    return dfa1Valido && dfa2Valido;
}

void ejecutarCompatibilidad(const std::string& titulo, const DFA& dfa1, const DFA& dfa2,
                            const ValidadorDFA& validador,
                            const CompatibilidadDFA& compatibilidad,
                            ListaErrores& erroresValidacion,
                            ListaErrores& erroresCompatibilidad) {
    std::cout << titulo << std::endl;

    if (!validarAmbos(dfa1, dfa2, validador, erroresValidacion)) {
        std::cout << "OPERACION BLOQUEADA: ambos DFA deben ser validos." << std::endl;
        std::cout << "No se puede comprobar la compatibilidad porque uno de los DFA es invalido."
                  << std::endl;
        std::cout << std::endl;
        return;
    }

    bool compatibles = compatibilidad.compararAlfabetos(dfa1, dfa2, erroresCompatibilidad);

    std::cout << "Resultado de compatibilidad: "
              << (compatibles ? "ALFABETOS COMPATIBLES" : "ALFABETOS INCOMPATIBLES")
              << std::endl;
    std::cout << "Cantidad de errores: " << erroresCompatibilidad.cantidad() << std::endl;

    if (compatibles) {
        std::cout << "APTO PARA FUTURA UNION" << std::endl;
    } else {
        erroresCompatibilidad.mostrar();
        std::cout << "UNION BLOQUEADA" << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    ValidadorDFA validador;
    CompatibilidadDFA compatibilidad;
    ListaErrores erroresValidacion;
    ListaErrores erroresCompatibilidad;

    DFA dfa1Prueba1;
    DFA dfa2Prueba1;

    construirDFAUnEstado(dfa1Prueba1, "a", "b", "", 2);
    construirDFAUnEstado(dfa2Prueba1, "a", "b", "", 2);

    ejecutarCompatibilidad("PRUEBA 1 - ALFABETOS IGUALES", dfa1Prueba1, dfa2Prueba1,
                           validador, compatibilidad, erroresValidacion,
                           erroresCompatibilidad);

    DFA dfa1Prueba2;
    DFA dfa2Prueba2;

    construirDFAUnEstado(dfa1Prueba2, "a", "b", "c", 3);
    construirDFAUnEstado(dfa2Prueba2, "c", "a", "b", 3);

    ejecutarCompatibilidad("PRUEBA 2 - MISMO ALFABETO EN ORDEN DIFERENTE",
                           dfa1Prueba2, dfa2Prueba2, validador, compatibilidad,
                           erroresValidacion, erroresCompatibilidad);

    DFA dfa1Prueba3;
    DFA dfa2Prueba3;

    construirDFAUnEstado(dfa1Prueba3, "a", "b", "", 2);
    construirDFAUnEstado(dfa2Prueba3, "a", "b", "c", 3);

    ejecutarCompatibilidad("PRUEBA 3 - SIMBOLO FALTANTE EN DFA 1", dfa1Prueba3,
                           dfa2Prueba3, validador, compatibilidad, erroresValidacion,
                           erroresCompatibilidad);

    DFA dfa1Prueba4;
    DFA dfa2Prueba4;

    construirDFAUnEstado(dfa1Prueba4, "a", "b", "c", 3);
    construirDFAUnEstado(dfa2Prueba4, "a", "b", "", 2);

    ejecutarCompatibilidad("PRUEBA 4 - SIMBOLO FALTANTE EN DFA 2", dfa1Prueba4,
                           dfa2Prueba4, validador, compatibilidad, erroresValidacion,
                           erroresCompatibilidad);

    DFA dfa1Prueba5;
    DFA dfa2Prueba5;

    construirDFAUnEstado(dfa1Prueba5, "a", "b", "", 2);
    construirDFAUnEstado(dfa2Prueba5, "a", "c", "d", 3);

    ejecutarCompatibilidad("PRUEBA 5 - DIFERENCIAS EN AMBOS SENTIDOS", dfa1Prueba5,
                           dfa2Prueba5, validador, compatibilidad, erroresValidacion,
                           erroresCompatibilidad);

    DFA dfa1Valido;
    DFA dfa2Invalido;

    construirDFAUnEstado(dfa1Valido, "a", "", "", 1);
    dfa2Invalido.agregarEstado("q0");
    dfa2Invalido.agregarSimbolo("a");
    dfa2Invalido.establecerEstadoInicial("q99");
    dfa2Invalido.agregarTransicion("q0", "a", "q0");

    ejecutarCompatibilidad("PRUEBA 6 - BLOQUEO POR DFA INVALIDO", dfa1Valido,
                           dfa2Invalido, validador, compatibilidad, erroresValidacion,
                           erroresCompatibilidad);

    return 0;
}
