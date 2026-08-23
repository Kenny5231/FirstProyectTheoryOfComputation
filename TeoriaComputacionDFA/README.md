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
Esta fase solo prepara la estructura base del proyecto. No implementa DFA, estados, transiciones, alfabetos, listas enlazadas, validadores, union de automatas ni simulacion de cadenas.
