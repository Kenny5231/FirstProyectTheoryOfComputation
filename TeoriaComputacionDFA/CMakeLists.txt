cmake_minimum_required(VERSION 3.16)

project(TeoriaComputacionDFA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SistemaDFA
	src/main.cpp
	src/automata/DFA.cpp
	src/automata/DFAUnion.cpp
	src/estructuras/CadenaEntrada.cpp
	src/operaciones/CompatibilidadDFA.cpp
	src/operaciones/ConstructorDFAUnion.cpp
	src/operaciones/GeneradorEstadosFinalesCompuestos.cpp
	src/operaciones/GeneradorTransicionesCompuestas.cpp
	src/operaciones/ProductoCartesiano.cpp
	src/simulacion/SimuladorDFA.cpp
	src/simulacion/SimuladorDFAUnion.cpp
	src/estructuras/ListaEstados.cpp
	src/estructuras/ListaEstadosCompuestos.cpp
	src/estructuras/ListaSimbolos.cpp
	src/estructuras/ListaTransicionesCompuestas.cpp
	src/estructuras/ListaTransiciones.cpp
	src/validacion/ListaErrores.cpp
	src/validacion/ValidadorDFA.cpp
)

target_include_directories(SistemaDFA PRIVATE include)
