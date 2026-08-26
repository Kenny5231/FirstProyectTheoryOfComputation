#include "reportes/VisualizadorTrazabilidad.h"

#include <iostream>

#include "estructuras/NodoPasoDFA.h"
#include "estructuras/NodoPasoDFAUnion.h"

void VisualizadorTrazabilidad::mostrarDFA(
    const DFA& dfa,
    const ListaPasosDFA& pasos,
    bool procesable,
    bool aceptada,
    const std::string& estadoFinal) const {
    std::cout << "TRAZABILIDAD DFA" << std::endl;
    std::cout << "Estado inicial:" << std::endl;
    std::cout << dfa.obtenerEstadoInicial() << std::endl;

    if (!procesable) {
        std::cout << "SIMULACION DETENIDA" << std::endl;
    }

    if (pasos.estaVacia()) {
        std::cout << "No se procesaron simbolos." << std::endl;
    } else {
        const NodoPasoDFA* actual = pasos.obtenerPrimero();
        int numeroPaso = 1;

        while (actual != nullptr) {
            std::cout << "Paso " << numeroPaso << ":" << std::endl;
            std::cout << actual->origen << " --" << actual->simbolo
                      << "--> " << actual->destino << std::endl;
            actual = actual->siguiente;
            numeroPaso++;
        }
    }

    std::cout << "Estado alcanzado:" << std::endl;
    std::cout << estadoFinal << std::endl;
    std::cout << "Cantidad de pasos: " << pasos.cantidad() << std::endl;
    std::cout << "Resultado:" << std::endl;

    if (!procesable) {
        std::cout << "NO PROCESABLE" << std::endl;
    } else {
        std::cout << (aceptada ? "ACEPTADA" : "RECHAZADA") << std::endl;
    }
}

void VisualizadorTrazabilidad::mostrarDFAUnion(
    const DFAUnion& dfaUnion,
    const ListaPasosDFAUnion& pasos,
    bool procesable,
    bool aceptada,
    const std::string& estadoFinalDFA1,
    const std::string& estadoFinalDFA2) const {
    std::cout << "TRAZABILIDAD DFA UNION" << std::endl;
    std::cout << "Estado inicial:" << std::endl;
    std::cout << "(" << dfaUnion.obtenerEstadoInicialDFA1() << ","
              << dfaUnion.obtenerEstadoInicialDFA2() << ")" << std::endl;

    if (!procesable) {
        std::cout << "SIMULACION DETENIDA" << std::endl;
    }

    if (pasos.estaVacia()) {
        std::cout << "No se procesaron simbolos." << std::endl;
    } else {
        const NodoPasoDFAUnion* actual = pasos.obtenerPrimero();
        int numeroPaso = 1;

        while (actual != nullptr) {
            std::cout << "Paso " << numeroPaso << ":" << std::endl;
            std::cout << "(" << actual->origenDFA1 << ","
                      << actual->origenDFA2 << ") --" << actual->simbolo
                      << "--> (" << actual->destinoDFA1 << ","
                      << actual->destinoDFA2 << ")" << std::endl;
            actual = actual->siguiente;
            numeroPaso++;
        }
    }

    std::cout << "Estado alcanzado:" << std::endl;
    std::cout << "(" << estadoFinalDFA1 << "," << estadoFinalDFA2 << ")"
              << std::endl;
    std::cout << "Cantidad de pasos: " << pasos.cantidad() << std::endl;
    std::cout << "Resultado:" << std::endl;

    if (!procesable) {
        std::cout << "NO PROCESABLE" << std::endl;
    } else {
        std::cout << (aceptada ? "ACEPTADA" : "RECHAZADA") << std::endl;
    }
}
