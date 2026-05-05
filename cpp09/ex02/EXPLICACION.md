# ex02 - PmergeMe

## 1. Conceptos clave del ejercicio

### Algoritmos de ordenación
Ordenar datos es uno de los problemas clásicos de la programación.

Es importante porque:
- aparece constantemente en software real
- afecta directamente al rendimiento
- obliga a pensar en estructuras de datos, complejidad y coste de operaciones

En este ejercicio no basta con ordenar: hay que usar un algoritmo concreto.

### Ford-Johnson / Merge-Insertion Sort
El subject pide usar el algoritmo Ford-Johnson, también llamado merge-insert sort.

La idea general del algoritmo es:
- formar parejas
- ordenar internamente cada pareja
- quedarse con los mayores de cada pareja
- ordenar recursivamente esos mayores
- insertar los menores en un orden concreto para minimizar comparaciones

Ese orden concreto está relacionado con la secuencia de Jacobsthal.

Este algoritmo es importante en programación porque enseña algo más profundo que "ordenar":
- cómo reducir comparaciones
- cómo aprovechar estructura parcial ya ordenada
- cómo diseñar algoritmos no triviales sobre una base recursiva

### Contenedores STL: `std::vector` y `std::deque`
El subject exige usar al menos dos contenedores distintos.

En este proyecto se usan:
- `std::vector`
- `std::deque`

Esto es importante porque demuestra que el mismo problema puede tener costes distintos según la estructura usada.

#### `std::vector`
Ventajas:
- memoria contigua
- muy buena localidad de caché
- acceso aleatorio muy eficiente

Importancia:
- es uno de los contenedores más usados en programación de alto rendimiento

#### `std::deque`
Ventajas:
- acceso aleatorio también constante
- inserciones y crecimiento más flexibles que `vector` en ciertos casos

Importancia:
- permite comparar comportamiento con `vector`
- enseña que no todos los contenedores se comportan igual aunque la interfaz se parezca

### Inserción binaria
Cuando el algoritmo inserta elementos en una cadena parcialmente ordenada, usa búsqueda binaria para encontrar la posición.

Esto es importante porque:
- reduce el número de comparaciones
- aprovecha que parte de la secuencia ya está ordenada

### Secuencia de Jacobsthal
La secuencia de Jacobsthal se usa para decidir el orden en el que se insertan los elementos pendientes.

No es un detalle decorativo: forma parte de la idea del Ford-Johnson para reducir comparaciones al ordenar.

Este concepto es importante porque demuestra que un algoritmo puede necesitar una estrategia matemática concreta, no solo una estructura de control básica.

### Medición de tiempo
El exercise no solo pide ordenar, sino también mostrar el tiempo usado por cada contenedor.

Esto es importante en programación porque:
- no basta con que algo funcione
- también interesa cuánto cuesta
- obliga a pensar en rendimiento y no solo en corrección

---

## 2. Explicación del ejercicio

### Qué pide el ejercicio
El programa `PmergeMe` debe:
- recibir una secuencia de enteros positivos por argumentos
- ordenarla usando Ford-Johnson
- hacerlo sobre al menos dos contenedores distintos
- mostrar la secuencia antes y después
- mostrar el tiempo usado por cada contenedor

### Cómo está resuelto en este proyecto

#### `main.cpp`
El `main`:
- crea un objeto `PmergeMe`
- llama a `sort`
- imprime `Before`
- imprime `After`
- imprime el tiempo para `std::vector`
- imprime el tiempo para `std::deque`

También captura excepciones y muestra `Error` si la entrada no es válida.

La idea es que `main` coordine, pero no contenga la lógica del algoritmo.

#### Clase `PmergeMe`
La clase guarda:
- `_vectorInput`
- `_dequeInput`
- `_vectorSorted`
- `_dequeSorted`
- `_vectorTime`
- `_dequeTime`

Esto permite:
- mantener la entrada original
- almacenar el resultado de cada contenedor
- comparar tiempos claramente

### Validación de entrada
La validación se hace en `parsePositiveInt`.

Se comprueba:
- que el token no esté vacío
- que todos sus caracteres sean dígitos
- que no haya overflow sobre `int`
- que el valor sea estrictamente positivo

Esto es importante porque el subject exige enteros positivos y error por `stderr` cuando algo no es válido.

### Implementación separada por contenedor
Para seguir el `strongly advised` del subject, el algoritmo se implementa por separado en:
- `sortVector`
- `sortDeque`

Eso permite defender claramente que:
- el Ford-Johnson está presente para cada contenedor
- no se trata solo de una función genérica aplicada a dos tipos
- se ha pensado el ejercicio en términos de los contenedores concretos requeridos

### Fase 1: formar parejas
La secuencia se recorre de dos en dos.

Para cada pareja:
- se coloca primero el menor
- después el mayor

Así se construyen pares del tipo:
- `(small, large)`

Si el número de elementos es impar:
- el último se guarda como `straggler`

Esta fase es importante porque crea la estructura base del Ford-Johnson.

### Fase 2: ordenar los mayores
Se extraen los elementos mayores de cada pareja y se ordenan recursivamente con el mismo algoritmo.

Esto crea una cadena principal ordenada a partir de la cual luego se van insertando los menores.

Aquí está el corazón recursivo del algoritmo.

### Fase 3: construir la cadena principal
Una vez ordenados los mayores:
- se inicia la cadena con el menor de la primera pareja
- se añaden los mayores ya ordenados
- los menores restantes pasan a una lista de pendientes

Esto es clave porque ya no se parte de cero: se parte de una estructura parcialmente ordenada.

### Fase 4: orden Jacobsthal
Los elementos pendientes no se insertan en cualquier orden.

Se calcula un orden basado en Jacobsthal para decidir la secuencia de inserción.

La razón es reducir comparaciones, que es precisamente la idea del Ford-Johnson.

### Fase 5: inserción binaria
Cada menor pendiente se inserta antes del mayor de su misma pareja.

Para localizar esa posición:
- se encuentra el límite superior válido
- se hace una búsqueda binaria dentro de ese rango

Esto respeta la lógica del algoritmo y evita comparaciones innecesarias.

### Fase 6: straggler
Si había un elemento suelto por cantidad impar:
- se inserta al final usando también búsqueda binaria

Así se garantiza que ningún elemento quede fuera del orden final.

### Medición de tiempos
La función `sort` mide por separado:
- el tiempo de `sortVector`
- el tiempo de `sortDeque`

Se usa `std::clock()` y se convierte a microsegundos.

Esto permite mostrar una comparación clara entre ambos contenedores.

---

## 3. Decisiones de diseño y por qué se han tomado

### Por qué `vector` y `deque`
Se eligieron porque el subject pide dos contenedores y porque son suficientemente parecidos para comparar el impacto del contenedor, pero suficientemente distintos para mostrar diferencias reales de rendimiento.

`vector` suele ser más rápido por:
- memoria contigua
- mejor caché
- menos coste de indirección

`deque` suele ser más lento en este ejercicio por:
- almacenamiento segmentado
- peor localidad de caché
- más coste estructural al recorrer e insertar

### Por qué implementar el algoritmo por separado
Aunque una plantilla genérica era posible, el subject recomienda implementar el algoritmo para cada contenedor.

Se tomó esa decisión porque:
- facilita la defensa en evaluación
- demuestra de forma explícita el uso del algoritmo en cada contenedor
- evita la crítica de "solo has escrito una función genérica"

### Por qué usar búsqueda binaria
Porque la cadena ya está parcialmente ordenada.

En ese contexto, buscar linealmente sería menos elegante y aumentaría el número de comparaciones.

### Por qué conservar la entrada original
Porque el programa debe mostrar claramente:
- `Before`
- `After`

Si se ordenara directamente la entrada sin conservarla, se perdería parte de la salida requerida.

### Por qué usar excepciones
Porque permiten abortar la construcción del objeto o el procesamiento cuando la entrada es inválida, y luego `main` se encarga de mostrar `Error`.

Eso deja el flujo del programa limpio y fácil de seguir.

---

## 4. Qué demuestra este ejercicio

Este ejercicio demuestra que se sabe:
- aplicar un algoritmo de ordenación no trivial
- trabajar recursivamente
- comparar dos contenedores STL distintos
- medir rendimiento
- justificar decisiones de estructura de datos
- escribir una solución defendible tanto funcional como conceptualmente

---

## 5. Qué responder si preguntan por la diferencia de tiempo

Una explicación razonable es esta:

"En mi implementación, `std::vector` suele ser más rápido que `std::deque` porque guarda los elementos en memoria contigua, lo que mejora la localidad de caché y hace más eficientes los recorridos y accesos. `std::deque` también ofrece acceso aleatorio, pero su almacenamiento está segmentado, así que en un algoritmo con muchas lecturas y comparaciones suele rendir peor. Por eso, aunque ambos implementan el mismo Ford-Johnson, el coste práctico del contenedor no es el mismo."

---

## 6. Idea de defensa oral

Si me preguntan por el ejercicio, lo explicaría así:

"Este ejercicio consiste en ordenar una secuencia de enteros positivos usando Ford-Johnson y comparar el comportamiento entre `std::vector` y `std::deque`. Primero valido todos los argumentos para asegurar que son enteros positivos y que no hay overflow. Después, para cada contenedor, formo parejas, ordeno cada pareja internamente, tomo los mayores y los ordeno recursivamente. A partir de esa cadena principal, inserto los menores en el orden marcado por Jacobsthal usando búsqueda binaria, y si sobra un elemento impar lo inserto al final también con búsqueda binaria. Conservo la secuencia original para mostrar `Before`, la secuencia ordenada para `After`, y mido el tiempo de cada contenedor por separado para poder comparar rendimiento." 
