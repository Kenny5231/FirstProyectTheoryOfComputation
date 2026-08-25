#ifndef NODOTRANSICIONCOMPUESTA_H
#define NODOTRANSICIONCOMPUESTA_H

#include <string>

struct NodoTransicionCompuesta {
    std::string origenDFA1;
    std::string origenDFA2;
    std::string simbolo;
    std::string destinoDFA1;
    std::string destinoDFA2;
    NodoTransicionCompuesta* siguiente;

    NodoTransicionCompuesta(const std::string& origen1, const std::string& origen2,
                            const std::string& simboloEntrada,
                            const std::string& destino1,
                            const std::string& destino2)
        : origenDFA1(origen1), origenDFA2(origen2), simbolo(simboloEntrada),
          destinoDFA1(destino1), destinoDFA2(destino2), siguiente(nullptr) {}
};

#endif
