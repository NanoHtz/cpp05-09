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

La secuencia se define como:
- J(0) = 0
- J(1) = 1
- J(n) = J(n-1) + 2 * J(n-2)

Valores: **0, 1, 1, 3, 5, 11, 21, 43, 85, 171, ...**

El algoritmo genera un orden de inserción para los pendientes en grupos basados en esta secuencia. Por ejemplo, para 5 elementos pendientes, el orden de inserción es:
`[0, 2, 1, 4, 3]`

Esto significa: inserta primero el pendiente[0], luego pendiente[2], luego pendiente[1], luego pendiente[4], luego pendiente[3].

Insertando en este orden, cada nuevo elemento tiene un rango de búsqueda acotado que minimiza comparaciones. Es la razón matemática por la que Ford-Johnson es eficiente.

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

### Estructura de nodo y group IDs (detalle de implementación)

Cada elemento de la cadena se representa como:
```
pair<int, pair<size_t, bool>>
     ↑         ↑        ↑
   valor    group_id   es_principal
```

`group_id` indica a qué pareja original pertenece el elemento.
`es_principal` indica si es el elemento mayor de la pareja (true) o el menor (false).

Esto es necesario porque al insertar un pendiente `b_i` (el menor de la pareja i), hay que buscarlo solo en el rango `[inicio, posición del mayor de la pareja i]`. No tiene sentido buscarlo después de su propio par.

`findUpperBound` recorre la cadena y localiza el índice del mayor de la pareja `i`, que actúa como límite superior de la búsqueda binaria.

### Fase 5: inserción binaria
Cada menor pendiente `b_i` se inserta respetando su límite:

```
1. upper = findUpperBound(chain, group_id_de_b_i)
   → localiza la posición del mayor de su misma pareja en la cadena

2. insertAt = lowerBound(chain, valor_de_b_i, upper)
   → búsqueda binaria dentro de [0, upper) para encontrar dónde insertar

3. chain.insert(chain.begin() + insertAt, b_i)
```

Esto garantiza que `b_i` nunca se compara con elementos más a la derecha que su propio par, reduciendo comparaciones.

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

## 2b. Ejemplo completo trazado paso a paso

Entrada: `3 5 9 7 4`

### Fase 1: formar parejas
```
Posición:  [0]  [1]  [2]  [3]  [4]
Valores:    3    5    9    7    4

Par 0: (3, 5) → small=3, large=5
Par 1: (7, 9) → small=7, large=9   (se ordena: 7 < 9)
Straggler: 4  (elemento suelto por cantidad impar)
```

### Fase 2: ordenar los mayores recursivamente
```
larges = [5, 9]
sortVector([5, 9]):
  Par: (5, 9) → small=5, large=9
  Llama a sortVector([9]) → devuelve [9]
  sortedPairs = [(5, 9)]
  chain = [5, 9] (resultado)
→ sortedLargeValues = [5, 9]
```

### Fase 3: reconstruir parejas en orden e inicializar la cadena
```
sortedLargeValues[0]=5 → viene del par (3,5) → sortedPairs[0]=(3,5)
sortedLargeValues[1]=9 → viene del par (7,9) → sortedPairs[1]=(7,9)

Iniciar chain con el menor de la primera pareja:
chain = [(3, group=0, main=false)]

Añadir los mayores:
chain = [(3, g=0, main=false), (5, g=0, main=true), (9, g=1, main=true)]

Añadir menores restantes a pending (desde el par 1 en adelante):
pending = [(7, g=1, main=false)]
```

### Fase 4: calcular orden Jacobsthal para 1 pendiente
```
buildJacobsthalOrder(1) → [0]
Se insertará: pending[0] = 7
```

### Fase 5: insertar pendientes con búsqueda binaria
```
Insertar pending[0] = 7, group=1:

  upper = findUpperBound(chain, group=1)
        = posición del nodo con main=true y group=1
        = índice 2  (el valor 9)

  insertAt = lowerBound(chain, valor=7, end=2)
           = búsqueda en chain[0..1]: [(3,g0,false), (5,g0,true)]
           = primera posición con valor >= 7
           = índice 2 (9 > 7, pero estamos buscando en [0,2))
           → el 7 va antes del 9, en índice 2

chain = [(3,g0,false), (5,g0,true), (7,g1,false), (9,g1,true)]
```

### Fase 6: insertar el straggler (4)
```
insertAt = lowerBound(chain, valor=4, end=4)
         = primera posición con valor >= 4
         = índice 1 (5 >= 4)

chain = [(3,g0,false), (4,_,false), (5,g0,true), (7,g1,false), (9,g1,true)]
```

### Resultado final
```
Extraer los valores: [3, 4, 5, 7, 9] ✓
```

La secuencia está ordenada y se ha llegado a ella siguiendo exactamente el modelo Ford-Johnson.

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
