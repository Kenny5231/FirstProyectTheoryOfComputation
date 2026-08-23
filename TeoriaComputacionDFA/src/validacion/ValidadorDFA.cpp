#include "validacion/ValidadorDFA.h"

bool ValidadorDFA::validar(const DFA& dfa, ListaErrores& errores) const {
    errores.limpiar();

    validarNoVacuidad(dfa, errores);
    validarUnicidadEstados(dfa, errores);
    validarUnicidadAlfabeto(dfa, errores);
    validarSimbolosAlfabeto(dfa, errores);
    validarEstadoInicial(dfa, errores);
    validarEstadosFinales(dfa, errores);
    validarIntegridadTransiciones(dfa, errores);
    validarTotalidadYDeterminismo(dfa, errores);

    return errores.estaVacia();
}

void ValidadorDFA::validarNoVacuidad(const DFA& dfa, ListaErrores& errores) const {
    if (dfa.obtenerEstados().estaVacia()) {
        errores.insertar("El conjunto de estados esta vacio.");
    }

    if (dfa.obtenerAlfabeto().estaVacia()) {
        errores.insertar("El alfabeto esta vacio.");
    }
}

void ValidadorDFA::validarUnicidadEstados(const DFA& dfa, ListaErrores& errores) const {
    const NodoEstado* actual = dfa.obtenerEstados().obtenerPrimero();

    while (actual != nullptr) {
        const NodoEstado* comparador = actual->siguiente;

        while (comparador != nullptr) {
            if (actual->nombre == comparador->nombre) {
                errores.insertar("El estado '" + actual->nombre +
                                "' esta duplicado en el conjunto de estados.");
            }

            comparador = comparador->siguiente;
        }

        actual = actual->siguiente;
    }
}

void ValidadorDFA::validarUnicidadAlfabeto(const DFA& dfa, ListaErrores& errores) const {
    const NodoSimbolo* actual = dfa.obtenerAlfabeto().obtenerPrimero();

    while (actual != nullptr) {
        const NodoSimbolo* comparador = actual->siguiente;

        while (comparador != nullptr) {
            if (actual->simbolo == comparador->simbolo) {
                errores.insertar("El simbolo '" + actual->simbolo +
                                "' esta duplicado en el alfabeto.");
            }

            comparador = comparador->siguiente;
        }

        actual = actual->siguiente;
    }
}

void ValidadorDFA::validarSimbolosAlfabeto(const DFA& dfa, ListaErrores& errores) const {
    const NodoSimbolo* actual = dfa.obtenerAlfabeto().obtenerPrimero();

    while (actual != nullptr) {
        const std::string& simbolo = actual->simbolo;

        if (simbolo.empty()) {
            errores.insertar("El alfabeto contiene un simbolo vacio.");
        }

        if (simbolo == u8"ε" || simbolo == u8"λ") {
            errores.insertar("El simbolo '" + simbolo +
                            "' no esta permitido en el alfabeto de un DFA.");
        }

        if (contieneEspacioEnBlanco(simbolo)) {
            errores.insertar("El simbolo '" + simbolo +
                            "' contiene espacios en blanco y no es valido.");
        }

        if (contieneGuion(simbolo)) {
            errores.insertar("El simbolo '" + simbolo +
                            "' contiene un guion y no es valido.");
        }

        actual = actual->siguiente;
    }
}

void ValidadorDFA::validarEstadoInicial(const DFA& dfa, ListaErrores& errores) const {
    if (!dfa.tieneEstadoInicial()) {
        errores.insertar("El DFA no tiene un estado inicial definido.");
        return;
    }

    if (!contieneEstado(dfa.obtenerEstados(), dfa.obtenerEstadoInicial())) {
        errores.insertar("El estado inicial '" + dfa.obtenerEstadoInicial() +
                        "' no pertenece al conjunto de estados.");
    }
}

void ValidadorDFA::validarEstadosFinales(const DFA& dfa, ListaErrores& errores) const {
    const NodoEstado* actualFinal = dfa.obtenerEstadosFinales().obtenerPrimero();

    while (actualFinal != nullptr) {
        if (!contieneEstado(dfa.obtenerEstados(), actualFinal->nombre)) {
            errores.insertar("El estado final '" + actualFinal->nombre +
                            "' no pertenece al conjunto de estados.");
        }

        actualFinal = actualFinal->siguiente;
    }
}

void ValidadorDFA::validarIntegridadTransiciones(const DFA& dfa,
                                                 ListaErrores& errores) const {
    const NodoTransicion* actual = dfa.obtenerTransiciones().obtenerPrimero();

    while (actual != nullptr) {
        if (!contieneEstado(dfa.obtenerEstados(), actual->origen)) {
            errores.insertar("El estado de origen '" + actual->origen +
                            "' no esta registrado en el conjunto de estados.");
        }

        if (!contieneSimbolo(dfa.obtenerAlfabeto(), actual->simbolo)) {
            errores.insertar("El simbolo '" + actual->simbolo +
                            "' de una transicion no pertenece al alfabeto.");
        }

        if (!contieneEstado(dfa.obtenerEstados(), actual->destino)) {
            errores.insertar("El estado de destino '" + actual->destino +
                            "' no esta registrado en el conjunto de estados.");
        }

        actual = actual->siguiente;
    }
}

void ValidadorDFA::validarTotalidadYDeterminismo(const DFA& dfa,
                                                 ListaErrores& errores) const {
    const NodoEstado* estadoActual = dfa.obtenerEstados().obtenerPrimero();

    while (estadoActual != nullptr) {
        const NodoSimbolo* simboloActual = dfa.obtenerAlfabeto().obtenerPrimero();

        while (simboloActual != nullptr) {
            int cantidad = dfa.obtenerTransiciones().cantidadPorPar(estadoActual->nombre,
                                                                    simboloActual->simbolo);

            if (cantidad == 0) {
                errores.insertar("El estado '" + estadoActual->nombre +
                                "' carece de transicion para el simbolo '" +
                                simboloActual->simbolo + "'.");
            } else if (cantidad > 1) {
                errores.insertar("El estado '" + estadoActual->nombre +
                                "' tiene multiples transiciones para el simbolo '" +
                                simboloActual->simbolo + "'.");
            }

            simboloActual = simboloActual->siguiente;
        }

        estadoActual = estadoActual->siguiente;
    }
}

bool ValidadorDFA::contieneEstado(const ListaEstados& estados,
                                  const std::string& nombre) const {
    const NodoEstado* actual = estados.obtenerPrimero();

    while (actual != nullptr) {
        if (actual->nombre == nombre) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

bool ValidadorDFA::contieneSimbolo(const ListaSimbolos& alfabeto,
                                   const std::string& simbolo) const {
    const NodoSimbolo* actual = alfabeto.obtenerPrimero();

    while (actual != nullptr) {
        if (actual->simbolo == simbolo) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

bool ValidadorDFA::contieneEspacioEnBlanco(const std::string& texto) const {
    int indice = 0;

    while (indice < static_cast<int>(texto.size())) {
        char caracter = texto[indice];

        if (caracter == ' ' || caracter == '\t' || caracter == '\n' || caracter == '\r') {
            return true;
        }

        indice++;
    }

    return false;
}

bool ValidadorDFA::contieneGuion(const std::string& texto) const {
    int indice = 0;

    while (indice < static_cast<int>(texto.size())) {
        if (texto[indice] == '-') {
            return true;
        }

        indice++;
    }

    return false;
}
