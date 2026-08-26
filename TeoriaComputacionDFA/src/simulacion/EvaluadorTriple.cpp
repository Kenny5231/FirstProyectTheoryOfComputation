#include "simulacion/EvaluadorTriple.h"

#include "estructuras/NodoSimboloCadena.h"
#include "simulacion/SimuladorDFA.h"
#include "simulacion/SimuladorDFAUnion.h"

bool EvaluadorTriple::evaluar(const DFA& dfa1,
                              const DFA& dfa2,
                              const DFAUnion& dfaUnion,
                              const CadenaEntrada& cadena,
                              ResultadoTriple& resultado) const {
    resultado = ResultadoTriple();

    SimuladorDFA simuladorDFA;
    SimuladorDFAUnion simuladorUnion;

    bool procesableDFA1 = simuladorDFA.simular(
        dfa1, cadena, resultado.aceptadaDFA1, resultado.estadoFinalDFA1);
    bool procesableDFA2 = simuladorDFA.simular(
        dfa2, cadena, resultado.aceptadaDFA2, resultado.estadoFinalDFA2);
    bool procesableUnion = simuladorUnion.simular(
        dfaUnion, cadena, resultado.aceptadaUnion,
        resultado.estadoFinalUnionDFA1, resultado.estadoFinalUnionDFA2);

    if (!procesableDFA1 || !procesableDFA2 || !procesableUnion) {
        resultado.procesable = false;
        resultado.unionConsistente = false;

        if (!buscarPrimerSimboloFueraDelAlfabeto(
                cadena, dfaUnion.obtenerAlfabeto(), resultado.simboloInvalido)) {
            buscarPrimerSimboloFueraDelAlfabeto(
                cadena, dfa1.obtenerAlfabeto(), resultado.simboloInvalido);
        }

        return false;
    }

    resultado.procesable = true;
    resultado.unionConsistente =
        resultado.aceptadaUnion ==
        (resultado.aceptadaDFA1 || resultado.aceptadaDFA2);

    return true;
}

bool EvaluadorTriple::buscarPrimerSimboloFueraDelAlfabeto(
    const CadenaEntrada& cadena,
    const ListaSimbolos& alfabeto,
    std::string& simboloInvalido) const {
    const NodoSimboloCadena* actual = cadena.obtenerPrimero();

    while (actual != nullptr) {
        if (!alfabeto.existe(actual->simbolo)) {
            simboloInvalido = actual->simbolo;
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}
