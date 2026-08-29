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

FASE 15:
- ResultadoTriple.
- EvaluadorTriple.
- Evaluacion simultanea DFA1/DFA2/DFAUnion.
- Veredicto triple.
- Estados finales alcanzados.
- Diferenciacion entre RECHAZADA y NO PROCESABLE.
- Deteccion manual del simbolo invalido real.
- Comprobacion Union = DFA1 OR DFA2.

## Interfaz grafica
El producto final utilizara Qt 6 + Qt Widgets.

Las siguientes fases estaran orientadas a construir la interfaz grafica.
Qt se utilizara para presentacion e interaccion con el usuario.
Qt NO sustituira las estructuras manuales del proyecto.
No se utilizaran colecciones Qt para almacenar los automatas.

Todavia NO estan implementados:
- Entrada interactiva definitiva.
- Menu final.

## FASE 16 - Base de interfaz grafica
- Qt 6 y Qt Widgets.
- `MainWindow` y `QApplication`.
- Navegacion lateral mediante `QStackedWidget`.
- Paginas de Inicio, DFA 1, DFA 2, DFA Union y Prueba.
- Diseno adaptable mediante layouts, sin posicionamiento absoluto.
- Separacion entre la interfaz grafica y la logica del automata.
- Respaldo del main de consola en `src/pruebas/main_consola_fases1_15.cpp`.
- Motor existente de las fases 1 a 15 preservado.

La interfaz grafica de la Fase 17 permite construir DFA 1 y DFA 2. La validacion formal desde GUI comienza en la Fase 18.

## FASE 17 - Editor grafico de DFA
- `EditorDFAWidget` reutilizable para DFA 1 y DFA 2.
- DFA 1 y DFA 2 almacenados como miembros independientes de `MainWindow`.
- Ingreso grafico de estados, simbolos, estado inicial, estados finales y transiciones.
- Tabla visual de transiciones reconstruida desde `ListaTransiciones`.
- Combos y tabla utilizados solamente como presentacion y seleccion.
- Actualizacion de la interfaz mediante recorridos manuales de las listas enlazadas.
- Sin colecciones STL o Qt para almacenar DFA.
- Motor logico de las fases anteriores preservado.

La validacion formal desde la interfaz grafica se implementara en la Fase 18.

## FASE 18 - Validacion grafica
- Boton `Validar DFA` conectado directamente con `ValidadorDFA`.
- Uso de `ListaErrores` y recorrido manual de sus nodos.
- Panel grafico para mostrar multiples errores simultaneamente.
- Estados de interfaz `Pendiente`, `DFA valido` y `DFA invalido`.
- Invalidacion automatica despues de modificar el DFA.
- El conjunto de estados finales vacio sigue permitido por el validador.
- La GUI no duplica reglas de validacion ni almacena los errores en colecciones Qt.

La union grafica se implementa en la Fase 19.

## FASE 19 - DFA Union grafico
- `VistaUnionDFAWidget` para construir y mostrar la union.
- Prevalidacion mediante `EditorDFAWidget::esDFAValido()`.
- Comparacion de alfabetos mediante `CompatibilidadDFA`.
- Construccion mediante `ConstructorDFAUnion`.
- Propiedad segura y regenerable de `DFAUnion`.
- Visualizacion manual de QU, ΣU, q0U, FU y la matriz de transiciones.
- Invalidacion automatica cuando se modifica DFA 1 o DFA 2.
- `QTableWidget` utilizado solamente como presentacion.

La prueba grafica de cadenas se implementa en la Fase 20.

## FASE 20 - Prueba grafica de cadenas
- `VistaPruebaCadenaWidget` para evaluar una secuencia desde la interfaz.
- Parser manual de simbolos, sin `QString::split()` ni `QStringList`.
- Soporte de simbolos de varios caracteres y cadena vacia epsilon.
- Integracion con `EvaluadorTriple` para el veredicto de DFA 1, DFA 2 y DFA Union.
- Trazabilidad grafica mediante `SimuladorDFA`, `SimuladorDFAUnion`, `ListaPasosDFA` y `ListaPasosDFAUnion`.
- Estados alcanzados, consistencia de la union y cadenas no procesables.
- Invalidacion automatica de resultados cuando cambian los DFA o la union.

Despues de esta fase quedan unicamente integracion, pulido y pruebas finales.

## FASE 21 - Visualizacion grafica de automatas
- `VisualizadorAutomataWidget` con `QGraphicsView` y `QGraphicsScene` como presentacion.
- Posiciones manuales para estados simples y compuestos.
- Distribucion automatica circular y tratamiento especial para uno o dos estados.
- Estados iniciales, finales, transiciones, ciclos y aristas curvas.
- Visualizacion de DFA 1, DFA 2 y DFA Union.
- Resaltado del recorrido del DFA Union en la prueba de cadenas.
- Navegacion paso a paso sin colecciones STL o Qt para almacenar el automata.
- Las fuentes de verdad siguen siendo `DFA` y `DFAUnion`.

Correccion visual de Fase 21:
- Las etiquetas de transiciones usan su tamano real para quedar centradas.
- Los simbolos de self-loops se colocan por encima del arco.
- Cada etiqueta tiene un fondo claro y un orden Z que evita que quede oculta por los estados.
- Los simbolos resaltados conservan el color verde durante la trazabilidad.

## PRUEBA INDIVIDUAL DE CADENAS (correccion de Fase 20)
- La prueba de una cadena se realiza SOBRE UN SOLO AUTOMATA seleccionable (DFA 1 o DFA 2).
- Solamente el DFA seleccionado necesita estar valido para poder evaluar.
- Ya no se depende de que DFA 2 este valido ni de que el DFA Union este generado.
- El estado del otro DFA no afecta la disponibilidad de la prueba individual.
- Se utiliza `SimuladorDFA` (con `simularConTraza`) sobre el DFA seleccionado.
- Se muestra el desarrollo matematico paso a paso de la funcion de transicion extendida δ̂.
- Se muestra la pertenencia del estado alcanzado al conjunto de estados finales F.
- Se concluye formalmente si la cadena w pertenece a L(M) (ACEPTADA) o no (RECHAZADA).
- Se diferencia la cadena NO PROCESABLE (simbolo fuera del alfabeto) de la cadena RECHAZADA.
- Se conserva el recorrido textual (RECORRIDO) y la visualizacion grafica paso a paso del DFA seleccionado.
- La prueba del DFA Union se desarrollara por separado en una correccion posterior.
