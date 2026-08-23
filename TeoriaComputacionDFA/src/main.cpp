#include <iostream>

#include "automata/DFA.h"

int main() {
    DFA dfaValido;

    dfaValido.agregarEstado("q0");
    dfaValido.agregarEstado("q1");
    dfaValido.agregarEstado("q2");

    dfaValido.agregarSimbolo("a");
    dfaValido.agregarSimbolo("b");

    dfaValido.establecerEstadoInicial("q0");
    dfaValido.agregarEstadoFinal("q2");

    dfaValido.agregarTransicion("q0", "a", "q1");
    dfaValido.agregarTransicion("q0", "b", "q0");
    dfaValido.agregarTransicion("q1", "a", "q2");
    dfaValido.agregarTransicion("q1", "b", "q0");
    dfaValido.agregarTransicion("q2", "a", "q2");
    dfaValido.agregarTransicion("q2", "b", "q2");

    std::cout << "FASE 5 - DFA VALIDO (ESTRUCTURAL)" << std::endl;
    std::cout << std::endl;

    dfaValido.obtenerEstados().mostrar();
    std::cout << std::endl;

    dfaValido.obtenerAlfabeto().mostrar();
    std::cout << std::endl;

    std::cout << "Estado inicial definido: "
              << (dfaValido.tieneEstadoInicial() ? "SI" : "NO") << std::endl;
    std::cout << "Estado inicial: " << dfaValido.obtenerEstadoInicial() << std::endl;

    std::cout << std::endl;

    std::cout << "Estados finales:" << std::endl;
    const NodoEstado* actualFinal = dfaValido.obtenerEstadosFinales().obtenerPrimero();
    while (actualFinal != nullptr) {
        std::cout << actualFinal->nombre << std::endl;
        actualFinal = actualFinal->siguiente;
    }

    std::cout << std::endl;

    dfaValido.obtenerTransiciones().mostrar();
    std::cout << std::endl;

    std::cout << "Cantidad de estados: " << dfaValido.obtenerEstados().cantidad()
              << std::endl;
    std::cout << "Cantidad de simbolos: " << dfaValido.obtenerAlfabeto().cantidad()
              << std::endl;
    std::cout << "Cantidad de estados finales: "
              << dfaValido.obtenerEstadosFinales().cantidad() << std::endl;
    std::cout << "Cantidad de transiciones: "
              << dfaValido.obtenerTransiciones().cantidad() << std::endl;

    std::cout << std::endl;

    DFA dfaCandidatoInvalido;

    dfaCandidatoInvalido.agregarEstado("q0");
    dfaCandidatoInvalido.agregarSimbolo("a");
    dfaCandidatoInvalido.establecerEstadoInicial("q99");
    dfaCandidatoInvalido.agregarEstadoFinal("q88");
    dfaCandidatoInvalido.agregarTransicion("q0", "a", "q77");
    dfaCandidatoInvalido.agregarTransicion("q0", "a", "q0");

    std::cout << "FASE 5 - DFA CANDIDATO (SIN VALIDACION FORMAL)" << std::endl;
    std::cout << std::endl;

    dfaCandidatoInvalido.obtenerEstados().mostrar();
    std::cout << std::endl;

    dfaCandidatoInvalido.obtenerAlfabeto().mostrar();
    std::cout << std::endl;

    std::cout << "Estado inicial definido: "
              << (dfaCandidatoInvalido.tieneEstadoInicial() ? "SI" : "NO") << std::endl;
    std::cout << "Estado inicial almacenado: "
              << dfaCandidatoInvalido.obtenerEstadoInicial() << std::endl;
    std::cout << std::endl;

    std::cout << "Estados finales almacenados:" << std::endl;
    const NodoEstado* finalInvalido = dfaCandidatoInvalido.obtenerEstadosFinales().obtenerPrimero();
    while (finalInvalido != nullptr) {
        std::cout << finalInvalido->nombre << std::endl;
        finalInvalido = finalInvalido->siguiente;
    }
    std::cout << std::endl;

    dfaCandidatoInvalido.obtenerTransiciones().mostrar();
    std::cout << std::endl;

    std::cout << "Cantidad por par (q0,a) en candidato: "
              << dfaCandidatoInvalido.obtenerTransiciones().cantidadPorPar("q0", "a")
              << std::endl;

    return 0;
}
