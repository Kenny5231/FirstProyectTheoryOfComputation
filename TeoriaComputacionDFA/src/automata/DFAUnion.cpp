#include "automata/DFAUnion.h"

DFAUnion::DFAUnion()
    : estadoInicialDFA1(""), estadoInicialDFA2(""), estadoInicialDefinido(false) {}

bool DFAUnion::agregarEstado(const std::string& estadoDFA1,
                             const std::string& estadoDFA2) {
    return estados.insertar(estadoDFA1, estadoDFA2);
}

bool DFAUnion::agregarSimbolo(const std::string& simbolo) {
    return alfabeto.insertar(simbolo);
}

bool DFAUnion::agregarTransicion(const std::string& origenDFA1,
                                 const std::string& origenDFA2,
                                 const std::string& simbolo,
                                 const std::string& destinoDFA1,
                                 const std::string& destinoDFA2) {
    return transiciones.insertar(origenDFA1, origenDFA2, simbolo, destinoDFA1,
                                 destinoDFA2);
}

void DFAUnion::establecerEstadoInicial(const std::string& estadoDFA1,
                                       const std::string& estadoDFA2) {
    estadoInicialDFA1 = estadoDFA1;
    estadoInicialDFA2 = estadoDFA2;
    estadoInicialDefinido = true;
}

bool DFAUnion::tieneEstadoInicial() const {
    return estadoInicialDefinido;
}

const std::string& DFAUnion::obtenerEstadoInicialDFA1() const {
    return estadoInicialDFA1;
}

const std::string& DFAUnion::obtenerEstadoInicialDFA2() const {
    return estadoInicialDFA2;
}

bool DFAUnion::agregarEstadoFinal(const std::string& estadoDFA1,
                                  const std::string& estadoDFA2) {
    return estadosFinales.insertar(estadoDFA1, estadoDFA2);
}

const ListaEstadosCompuestos& DFAUnion::obtenerEstados() const {
    return estados;
}

const ListaSimbolos& DFAUnion::obtenerAlfabeto() const {
    return alfabeto;
}

const ListaTransicionesCompuestas& DFAUnion::obtenerTransiciones() const {
    return transiciones;
}

const ListaEstadosCompuestos& DFAUnion::obtenerEstadosFinales() const {
    return estadosFinales;
}

bool DFAUnion::estaVacio() const {
    return estados.estaVacia();
}
