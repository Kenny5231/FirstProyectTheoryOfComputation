#include <iostream>

#include "automata/DFA.h"
#include "automata/DFAUnion.h"
#include "operaciones/CompatibilidadDFA.h"
#include "operaciones/ConstructorDFAUnion.h"
#include "operaciones/GeneradorEstadosFinalesCompuestos.h"
#include "operaciones/GeneradorTransicionesCompuestas.h"
#include "operaciones/ProductoCartesiano.h"
#include "reportes/VisualizadorDFAUnion.h"
#include "reportes/VisualizadorTrazabilidad.h"
#include "reportes/VisualizadorVeredictoTriple.h"
#include "estructuras/CadenaEntrada.h"
#include "estructuras/ListaEstadosCompuestos.h"
#include "estructuras/ListaPasosDFA.h"
#include "estructuras/ListaPasosDFAUnion.h"
#include "estructuras/ListaTransicionesCompuestas.h"
#include "simulacion/EvaluadorTriple.h"
#include "simulacion/ResultadoTriple.h"
#include "simulacion/SimuladorDFA.h"
#include "simulacion/SimuladorDFAUnion.h"
#include "validacion/ListaErrores.h"
#include "validacion/ValidadorDFA.h"

void construirDFAFase9_1(DFA& dfa) {
    dfa.agregarEstado("q0");
    dfa.agregarEstado("q1");
    dfa.agregarSimbolo("a");
    dfa.agregarSimbolo("b");
    dfa.establecerEstadoInicial("q0");
    dfa.agregarEstadoFinal("q1");

    dfa.agregarTransicion("q0", "a", "q1");
    dfa.agregarTransicion("q0", "b", "q0");
    dfa.agregarTransicion("q1", "a", "q1");
    dfa.agregarTransicion("q1", "b", "q0");
}

void construirDFAFase9_2(DFA& dfa) {
    dfa.agregarEstado("p0");
    dfa.agregarEstado("p1");
    dfa.agregarEstado("p2");
    dfa.agregarSimbolo("a");
    dfa.agregarSimbolo("b");
    dfa.establecerEstadoInicial("p0");
    dfa.agregarEstadoFinal("p2");

    dfa.agregarTransicion("p0", "a", "p1");
    dfa.agregarTransicion("p0", "b", "p0");
    dfa.agregarTransicion("p1", "a", "p2");
    dfa.agregarTransicion("p1", "b", "p0");
    dfa.agregarTransicion("p2", "a", "p2");
    dfa.agregarTransicion("p2", "b", "p1");
}

void construirDFAIncompatibles(DFA& dfa1, DFA& dfa2) {
    dfa1.agregarEstado("q0");
    dfa1.agregarSimbolo("a");
    dfa1.agregarSimbolo("b");
    dfa1.establecerEstadoInicial("q0");
    dfa1.agregarEstadoFinal("q0");
    dfa1.agregarTransicion("q0", "a", "q0");
    dfa1.agregarTransicion("q0", "b", "q0");

    dfa2.agregarEstado("p0");
    dfa2.agregarSimbolo("a");
    dfa2.agregarSimbolo("c");
    dfa2.establecerEstadoInicial("p0");
    dfa2.agregarEstadoFinal("p0");
    dfa2.agregarTransicion("p0", "a", "p0");
    dfa2.agregarTransicion("p0", "c", "p0");
}

void construirDFAInvalido(DFA& dfaValido, DFA& dfaInvalido) {
    dfaValido.agregarEstado("q0");
    dfaValido.agregarSimbolo("a");
    dfaValido.establecerEstadoInicial("q0");
    dfaValido.agregarEstadoFinal("q0");
    dfaValido.agregarTransicion("q0", "a", "q0");

    dfaInvalido.agregarEstado("p0");
    dfaInvalido.agregarSimbolo("a");
    dfaInvalido.establecerEstadoInicial("p99");
    dfaInvalido.agregarTransicion("p0", "a", "p0");
}

bool validarDFA(const std::string& nombre, const DFA& dfa,
                const ValidadorDFA& validador, ListaErrores& errores) {
    bool valido = validador.validar(dfa, errores);
    std::cout << nombre << " valido: " << (valido ? "SI" : "NO") << std::endl;

    if (!valido) {
        errores.mostrar();
    }

    return valido;
}

bool verificarValidezYCompatibilidad(const DFA& dfa1, const DFA& dfa2,
                                     const ValidadorDFA& validador,
                                     const CompatibilidadDFA& compatibilidad,
                                     ListaErrores& erroresValidacion,
                                     ListaErrores& erroresCompatibilidad,
                                     bool& bloqueoPorInvalidez,
                                     bool& bloqueoPorIncompatibilidad) {
    bloqueoPorInvalidez = false;
    bloqueoPorIncompatibilidad = false;

    bool dfa1Valido = validarDFA("DFA 1", dfa1, validador, erroresValidacion);
    bool dfa2Valido = validarDFA("DFA 2", dfa2, validador, erroresValidacion);

    if (!dfa1Valido || !dfa2Valido) {
        std::cout << "OPERACION BLOQUEADA: ambos DFA deben ser validos." << std::endl;
        bloqueoPorInvalidez = true;
        return false;
    }

    bool compatibles = compatibilidad.compararAlfabetos(dfa1, dfa2, erroresCompatibilidad);

    std::cout << "Resultado de compatibilidad: "
              << (compatibles ? "ALFABETOS COMPATIBLES" : "ALFABETOS INCOMPATIBLES")
              << std::endl;

    if (!compatibles) {
        erroresCompatibilidad.mostrar();
        bloqueoPorIncompatibilidad = true;
        return false;
    }

    return true;
}

void ejecutarPruebaPrincipalFase9(const DFA& dfa1, const DFA& dfa2,
                                  const ValidadorDFA& validador,
                                  const CompatibilidadDFA& compatibilidad,
                                  const ProductoCartesiano& producto,
                                  const GeneradorTransicionesCompuestas& generador,
                                  ListaErrores& erroresValidacion,
                                  ListaErrores& erroresCompatibilidad,
                                  ListaEstadosCompuestos& estadosCompuestos,
                                  ListaTransicionesCompuestas& transicionesCompuestas) {
    std::cout << "PRUEBA PRINCIPAL FASE 9" << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    if (!verificarValidezYCompatibilidad(dfa1, dfa2, validador, compatibilidad,
                                         erroresValidacion, erroresCompatibilidad,
                                         bloqueoPorInvalidez,
                                         bloqueoPorIncompatibilidad)) {
        std::cout << "PRODUCTO CARTESIANO NO GENERADO." << std::endl;
        std::cout << "TRANSICIONES COMPUESTAS NO GENERADAS." << std::endl;
        std::cout << std::endl;
        return;
    }

    producto.generar(dfa1, dfa2, estadosCompuestos);

    std::cout << "Estados compuestos generados:" << std::endl;
    estadosCompuestos.mostrar();

    bool transicionesGeneradas =
        generador.generar(dfa1, dfa2, estadosCompuestos, transicionesCompuestas);

    if (!transicionesGeneradas) {
        std::cout << "ERROR INTERNO: no se encontro una transicion esperada."
                  << std::endl;
        std::cout << std::endl;
        return;
    }

    std::cout << "Transiciones compuestas generadas:" << std::endl;
    transicionesCompuestas.mostrar();

    int cantidadEstadosCompuestos = estadosCompuestos.cantidad();
    int cantidadSimbolos = dfa1.obtenerAlfabeto().cantidad();
    int cantidadEsperada = cantidadEstadosCompuestos * cantidadSimbolos;
    int cantidadObtenida = transicionesCompuestas.cantidad();

    std::cout << "Cantidad de estados compuestos: " << cantidadEstadosCompuestos
              << std::endl;
    std::cout << "Cantidad de simbolos: " << cantidadSimbolos << std::endl;
    std::cout << "Cantidad esperada de transiciones: " << cantidadEsperada
              << std::endl;
    std::cout << "Cantidad obtenida de transiciones: " << cantidadObtenida
              << std::endl;

    std::cout << "Existe (q0,p0) --a--> (q1,p1): "
              << (transicionesCompuestas.existeTransicionExacta("q0", "p0", "a",
                                                               "q1", "p1")
                      ? "SI"
                      : "NO")
              << std::endl;
    std::cout << "Existe (q0,p0) --a--> (q0,p0): "
              << (transicionesCompuestas.existeTransicionExacta("q0", "p0", "a",
                                                               "q0", "p0")
                      ? "SI"
                      : "NO")
              << std::endl;
    std::cout << "Existe (q1,p2) --b--> (q0,p1): "
              << (transicionesCompuestas.existeTransicionExacta("q1", "p2", "b",
                                                               "q0", "p1")
                      ? "SI"
                      : "NO")
              << std::endl;

    std::cout << std::endl;
}

void ejecutarPruebaBloqueo(const std::string& titulo, const DFA& dfa1,
                           const DFA& dfa2, const ValidadorDFA& validador,
                           const CompatibilidadDFA& compatibilidad,
                           ListaErrores& erroresValidacion,
                           ListaErrores& erroresCompatibilidad) {
    std::cout << titulo << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    bool sePuedeAvanzar = verificarValidezYCompatibilidad(
        dfa1, dfa2, validador, compatibilidad, erroresValidacion,
        erroresCompatibilidad, bloqueoPorInvalidez, bloqueoPorIncompatibilidad);

    if (!sePuedeAvanzar) {
        if (bloqueoPorIncompatibilidad) {
            std::cout << "PRODUCTO CARTESIANO NO GENERADO: alfabetos incompatibles."
                      << std::endl;
        } else {
            std::cout << "PRODUCTO CARTESIANO NO GENERADO." << std::endl;
        }

        std::cout << "TRANSICIONES COMPUESTAS NO GENERADAS." << std::endl;
        std::cout << std::endl;
        return;
    }

    std::cout << "La prueba de bloqueo no debia avanzar, revisar datos." << std::endl;

    std::cout << std::endl;
}

void ejecutarPruebaPrincipalFase10(const DFA& dfa1, const DFA& dfa2,
                                  const ValidadorDFA& validador,
                                  const CompatibilidadDFA& compatibilidad,
                                  const ProductoCartesiano& producto,
                                  const GeneradorTransicionesCompuestas& generador,
                                  const GeneradorEstadosFinalesCompuestos& generadorFinales,
                                  ListaErrores& erroresValidacion,
                                  ListaErrores& erroresCompatibilidad,
                                  ListaEstadosCompuestos& estadosCompuestos,
                                  ListaTransicionesCompuestas& transicionesCompuestas,
                                  ListaEstadosCompuestos& estadosFinalesCompuestos) {
    std::cout << "PRUEBA PRINCIPAL FASE 10 - ESTADOS FINALES DEL DFA UNION"
              << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    if (!verificarValidezYCompatibilidad(dfa1, dfa2, validador, compatibilidad,
                                         erroresValidacion, erroresCompatibilidad,
                                         bloqueoPorInvalidez,
                                         bloqueoPorIncompatibilidad)) {
        std::cout << "PRODUCTO CARTESIANO NO GENERADO." << std::endl;
        std::cout << "TRANSICIONES COMPUESTAS NO GENERADAS." << std::endl;
        std::cout << "ESTADOS FINALES COMPUESTOS NO GENERADOS." << std::endl;
        std::cout << std::endl;
        return;
    }

    producto.generar(dfa1, dfa2, estadosCompuestos);
    bool transicionesGeneradas =
        generador.generar(dfa1, dfa2, estadosCompuestos, transicionesCompuestas);

    if (!transicionesGeneradas) {
        std::cout << "ERROR INTERNO: no se encontro una transicion esperada."
                  << std::endl;
        std::cout << std::endl;
        return;
    }

    generadorFinales.generar(dfa1, dfa2, estadosCompuestos, estadosFinalesCompuestos);

    std::cout << "Estados finales compuestos generados:" << std::endl;
    estadosFinalesCompuestos.mostrar();

    std::cout << "Cantidad esperada: 4" << std::endl;
    std::cout << "Cantidad obtenida: " << estadosFinalesCompuestos.cantidad()
              << std::endl;

    std::cout << "Existe (q0,p2): "
              << (estadosFinalesCompuestos.existe("q0", "p2") ? "SI" : "NO")
              << std::endl;
    std::cout << "Existe (q1,p0): "
              << (estadosFinalesCompuestos.existe("q1", "p0") ? "SI" : "NO")
              << std::endl;
    std::cout << "Existe (q0,p0): "
              << (estadosFinalesCompuestos.existe("q0", "p0") ? "SI" : "NO")
              << std::endl;
    std::cout << "Existe (q1,p2): "
              << (estadosFinalesCompuestos.existe("q1", "p2") ? "SI" : "NO")
              << std::endl;
    std::cout << std::endl;
}

void ejecutarPruebaPrincipalFase11(const DFA& dfa1, const DFA& dfa2,
                                   const ValidadorDFA& validador,
                                   const CompatibilidadDFA& compatibilidad,
                                   const ConstructorDFAUnion& constructorUnion,
                                   ListaErrores& erroresValidacion,
                                   ListaErrores& erroresCompatibilidad) {
    std::cout << "PRUEBA PRINCIPAL FASE 11 - INTEGRACION DFA UNION COMPLETO"
              << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    if (!verificarValidezYCompatibilidad(dfa1, dfa2, validador, compatibilidad,
                                         erroresValidacion, erroresCompatibilidad,
                                         bloqueoPorInvalidez,
                                         bloqueoPorIncompatibilidad)) {
        std::cout << "DFA UNION NO CONSTRUIDO." << std::endl;
        std::cout << std::endl;
        return;
    }

    DFAUnion dfaUnion;
    bool construido = constructorUnion.construir(dfa1, dfa2, dfaUnion);

    if (!construido) {
        std::cout << "ERROR: no fue posible construir el DFA Union." << std::endl;
        std::cout << std::endl;
        return;
    }

    std::cout << "# ==============================" << std::endl;
    std::cout << "DFA UNION" << std::endl;
    std::cout << "Estados QU:" << std::endl;
    dfaUnion.obtenerEstados().mostrar();

    std::cout << "Alfabeto:" << std::endl;
    dfaUnion.obtenerAlfabeto().mostrar();

    std::cout << "Estado inicial:" << std::endl;
    if (dfaUnion.tieneEstadoInicial()) {
        std::cout << "(" << dfaUnion.obtenerEstadoInicialDFA1() << ","
                  << dfaUnion.obtenerEstadoInicialDFA2() << ")" << std::endl;
    } else {
        std::cout << "NO DEFINIDO" << std::endl;
    }

    std::cout << "Estados finales FU:" << std::endl;
    dfaUnion.obtenerEstadosFinales().mostrar();

    std::cout << "Transiciones:" << std::endl;
    dfaUnion.obtenerTransiciones().mostrar();

    std::cout << "Cantidad de estados: " << dfaUnion.obtenerEstados().cantidad()
              << std::endl;
    std::cout << "Cantidad de simbolos: " << dfaUnion.obtenerAlfabeto().cantidad()
              << std::endl;
    std::cout << "Cantidad de estados finales: "
              << dfaUnion.obtenerEstadosFinales().cantidad() << std::endl;
    std::cout << "Cantidad de transiciones: "
              << dfaUnion.obtenerTransiciones().cantidad() << std::endl;

    std::cout << "tieneEstadoInicial() == true: "
              << (dfaUnion.tieneEstadoInicial() ? "SI" : "NO") << std::endl;
    std::cout << "obtenerEstadoInicialDFA1() == q0: "
              << (dfaUnion.obtenerEstadoInicialDFA1() == "q0" ? "SI" : "NO")
              << std::endl;
    std::cout << "obtenerEstadoInicialDFA2() == p0: "
              << (dfaUnion.obtenerEstadoInicialDFA2() == "p0" ? "SI" : "NO")
              << std::endl;

    std::cout << "Existe estado (q0,p0) en QU: "
              << (dfaUnion.obtenerEstados().existe("q0", "p0") ? "SI" : "NO")
              << std::endl;
    std::cout << "Existe final (q0,p2) en FU: "
              << (dfaUnion.obtenerEstadosFinales().existe("q0", "p2") ? "SI"
                                                                      : "NO")
              << std::endl;
    std::cout << "Existe final (q0,p0) en FU: "
              << (dfaUnion.obtenerEstadosFinales().existe("q0", "p0") ? "SI"
                                                                      : "NO")
              << std::endl;

    std::cout << "Existe (q0,p0) --a--> (q1,p1): "
              << (dfaUnion.obtenerTransiciones().existeTransicionExacta(
                      "q0", "p0", "a", "q1", "p1")
                      ? "SI"
                      : "NO")
              << std::endl;
    std::cout << std::endl;
}

void ejecutarPruebaEstadoInicialYFinalFase12(
    const VisualizadorDFAUnion& visualizador) {
    std::cout << "PRUEBA FASE 12 - ESTADO INICIAL Y FINAL" << std::endl;

    DFAUnion dfaUnion;
    dfaUnion.agregarEstado("q0", "p0");
    dfaUnion.agregarSimbolo("a");
    dfaUnion.establecerEstadoInicial("q0", "p0");
    dfaUnion.agregarEstadoFinal("q0", "p0");
    dfaUnion.agregarTransicion("q0", "p0", "a", "q0", "p0");

    visualizador.mostrarComponentes(dfaUnion);
    visualizador.mostrarTablaTransiciones(dfaUnion);
    std::cout << std::endl;
}

void ejecutarPruebaFUVacioFase12(const VisualizadorDFAUnion& visualizador) {
    std::cout << "PRUEBA FASE 12 - FU VACIO" << std::endl;

    DFAUnion dfaUnion;
    dfaUnion.agregarEstado("q0", "p0");
    dfaUnion.agregarSimbolo("a");
    dfaUnion.establecerEstadoInicial("q0", "p0");
    dfaUnion.agregarTransicion("q0", "p0", "a", "q0", "p0");

    visualizador.mostrarComponentes(dfaUnion);
    visualizador.mostrarTablaTransiciones(dfaUnion);
    std::cout << std::endl;
}

void ejecutarPruebaPrincipalFase12(const DFA& dfa1, const DFA& dfa2,
                                   const ValidadorDFA& validador,
                                   const CompatibilidadDFA& compatibilidad,
                                   const ConstructorDFAUnion& constructorUnion,
                                   ListaErrores& erroresValidacion,
                                   ListaErrores& erroresCompatibilidad) {
    std::cout << "PRUEBA PRINCIPAL FASE 12 - VISUALIZACION DFA UNION"
              << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    if (!verificarValidezYCompatibilidad(dfa1, dfa2, validador, compatibilidad,
                                         erroresValidacion, erroresCompatibilidad,
                                         bloqueoPorInvalidez,
                                         bloqueoPorIncompatibilidad)) {
        std::cout << "VISUALIZACION BLOQUEADA." << std::endl;
        std::cout << std::endl;
        return;
    }

    DFAUnion dfaUnion;
    bool construido = constructorUnion.construir(dfa1, dfa2, dfaUnion);

    if (!construido) {
        std::cout << "ERROR: no fue posible construir el DFA Union." << std::endl;
        std::cout << std::endl;
        return;
    }

    VisualizadorDFAUnion visualizador;
    visualizador.mostrarComponentes(dfaUnion);
    visualizador.mostrarTablaTransiciones(dfaUnion);
    std::cout << std::endl;

    ejecutarPruebaEstadoInicialYFinalFase12(visualizador);
    ejecutarPruebaFUVacioFase12(visualizador);
}

void mostrarResultadoDFA(const std::string& nombre, bool procesable,
                         bool aceptada, const std::string& estadoFinal) {
    std::cout << nombre << ": ";

    if (!procesable) {
        std::cout << "CADENA NO PROCESABLE" << std::endl;
        return;
    }

    std::cout << "estado final = " << estadoFinal << " | "
              << (aceptada ? "ACEPTADA" : "RECHAZADA") << std::endl;
}

void mostrarResultadoUnion(bool procesable, bool aceptada,
                           const std::string& estadoFinalDFA1,
                           const std::string& estadoFinalDFA2) {
    std::cout << "DFA Union: ";

    if (!procesable) {
        std::cout << "CADENA NO PROCESABLE" << std::endl;
        return;
    }

    std::cout << "estado final = (" << estadoFinalDFA1 << ","
              << estadoFinalDFA2 << ") | "
              << (aceptada ? "ACEPTADA" : "RECHAZADA") << std::endl;
}

void ejecutarCasoSimulacion(const std::string& nombreCadena,
                            const CadenaEntrada& cadena,
                            const DFA& dfa1,
                            const DFA& dfa2,
                            const DFAUnion& dfaUnion,
                            const SimuladorDFA& simuladorDFA,
                            const SimuladorDFAUnion& simuladorUnion,
                            bool comprobarPropiedadUnion) {
    std::cout << "Cadena de prueba: " << nombreCadena << std::endl;
    cadena.mostrar();

    if (cadena.estaVacia()) {
        std::cout << "Cadena vacia: no se buscan transiciones; se evalua q0."
                  << std::endl;
    }

    bool aceptadaDFA1 = false;
    bool aceptadaDFA2 = false;
    bool aceptadaUnion = false;
    std::string estadoFinalDFA1;
    std::string estadoFinalDFA2;
    std::string estadoFinalUnionDFA1;
    std::string estadoFinalUnionDFA2;

    bool procesableDFA1 =
        simuladorDFA.simular(dfa1, cadena, aceptadaDFA1, estadoFinalDFA1);
    bool procesableDFA2 =
        simuladorDFA.simular(dfa2, cadena, aceptadaDFA2, estadoFinalDFA2);
    bool procesableUnion = simuladorUnion.simular(
        dfaUnion, cadena, aceptadaUnion, estadoFinalUnionDFA1,
        estadoFinalUnionDFA2);

    mostrarResultadoDFA("DFA 1", procesableDFA1, aceptadaDFA1, estadoFinalDFA1);
    mostrarResultadoDFA("DFA 2", procesableDFA2, aceptadaDFA2, estadoFinalDFA2);
    mostrarResultadoUnion(procesableUnion, aceptadaUnion, estadoFinalUnionDFA1,
                          estadoFinalUnionDFA2);

    if (!procesableDFA1 || !procesableDFA2 || !procesableUnion) {
        std::cout << "CADENA NO PROCESABLE: existe un simbolo fuera del alfabeto."
                  << std::endl;
    }

    if (comprobarPropiedadUnion && procesableDFA1 && procesableDFA2 &&
        procesableUnion) {
        bool propiedadUnion = aceptadaUnion == (aceptadaDFA1 || aceptadaDFA2);

        std::cout << "Union == (DFA1 || DFA2): "
                  << (propiedadUnion ? "SI" : "NO") << std::endl;
    }

    std::cout << std::endl;
}

void ejecutarPruebaPrincipalFase13(const DFA& dfa1, const DFA& dfa2,
                                   const ValidadorDFA& validador,
                                   const CompatibilidadDFA& compatibilidad,
                                   const ConstructorDFAUnion& constructorUnion,
                                   ListaErrores& erroresValidacion,
                                   ListaErrores& erroresCompatibilidad) {
    std::cout << "PRUEBA PRINCIPAL FASE 13 - SIMULACION DE CADENAS"
              << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    if (!verificarValidezYCompatibilidad(dfa1, dfa2, validador, compatibilidad,
                                         erroresValidacion, erroresCompatibilidad,
                                         bloqueoPorInvalidez,
                                         bloqueoPorIncompatibilidad)) {
        std::cout << "SIMULACION BLOQUEADA." << std::endl;
        std::cout << std::endl;
        return;
    }

    DFAUnion dfaUnion;
    bool construido = constructorUnion.construir(dfa1, dfa2, dfaUnion);

    if (!construido) {
        std::cout << "ERROR: no fue posible construir el DFA Union." << std::endl;
        std::cout << std::endl;
        return;
    }

    SimuladorDFA simuladorDFA;
    SimuladorDFAUnion simuladorUnion;

    CadenaEntrada cadenaA;
    cadenaA.agregarSimbolo("a");
    ejecutarCasoSimulacion("a", cadenaA, dfa1, dfa2, dfaUnion, simuladorDFA,
                           simuladorUnion, true);

    CadenaEntrada cadenaAA;
    cadenaAA.agregarSimbolo("a");
    cadenaAA.agregarSimbolo("a");
    ejecutarCasoSimulacion("aa", cadenaAA, dfa1, dfa2, dfaUnion, simuladorDFA,
                           simuladorUnion, true);

    CadenaEntrada cadenaB;
    cadenaB.agregarSimbolo("b");
    ejecutarCasoSimulacion("b", cadenaB, dfa1, dfa2, dfaUnion, simuladorDFA,
                           simuladorUnion, true);

    CadenaEntrada cadenaVacia;
    ejecutarCasoSimulacion("epsilon", cadenaVacia, dfa1, dfa2, dfaUnion,
                           simuladorDFA, simuladorUnion, true);

    CadenaEntrada cadenaInvalida;
    cadenaInvalida.agregarSimbolo("a");
    cadenaInvalida.agregarSimbolo("x");
    cadenaInvalida.agregarSimbolo("b");
    ejecutarCasoSimulacion("a x b", cadenaInvalida, dfa1, dfa2, dfaUnion,
                           simuladorDFA, simuladorUnion, false);
}

void ejecutarCasoTrazabilidad(const std::string& nombreCadena,
                              const CadenaEntrada& cadena,
                              const DFA& dfa1,
                              const DFA& dfa2,
                              const DFAUnion& dfaUnion,
                              const SimuladorDFA& simuladorDFA,
                              const SimuladorDFAUnion& simuladorUnion,
                              const VisualizadorTrazabilidad& visualizador) {
    std::cout << "Cadena de prueba con traza: " << nombreCadena << std::endl;
    cadena.mostrar();

    ListaPasosDFA pasosDFA1;
    ListaPasosDFA pasosDFA2;
    ListaPasosDFAUnion pasosUnion;
    bool aceptadaDFA1 = false;
    bool aceptadaDFA2 = false;
    bool aceptadaUnion = false;
    std::string estadoFinalDFA1;
    std::string estadoFinalDFA2;
    std::string estadoFinalUnionDFA1;
    std::string estadoFinalUnionDFA2;

    bool procesableDFA1 = simuladorDFA.simularConTraza(
        dfa1, cadena, pasosDFA1, aceptadaDFA1, estadoFinalDFA1);
    bool procesableDFA2 = simuladorDFA.simularConTraza(
        dfa2, cadena, pasosDFA2, aceptadaDFA2, estadoFinalDFA2);
    bool procesableUnion = simuladorUnion.simularConTraza(
        dfaUnion, cadena, pasosUnion, aceptadaUnion, estadoFinalUnionDFA1,
        estadoFinalUnionDFA2);

    visualizador.mostrarDFA(dfa1, pasosDFA1, procesableDFA1, aceptadaDFA1,
                            estadoFinalDFA1);
    visualizador.mostrarDFA(dfa2, pasosDFA2, procesableDFA2, aceptadaDFA2,
                            estadoFinalDFA2);
    visualizador.mostrarDFAUnion(dfaUnion, pasosUnion, procesableUnion,
                                 aceptadaUnion, estadoFinalUnionDFA1,
                                 estadoFinalUnionDFA2);

    std::cout << "Cantidad pasos DFA 1: " << pasosDFA1.cantidad() << std::endl;
    std::cout << "Cantidad pasos DFA 2: " << pasosDFA2.cantidad() << std::endl;
    std::cout << "Cantidad pasos DFA Union: " << pasosUnion.cantidad()
              << std::endl;

    if (!procesableDFA1 || !procesableDFA2 || !procesableUnion) {
        std::cout << "La simulacion se detuvo antes de crear pasos falsos."
                  << std::endl;
    }

    std::cout << std::endl;
}

void ejecutarPruebaPrincipalFase14(const DFA& dfa1, const DFA& dfa2,
                                   const ValidadorDFA& validador,
                                   const CompatibilidadDFA& compatibilidad,
                                   const ConstructorDFAUnion& constructorUnion,
                                   ListaErrores& erroresValidacion,
                                   ListaErrores& erroresCompatibilidad) {
    std::cout << "PRUEBA PRINCIPAL FASE 14 - TRAZABILIDAD PASO A PASO"
              << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    if (!verificarValidezYCompatibilidad(dfa1, dfa2, validador, compatibilidad,
                                         erroresValidacion, erroresCompatibilidad,
                                         bloqueoPorInvalidez,
                                         bloqueoPorIncompatibilidad)) {
        std::cout << "TRAZABILIDAD BLOQUEADA." << std::endl;
        std::cout << std::endl;
        return;
    }

    DFAUnion dfaUnion;
    bool construido = constructorUnion.construir(dfa1, dfa2, dfaUnion);

    if (!construido) {
        std::cout << "ERROR: no fue posible construir el DFA Union." << std::endl;
        std::cout << std::endl;
        return;
    }

    SimuladorDFA simuladorDFA;
    SimuladorDFAUnion simuladorUnion;
    VisualizadorTrazabilidad visualizador;

    CadenaEntrada cadenaAAB;
    cadenaAAB.agregarSimbolo("a");
    cadenaAAB.agregarSimbolo("a");
    cadenaAAB.agregarSimbolo("b");
    ejecutarCasoTrazabilidad("aab", cadenaAAB, dfa1, dfa2, dfaUnion,
                             simuladorDFA, simuladorUnion, visualizador);

    CadenaEntrada cadenaAA;
    cadenaAA.agregarSimbolo("a");
    cadenaAA.agregarSimbolo("a");
    ejecutarCasoTrazabilidad("aa", cadenaAA, dfa1, dfa2, dfaUnion,
                             simuladorDFA, simuladorUnion, visualizador);

    CadenaEntrada cadenaVacia;
    ejecutarCasoTrazabilidad("epsilon", cadenaVacia, dfa1, dfa2, dfaUnion,
                             simuladorDFA, simuladorUnion, visualizador);

    CadenaEntrada cadenaInvalida;
    cadenaInvalida.agregarSimbolo("a");
    cadenaInvalida.agregarSimbolo("x");
    cadenaInvalida.agregarSimbolo("b");
    ejecutarCasoTrazabilidad("a x b", cadenaInvalida, dfa1, dfa2, dfaUnion,
                             simuladorDFA, simuladorUnion, visualizador);

    CadenaEntrada cadenaRepetida;
    cadenaRepetida.agregarSimbolo("a");
    cadenaRepetida.agregarSimbolo("a");
    cadenaRepetida.agregarSimbolo("a");
    ejecutarCasoTrazabilidad("a a a", cadenaRepetida, dfa1, dfa2, dfaUnion,
                             simuladorDFA, simuladorUnion, visualizador);
}

void ejecutarCasoVeredictoTriple(const std::string& nombreCadena,
                                 const CadenaEntrada& cadena,
                                 const DFA& dfa1,
                                 const DFA& dfa2,
                                 const DFAUnion& dfaUnion,
                                 const EvaluadorTriple& evaluador,
                                 const VisualizadorVeredictoTriple& visualizador) {
    std::cout << "Cadena de prueba con veredicto triple: " << nombreCadena
              << std::endl;

    ResultadoTriple resultado;
    evaluador.evaluar(dfa1, dfa2, dfaUnion, cadena, resultado);
    visualizador.mostrar(cadena, resultado);

    if (resultado.procesable) {
        std::cout << "Union == (DFA1 || DFA2): "
                  << (resultado.unionConsistente ? "SI" : "NO") << std::endl;
    }

    std::cout << std::endl;
}

void ejecutarPruebaPrincipalFase15(const DFA& dfa1, const DFA& dfa2,
                                   const ValidadorDFA& validador,
                                   const CompatibilidadDFA& compatibilidad,
                                   const ConstructorDFAUnion& constructorUnion,
                                   ListaErrores& erroresValidacion,
                                   ListaErrores& erroresCompatibilidad) {
    std::cout << "PRUEBA PRINCIPAL FASE 15 - VEREDICTO TRIPLE DE CADENAS"
              << std::endl;

    bool bloqueoPorInvalidez = false;
    bool bloqueoPorIncompatibilidad = false;

    if (!verificarValidezYCompatibilidad(dfa1, dfa2, validador, compatibilidad,
                                         erroresValidacion, erroresCompatibilidad,
                                         bloqueoPorInvalidez,
                                         bloqueoPorIncompatibilidad)) {
        std::cout << "VEREDICTO TRIPLE BLOQUEADO." << std::endl;
        std::cout << std::endl;
        return;
    }

    DFAUnion dfaUnion;
    bool construido = constructorUnion.construir(dfa1, dfa2, dfaUnion);

    if (!construido) {
        std::cout << "ERROR: no fue posible construir el DFA Union." << std::endl;
        std::cout << std::endl;
        return;
    }

    EvaluadorTriple evaluador;
    VisualizadorVeredictoTriple visualizador;

    CadenaEntrada cadenaA;
    cadenaA.agregarSimbolo("a");
    ejecutarCasoVeredictoTriple("a", cadenaA, dfa1, dfa2, dfaUnion, evaluador,
                                visualizador);

    CadenaEntrada cadenaAA;
    cadenaAA.agregarSimbolo("a");
    cadenaAA.agregarSimbolo("a");
    ejecutarCasoVeredictoTriple("aa", cadenaAA, dfa1, dfa2, dfaUnion,
                                evaluador, visualizador);

    CadenaEntrada cadenaB;
    cadenaB.agregarSimbolo("b");
    ejecutarCasoVeredictoTriple("b", cadenaB, dfa1, dfa2, dfaUnion, evaluador,
                                visualizador);

    CadenaEntrada cadenaAAB;
    cadenaAAB.agregarSimbolo("a");
    cadenaAAB.agregarSimbolo("a");
    cadenaAAB.agregarSimbolo("b");
    ejecutarCasoVeredictoTriple("aab", cadenaAAB, dfa1, dfa2, dfaUnion,
                                evaluador, visualizador);

    CadenaEntrada cadenaInvalida;
    cadenaInvalida.agregarSimbolo("a");
    cadenaInvalida.agregarSimbolo("z");
    cadenaInvalida.agregarSimbolo("b");
    ejecutarCasoVeredictoTriple("a z b", cadenaInvalida, dfa1, dfa2, dfaUnion,
                                evaluador, visualizador);

    CadenaEntrada cadenaVacia;
    ejecutarCasoVeredictoTriple("epsilon", cadenaVacia, dfa1, dfa2, dfaUnion,
                                evaluador, visualizador);
}

int main() {
    ValidadorDFA validador;
    CompatibilidadDFA compatibilidad;
    ProductoCartesiano producto;
    GeneradorTransicionesCompuestas generador;
    GeneradorEstadosFinalesCompuestos generadorFinales;
    ConstructorDFAUnion constructorUnion;
    ListaErrores erroresValidacion;
    ListaErrores erroresCompatibilidad;
    ListaEstadosCompuestos estadosCompuestos;
    ListaTransicionesCompuestas transicionesCompuestas;
    ListaEstadosCompuestos estadosFinalesCompuestos;

    DFA dfa1Principal;
    DFA dfa2Principal;
    construirDFAFase9_1(dfa1Principal);
    construirDFAFase9_2(dfa2Principal);

    ejecutarPruebaPrincipalFase9(dfa1Principal, dfa2Principal, validador,
                                 compatibilidad, producto, generador,
                                 erroresValidacion, erroresCompatibilidad,
                                 estadosCompuestos, transicionesCompuestas);

    ejecutarPruebaPrincipalFase10(dfa1Principal, dfa2Principal, validador,
                                  compatibilidad, producto, generador,
                                  generadorFinales, erroresValidacion,
                                  erroresCompatibilidad, estadosCompuestos,
                                  transicionesCompuestas,
                                  estadosFinalesCompuestos);

    ejecutarPruebaPrincipalFase11(dfa1Principal, dfa2Principal, validador,
                                  compatibilidad, constructorUnion,
                                  erroresValidacion, erroresCompatibilidad);

    ejecutarPruebaPrincipalFase12(dfa1Principal, dfa2Principal, validador,
                                  compatibilidad, constructorUnion,
                                  erroresValidacion, erroresCompatibilidad);

    ejecutarPruebaPrincipalFase13(dfa1Principal, dfa2Principal, validador,
                                  compatibilidad, constructorUnion,
                                  erroresValidacion, erroresCompatibilidad);

    ejecutarPruebaPrincipalFase14(dfa1Principal, dfa2Principal, validador,
                                  compatibilidad, constructorUnion,
                                  erroresValidacion, erroresCompatibilidad);

    ejecutarPruebaPrincipalFase15(dfa1Principal, dfa2Principal, validador,
                                  compatibilidad, constructorUnion,
                                  erroresValidacion, erroresCompatibilidad);

    DFA dfaIncompatible1;
    DFA dfaIncompatible2;
    construirDFAIncompatibles(dfaIncompatible1, dfaIncompatible2);

    ejecutarPruebaBloqueo("PRUEBA BLOQUEO - ALFABETOS INCOMPATIBLES",
                          dfaIncompatible1, dfaIncompatible2, validador,
                          compatibilidad, erroresValidacion,
                          erroresCompatibilidad);

    DFA dfaValido;
    DFA dfaInvalido;
    construirDFAInvalido(dfaValido, dfaInvalido);

    ejecutarPruebaBloqueo("PRUEBA BLOQUEO - DFA INVALIDO", dfaValido,
                          dfaInvalido, validador, compatibilidad,
                          erroresValidacion, erroresCompatibilidad);

    return 0;
}
