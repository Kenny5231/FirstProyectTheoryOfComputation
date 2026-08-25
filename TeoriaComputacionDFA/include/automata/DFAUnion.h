#ifndef DFAUNION_H
#define DFAUNION_H

#include <string>

#include "estructuras/ListaEstadosCompuestos.h"
#include "estructuras/ListaSimbolos.h"
#include "estructuras/ListaTransicionesCompuestas.h"

class DFAUnion {
private:
    ListaEstadosCompuestos estados;
    ListaSimbolos alfabeto;
    ListaTransicionesCompuestas transiciones;
    std::string estadoInicialDFA1;
    std::string estadoInicialDFA2;
    bool estadoInicialDefinido;
    ListaEstadosCompuestos estadosFinales;

public:
    DFAUnion();
    ~DFAUnion() = default;
    DFAUnion(const DFAUnion&) = delete;
    DFAUnion& operator=(const DFAUnion&) = delete;

    bool agregarEstado(const std::string& estadoDFA1, const std::string& estadoDFA2);
    bool agregarSimbolo(const std::string& simbolo);
    bool agregarTransicion(const std::string& origenDFA1,
                           const std::string& origenDFA2,
                           const std::string& simbolo,
                           const std::string& destinoDFA1,
                           const std::string& destinoDFA2);

    void establecerEstadoInicial(const std::string& estadoDFA1,
                                 const std::string& estadoDFA2);
    bool tieneEstadoInicial() const;
    const std::string& obtenerEstadoInicialDFA1() const;
    const std::string& obtenerEstadoInicialDFA2() const;

    bool agregarEstadoFinal(const std::string& estadoDFA1,
                            const std::string& estadoDFA2);

    const ListaEstadosCompuestos& obtenerEstados() const;
    const ListaSimbolos& obtenerAlfabeto() const;
    const ListaTransicionesCompuestas& obtenerTransiciones() const;
    const ListaEstadosCompuestos& obtenerEstadosFinales() const;

    bool estaVacio() const;
};

#endif
