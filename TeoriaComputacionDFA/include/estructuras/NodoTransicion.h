#ifndef NODOTRANSICION_H
#define NODOTRANSICION_H

#include <string>

struct NodoTransicion {
    std::string origen;
    std::string simbolo;
    std::string destino;
    NodoTransicion* siguiente;

    NodoTransicion(const std::string& origenEntrada,
                   const std::string& simboloEntrada,
                   const std::string& destinoEntrada)
        : origen(origenEntrada), simbolo(simboloEntrada), destino(destinoEntrada),
          siguiente(nullptr) {}
};

#endif
