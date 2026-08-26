#ifndef NODOPASODFA_H
#define NODOPASODFA_H

#include <string>

struct NodoPasoDFA {
    std::string origen;
    std::string simbolo;
    std::string destino;
    NodoPasoDFA* siguiente;

    NodoPasoDFA(const std::string& origenEntrada,
                const std::string& simboloEntrada,
                const std::string& destinoEntrada)
        : origen(origenEntrada), simbolo(simboloEntrada),
          destino(destinoEntrada), siguiente(nullptr) {}
};

#endif
