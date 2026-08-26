# TeoriaComputacionDFA

## Objetivo general del proyecto
Desarrollar un Sistema de Automatas Finitos Deterministas (DFA) por fases, con el proposito de:
- Representar Automatas Finitos Deterministas (DFA).
- Validar estrictamente si un automata ingresado cumple las propiedades de un DFA.
- Generar reportes especificos cuando un DFA sea invalido.
- Realizar la operacion de union entre dos DFA validos.
- Construir el DFA union mediante pares de estados.
- Evaluar cadenas.
- Mostrar el recorrido paso a paso.
- Mostrar si la cadena fue aceptada o rechazada por DFA 1, DFA 2 y DFA Union.

## Tecnologias y configuracion
- Lenguaje: C++17.
- Sistema de construccion: CMake.

## Restricciones academicas obligatorias
- No utilizar colecciones nativas de C++, incluyendo:
	- `std::vector`
	- `std::list`
	- `std::map`
	- `std::unordered_map`
	- `std::set`
	- `std::unordered_set`
	- `std::stack`
	- `std::queue`
- No utilizar algoritmos o metodos nativos que sustituyan algoritmos manuales, por ejemplo:
	- `std::find`
	- `std::sort`
	- Metodos de busqueda automatica en colecciones
	- Expresiones regulares complejas para sustituir validaciones que deben programarse manualmente
- Las operaciones de busqueda, insercion, eliminacion, recorrido y verificacion de existencia sobre las estructuras del automata deben programarse manualmente.
- Crear manualmente estructuras de datos y algoritmos mediante nodos, punteros y logica propia, segun las fases del proyecto.

## Alcance de la fase actual
Actualmente estan implementadas manualmente:

FASE 2:
- Estados manuales.

FASE 3:
- Alfabeto manual.

FASE 4:
- Transiciones manuales.

FASE 5:
- Clase DFA.
- Representacion de Q.
- Representacion de Sigma.
- Representacion de delta.
- Representacion de q0.
- Representacion de F.
- Composicion utilizando las estructuras manuales existentes.

FASE 6:
- Validacion estricta completa.

FASE 7:
- Comparacion manual de alfabetos.
- Comparacion independiente del orden.
- Busqueda de simbolos de DFA1 en DFA2.
- Busqueda de simbolos de DFA2 en DFA1.
- Reporte especifico de discrepancias.
- Bloqueo de operacion cuando los alfabetos son diferentes.
- Requisito de que ambos DFA hayan sido previamente validados.

FASE 8:
- NodoEstadoCompuesto.
- ListaEstadosCompuestos.
- Representacion explicita de pares (estadoDFA1, estadoDFA2).
- Busqueda manual de pares.
- Insercion manual.
- Recorrido manual.
- Conteo manual.
- Liberacion manual de memoria.
- Generacion manual de Q1 x Q2.
- Producto cartesiano mediante dos recorridos enlazados anidados.

FASE 9:
- NodoTransicionCompuesta.
- ListaTransicionesCompuestas.
- Representacion manual de transiciones entre pares.
- Busqueda manual de delta1.
- Busqueda manual de delta2.
- Generacion simultanea de destinos.
- Implementacion de deltaU((qA,qB),a) = (delta1(qA,a),delta2(qB,a)).
- Generacion mediante recorridos manuales.
- Verificacion de cantidad de transiciones.

FASE 10:
- Generacion de estados finales compuestos.
- Reutilizacion de ListaEstadosCompuestos para FU.
- Busqueda manual en F1 mediante ListaEstados::existe().
- Busqueda manual en F2 mediante ListaEstados::existe().
- Criterio de aceptacion OR: (qA,qB) ∈ FU ⇔ qA ∈ F1 OR qB ∈ F2.
- Implementacion del criterio finalEnDFA1 || finalEnDFA2.
- Soporte cuando F1 es vacio.
- Soporte cuando F2 es vacio.
- Soporte cuando ambos conjuntos finales son vacios.
- Lista de finales compuestos generada sin crear nuevas estructuras de nodos.

FASE 11:
- DFAUnion.
- Representacion completa de MU = (QU, SigmaU, deltaU, q0U, FU).
- QU mediante ListaEstadosCompuestos.
- SigmaU mediante ListaSimbolos.
- deltaU mediante ListaTransicionesCompuestas.
- q0U mediante dos componentes explicitos (estadoInicialDFA1, estadoInicialDFA2).
- FU mediante ListaEstadosCompuestos.
- ConstructorDFAUnion.
- Reutilizacion de ProductoCartesiano.
- Reutilizacion de GeneradorTransicionesCompuestas.
- Reutilizacion de GeneradorEstadosFinalesCompuestos.
- Integracion completa de la operacion de union.

FASE 12:
- VisualizadorDFAUnion.
- Listado de QU.
- Listado de SigmaU.
- Estado inicial q0U.
- Listado de FU.
- Cantidades del DFA Union.
- Tabla/matriz de transiciones.
- Marcadores de estado inicial y estado final.
- Busqueda manual de destinos.
- Soporte de FU vacio.

FASE 13:
- NodoSimboloCadena.
- CadenaEntrada.
- Secuencia enlazada manual de simbolos.
- Soporte de simbolos repetidos.
- Soporte de cadena vacia epsilon.
- SimuladorDFA.
- SimuladorDFAUnion.
- Recorrido manual de delta.
- Recorrido manual de deltaU.
- Comprobacion de simbolos de entrada contra el alfabeto.
- Estado final alcanzado.
- Aceptacion y rechazo.
- Comprobacion de la propiedad Union = DFA1 OR DFA2.

FASE 14:
- NodoPasoDFA.
- ListaPasosDFA.
- NodoPasoDFAUnion.
- ListaPasosDFAUnion.
- simularConTraza en DFA.
- simularConTraza en DFAUnion.
- Registro de origen, simbolo y destino.
- Trazabilidad completa.
- Visualizacion paso a paso.
- Soporte de cadena vacia.
- Soporte de simulacion detenida por simbolo invalido.

Todavia NO estan implementados:
- Veredicto triple integrado.
- Veredicto triple definitivo.
- Entrada interactiva definitiva.
- Menu final.
