#include <iostream>

#include "automata/DFA.h"
#include "validacion/ListaErrores.h"
#include "validacion/ValidadorDFA.h"

void ejecutarPrueba(const std::string& titulo, const DFA& dfa,
                   const ValidadorDFA& validador, ListaErrores& errores) {
    std::cout << titulo << std::endl;
    bool esValido = validador.validar(dfa, errores);

    std::cout << "Resultado: " << (esValido ? "DFA VALIDO" : "DFA INVALIDO")
              << std::endl;
    std::cout << "Cantidad de errores: " << errores.cantidad() << std::endl;

    if (!errores.estaVacia()) {
        errores.mostrar();
        std::cout << "NO APTO PARA OPERACIONES POSTERIORES" << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    ValidadorDFA validador;
    ListaErrores errores;

    DFA dfaUnicidad;

    bool primerEstado = dfaUnicidad.agregarEstado("q0");
    bool estadoDuplicado = dfaUnicidad.agregarEstado("q0");
    bool primerSimbolo = dfaUnicidad.agregarSimbolo("a");
    bool simboloDuplicado = dfaUnicidad.agregarSimbolo("a");

    std::cout << "PRUEBA DE UNICIDAD EN INSERCION" << std::endl;
    std::cout << "Primer estado q0 insertado: " << (primerEstado ? "SI" : "NO")
              << std::endl;
    std::cout << "Segundo estado q0 insertado: " << (estadoDuplicado ? "SI" : "NO")
              << std::endl;
    std::cout << "Primer simbolo a insertado: " << (primerSimbolo ? "SI" : "NO")
              << std::endl;
    std::cout << "Segundo simbolo a insertado: " << (simboloDuplicado ? "SI" : "NO")
              << std::endl;
    std::cout << std::endl;

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

    ejecutarPrueba("PRUEBA 1 - DFA VALIDO", dfaValido, validador, errores);

    std::cout << "Confirmacion estructura DFA valido:" << std::endl;
    std::cout << "Estados: " << dfaValido.obtenerEstados().cantidad() << std::endl;
    std::cout << "Simbolos: " << dfaValido.obtenerAlfabeto().cantidad() << std::endl;
    std::cout << "Estado inicial: " << dfaValido.obtenerEstadoInicial() << std::endl;
    std::cout << "Estados finales: " << dfaValido.obtenerEstadosFinales().cantidad()
              << std::endl;
    std::cout << "Transiciones: " << dfaValido.obtenerTransiciones().cantidad()
              << std::endl;
    std::cout << "Estado final vacio permitido en validacion: SI" << std::endl;
    std::cout << std::endl;

    DFA dfaSinFinales;

    dfaSinFinales.agregarEstado("q0");
    dfaSinFinales.agregarEstado("q1");
    dfaSinFinales.agregarSimbolo("a");
    dfaSinFinales.establecerEstadoInicial("q0");
    dfaSinFinales.agregarTransicion("q0", "a", "q1");
    dfaSinFinales.agregarTransicion("q1", "a", "q1");

    ejecutarPrueba("PRUEBA ADICIONAL - ESTADOS FINALES VACIOS", dfaSinFinales,
                   validador, errores);

    DFA dfaConErrores;

    dfaConErrores.agregarEstado("q0");
    dfaConErrores.agregarSimbolo("a");
    dfaConErrores.agregarSimbolo("b");
    dfaConErrores.establecerEstadoInicial("q99");
    dfaConErrores.agregarEstadoFinal("q88");
    dfaConErrores.agregarTransicion("q0", "a", "q77");
    dfaConErrores.agregarTransicion("q0", "a", "q0");

    ejecutarPrueba("PRUEBA 2 - MULTIPLES ERRORES", dfaConErrores, validador, errores);
    std::cout << "Cantidad por par (q0,a): "
              << dfaConErrores.obtenerTransiciones().cantidadPorPar("q0", "a")
              << std::endl;
    std::cout << std::endl;

    DFA dfaOrigenSimboloInvalidos;

    dfaOrigenSimboloInvalidos.agregarEstado("q0");
    dfaOrigenSimboloInvalidos.agregarSimbolo("a");
    dfaOrigenSimboloInvalidos.establecerEstadoInicial("q0");
    dfaOrigenSimboloInvalidos.agregarTransicion("q0", "a", "q0");
    dfaOrigenSimboloInvalidos.agregarTransicion("q5", "x", "q0");

    ejecutarPrueba("PRUEBA 3 - ORIGEN Y SIMBOLO INVALIDOS", dfaOrigenSimboloInvalidos,
                   validador, errores);

    DFA dfaSimboloProhibido;

    dfaSimboloProhibido.agregarEstado("q0");
    dfaSimboloProhibido.agregarSimbolo("-");
    dfaSimboloProhibido.establecerEstadoInicial("q0");
    dfaSimboloProhibido.agregarTransicion("q0", "-", "q0");

    ejecutarPrueba("PRUEBA 4 - SIMBOLO PROHIBIDO", dfaSimboloProhibido, validador,
                   errores);

    DFA dfaVacio;

    ejecutarPrueba("PRUEBA 5 - DFA VACIO", dfaVacio, validador, errores);

    return 0;
}
