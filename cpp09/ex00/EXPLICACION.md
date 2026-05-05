# ex00 - Bitcoin Exchange

## 1. Conceptos clave del ejercicio

### STL y `std::map`
En este ejercicio el concepto central es el uso de la STL, y en concreto de `std::map`.

`std::map` es un contenedor asociativo que guarda pares clave-valor ordenados por clave. En este caso:
- la clave es la fecha en formato `YYYY-MM-DD`
- el valor es el precio del bitcoin en esa fecha

Es importante en programación porque permite:
- buscar información por clave de forma eficiente
- mantener los datos ordenados automáticamente
- resolver problemas reales en los que una entrada identifica un valor asociado

Aquí es especialmente útil porque no solo necesitamos buscar una fecha exacta, sino también la fecha válida más cercana por debajo cuando la fecha pedida no existe exactamente.

### Parsing de archivos
El programa trabaja con dos archivos:
- `data.csv`, que actúa como base de datos
- el archivo de entrada pasado por argumento

Parsear significa leer texto y transformarlo en datos útiles para el programa. Es un concepto muy importante porque muchos programas reales trabajan leyendo ficheros, logs, CSV, JSON o entradas de usuario.

En este ejercicio se practica:
- abrir y recorrer archivos con `std::ifstream`
- separar una línea por delimitadores
- convertir texto a números
- validar formato antes de usar la información

### Validación de datos
No basta con leer una fecha o un número: hay que comprobar que sean válidos.

Aquí se valida:
- que la fecha tenga el formato correcto
- que el mes y el día sean coherentes
- que se respeten los años bisiestos
- que el valor sea numérico
- que el valor esté entre `0` y `1000`

Esto es muy importante en programación porque una gran parte de los errores reales no vienen del algoritmo, sino de asumir que la entrada siempre es correcta.

### Búsqueda de la fecha más cercana inferior
El subject pide que, si una fecha no existe en la base de datos, se use la fecha válida más cercana inferior.

Ese detalle es importante porque obliga a pensar no solo en "buscar si existe", sino en "buscar la mejor aproximación válida". Con `std::map` esto se resuelve muy bien con `lower_bound`.

En programación este tipo de problema aparece muchísimo:
- tarifas por fecha
- versiones válidas de configuración
- históricos de precios
- snapshots de datos

### Separación de responsabilidades
En este ejercicio se ve una estructura simple pero correcta:
- `main.cpp` valida argumentos e inicia el flujo
- `BitcoinExchange.cpp` contiene la lógica del problema
- `BitcoinExchange.hpp` define la interfaz de la clase

Esto es importante porque ayuda a escribir código mantenible, reutilizable y más fácil de depurar.

---

## 2. Explicación del ejercicio

### Qué pide el ejercicio
El programa debe:
- cargar una base de datos histórica de precios de bitcoin desde `data.csv`
- leer un archivo de entrada con líneas del tipo `date | value`
- para cada línea válida, calcular cuánto valdría esa cantidad de bitcoin en esa fecha

Si la fecha no existe exactamente en la base de datos, hay que usar la fecha anterior más cercana.

### Cómo está resuelto en este proyecto

#### `main.cpp`
El `main` tiene una responsabilidad mínima:
- comprobar que se pasa exactamente un argumento
- crear un objeto `BitcoinExchange`
- cargar la base de datos
- procesar el archivo de entrada

Esta decisión es buena porque evita meter toda la lógica en `main`, lo que haría el programa más difícil de explicar y mantener.

#### Clase `BitcoinExchange`
La clase mantiene un atributo:
- `_map`, de tipo `std::map<std::string, float>`

Se eligió `std::map` porque:
- ordena las fechas automáticamente
- permite búsqueda por clave
- permite usar `lower_bound` para localizar una fecha exacta o la inmediatamente posterior, y desde ahí retroceder a la anterior si hace falta

### Carga de la base de datos
El método `load_csv`:
- abre el archivo CSV
- ignora la cabecera
- lee cada línea
- separa fecha y valor usando la coma
- convierte el valor a `float`
- inserta el par en `_map`

La idea importante aquí es que la base de datos queda preparada una sola vez y luego se reutiliza para todas las consultas.

### Procesamiento del archivo de entrada
El método `process_input`:
- abre el archivo recibido por parámetro
- comprueba que la base de datos no esté vacía
- ignora la cabecera
- procesa línea por línea sin abortar en el primer error

Esto es importante porque el subject pide que el programa siga trabajando aunque algunas líneas sean incorrectas.

### Validación de fecha
La fecha se valida con la función auxiliar `isValidDate`.

Se comprueba:
- longitud exacta de 10 caracteres
- guiones en posición 4 y 7
- que el resto sean dígitos
- mes entre 1 y 12
- día dentro del número correcto de días del mes
- febrero adaptado a años bisiestos

Esta validación es importante porque una fecha bien formada visualmente puede seguir siendo inválida, por ejemplo:
- `2024-13-01`
- `2023-02-29`

### Validación del valor
El valor se convierte con `std::strtod`, lo que permite detectar entradas no numéricas.

Después se comprueba:
- que no sea negativo
- que no supere `1000`

Esto sigue exactamente el subject y además protege al programa frente a entradas incorrectas.

### Búsqueda de la fecha aplicable
Una vez validada la línea:
- se usa `_map.lower_bound(date)`

Esto puede dar tres situaciones:
- existe la fecha exacta y se usa directamente
- no existe, pero hay una fecha posterior: entonces se retrocede una posición para usar la fecha inferior más cercana
- la fecha es anterior a toda la base de datos: entonces se considera error

Esta es la decisión más importante del ejercicio, porque es la que justifica de verdad el uso de `std::map`.

### Cálculo final
Cuando ya se tiene la fecha válida y el valor:
- se imprime `date => value = price * value`

El cálculo es sencillo, pero está apoyado sobre una cadena previa de validaciones y búsqueda correcta.

---

## 3. Decisiones de diseño y por qué se han tomado

### Por qué `std::map` y no `std::vector`
`std::vector` habría obligado a:
- guardar todos los pares manualmente
- ordenar después
- implementar la búsqueda por fecha de forma menos natural

`std::map` ya ofrece:
- orden automático
- búsqueda por clave
- `lower_bound`

Por eso es la decisión más natural para este problema.

### Por qué la fecha se guarda como `std::string`
El formato `YYYY-MM-DD` tiene una propiedad muy útil:
- el orden lexicográfico coincide con el orden cronológico

Eso significa que comparar fechas como cadenas funciona correctamente si todas siguen ese formato.

Esta decisión simplifica mucho la implementación, porque no hace falta convertir la fecha a una estructura más compleja solo para ordenarla.

### Por qué se usan funciones auxiliares de validación
Separar lógica como `isLeapYear` e `isValidDate` hace que:
- el código sea más claro
- la validación sea reutilizable
- el método principal no se vuelva demasiado grande

### Por qué el programa sigue tras errores de línea
Porque el subject lo exige y porque es una práctica realista:
- un archivo de entrada puede tener líneas malas mezcladas con buenas
- no conviene perder todo el procesamiento por un solo error local

---

## 4. Qué demuestra este ejercicio

Este ejercicio demuestra que se sabe:
- usar la STL en un caso real
- elegir bien un contenedor según el problema
- parsear archivos de texto
- validar entradas de forma robusta
- trabajar con datos históricos ordenados
- separar interfaz, implementación y flujo principal

---

## 5. Idea de defensa oral

Si me preguntan por el ejercicio, lo explicaría así:

"Este ejercicio consiste en cargar una base de datos histórica de precios de bitcoin y, para cada línea del archivo de entrada, calcular cuánto valdría una cantidad dada en una fecha concreta. Elegí `std::map<std::string, float>` porque necesito datos ordenados por fecha y porque `lower_bound` me permite obtener fácilmente la fecha exacta o la fecha inferior más cercana si la pedida no existe. Primero valido el formato de la fecha y del valor, y solo si la línea es correcta hago el cálculo. Separé la lógica en una clase `BitcoinExchange` para que `main` quede limpio y la responsabilidad del problema esté encapsulada." 
