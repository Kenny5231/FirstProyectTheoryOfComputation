#ifndef DFA_H
#define DFA_H

#include <string>

#include "estructuras/ListaEstados.h"
#include "estructuras/ListaSimbolos.h"
#include "estructuras/ListaTransiciones.h"

class DFA {
private:
    ListaEstados estados;
    ListaSimbolos alfabeto;
    ListaTransiciones transiciones;
    std::string estadoInicial;
    bool estadoInicialDefinido;
    ListaEstados estadosFinales;

public:
    DFA();
    ~DFA();
    DFA(const DFA&) = delete;
    DFA& operator=(const DFA&) = delete;

    bool agregarEstado(const std::string& nombre);
    bool agregarSimbolo(const std::string& simbolo);
    bool agregarTransicion(const std::string& origen, const std::string& simbolo,
                           const std::string& destino);

    void establecerEstadoInicial(const std::string& nombre);
    bool tieneEstadoInicial() const;
    const std::string& obtenerEstadoInicial() const;

    bool agregarEstadoFinal(const std::string& nombre);

    const ListaEstados& obtenerEstados() const;
    const ListaSimbolos& obtenerAlfabeto() const;
    const ListaTransiciones& obtenerTransiciones() const;
    const ListaEstados& obtenerEstadosFinales() const;
};

#endif
