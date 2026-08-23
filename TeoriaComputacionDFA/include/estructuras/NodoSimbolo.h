#ifndef NODOSIMBOLO_H
#define NODOSIMBOLO_H

#include <string>

struct NodoSimbolo {
    std::string simbolo;
    NodoSimbolo* siguiente;

    explicit NodoSimbolo(const std::string& simboloEntrada)
        : simbolo(simboloEntrada), siguiente(nullptr) {}
};

#endif
