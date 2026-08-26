cmake_minimum_required(VERSION 3.16)

project(TeoriaComputacionDFA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(SistemaDFA
	src/main.cpp
	src/gui/MainWindow.cpp
	src/gui/EditorDFAWidget.cpp
	src/gui/VistaUnionDFAWidget.cpp
	src/gui/VistaPruebaCadenaWidget.cpp
	include/gui/MainWindow.h
	include/gui/EditorDFAWidget.h
	include/gui/VistaUnionDFAWidget.h
	include/gui/VistaPruebaCadenaWidget.h
	src/automata/DFA.cpp
	src/automata/DFAUnion.cpp
	src/estructuras/CadenaEntrada.cpp
	src/estructuras/ListaPasosDFA.cpp
	src/estructuras/ListaPasosDFAUnion.cpp
	src/operaciones/CompatibilidadDFA.cpp
	src/operaciones/ConstructorDFAUnion.cpp
	src/operaciones/GeneradorEstadosFinalesCompuestos.cpp
	src/operaciones/GeneradorTransicionesCompuestas.cpp
	src/operaciones/ProductoCartesiano.cpp
	src/reportes/VisualizadorDFAUnion.cpp
	src/reportes/VisualizadorTrazabilidad.cpp
	src/reportes/VisualizadorVeredictoTriple.cpp
	src/simulacion/EvaluadorTriple.cpp
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

target_link_libraries(SistemaDFA PRIVATE Qt6::Widgets)
