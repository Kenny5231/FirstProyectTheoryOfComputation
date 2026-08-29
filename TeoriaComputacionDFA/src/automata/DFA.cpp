#include "automata/DFA.h"

DFA::DFA() : estadoInicial(""), estadoInicialDefinido(false) {}

DFA::~DFA() = default;

bool DFA::agregarEstado(const std::string& nombre) {
    return estados.insertar(nombre);
}

bool DFA::agregarSimbolo(const std::string& simbolo) {
    return alfabeto.insertar(simbolo);
}

bool DFA::agregarTransicion(const std::string& origen, const std::string& simbolo,
                           const std::string& destino) {
    return transiciones.insertar(origen, simbolo, destino);
}

bool DFA::eliminarTransicion(const std::string& origen, const std::string& simbolo,
                             const std::string& destino) {
    return transiciones.eliminarTransicionExacta(origen, simbolo, destino);
}

void DFA::establecerEstadoInicial(const std::string& nombre) {
    estadoInicial = nombre;
    estadoInicialDefinido = true;
}

bool DFA::tieneEstadoInicial() const {
    return estadoInicialDefinido;
}

const std::string& DFA::obtenerEstadoInicial() const {
    return estadoInicial;
}

bool DFA::agregarEstadoFinal(const std::string& nombre) {
    return estadosFinales.insertar(nombre);
}

const ListaEstados& DFA::obtenerEstados() const {
    return estados;
}

const ListaSimbolos& DFA::obtenerAlfabeto() const {
    return alfabeto;
}

const ListaTransiciones& DFA::obtenerTransiciones() const {
    return transiciones;
}

const ListaEstados& DFA::obtenerEstadosFinales() const {
    return estadosFinales;
}
