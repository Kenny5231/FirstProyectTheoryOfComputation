#ifndef NODOSIMBOLOCADENA_H
#define NODOSIMBOLOCADENA_H

#include <string>

struct NodoSimboloCadena {
    std::string simbolo;
    NodoSimboloCadena* siguiente;

    NodoSimboloCadena(const std::string& simboloEntrada)
        : simbolo(simboloEntrada), siguiente(nullptr) {}
};

#endif
