#ifndef VALIDADORDFA_H
#define VALIDADORDFA_H

#include <string>

#include "automata/DFA.h"
#include "validacion/ListaErrores.h"

class ValidadorDFA {
public:
    bool validar(const DFA& dfa, ListaErrores& errores) const;

private:
    void validarNoVacuidad(const DFA& dfa, ListaErrores& errores) const;
    void validarUnicidadEstados(const DFA& dfa, ListaErrores& errores) const;
    void validarUnicidadAlfabeto(const DFA& dfa, ListaErrores& errores) const;
    void validarSimbolosAlfabeto(const DFA& dfa, ListaErrores& errores) const;
    void validarEstadoInicial(const DFA& dfa, ListaErrores& errores) const;
    void validarEstadosFinales(const DFA& dfa, ListaErrores& errores) const;
    void validarIntegridadTransiciones(const DFA& dfa, ListaErrores& errores) const;
    void validarTotalidadYDeterminismo(const DFA& dfa, ListaErrores& errores) const;

    bool contieneEstado(const ListaEstados& estados, const std::string& nombre) const;
    bool contieneSimbolo(const ListaSimbolos& alfabeto, const std::string& simbolo) const;
    bool contieneEspacioEnBlanco(const std::string& texto) const;
    bool contieneGuion(const std::string& texto) const;
};

#endif
