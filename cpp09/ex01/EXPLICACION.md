# ex01 - RPN

## 1. Conceptos clave del ejercicio

### Notación polaca inversa
RPN significa Reverse Polish Notation, o notación polaca inversa.

En esta notación:
- primero aparecen los operandos
- después aparece el operador

Ejemplo:
- expresión infija: `3 + 4`
- expresión RPN: `3 4 +`

Es importante en programación porque:
- elimina la necesidad de paréntesis
- elimina ambigüedad de precedencia
- se evalúa de forma muy natural con una pila

Además, esta notación aparece en compiladores, intérpretes y calculadoras.

### Estructura LIFO y `std::stack`
El contenedor central aquí es `std::stack<int>`.

Una pila sigue la regla LIFO: Last In, First Out.
- el último valor que entra es el primero que sale

Eso encaja perfectamente con RPN, porque cuando aparece un operador:
- hay que usar los dos operandos más recientes

Es importante en programación porque las pilas aparecen en:
- evaluación de expresiones
- llamadas a funciones
- backtracking
- parsers
- gestión de contexto temporal

### Tokenización
El programa recibe una expresión como una cadena y debe dividirla en tokens.

Un token puede ser:
- un número
- un operador

Aquí se usa `std::istringstream` para leer los tokens separados por espacios.

Este concepto es importante porque muchos programas trabajan interpretando comandos o mini-lenguajes, y la tokenización es el primer paso para entender la entrada.

### Validación de expresiones
No basta con operar: hay que detectar errores.

Aquí se valida:
- número de argumentos
- tokens inválidos
- operandos insuficientes
- división entre cero
- pila final con tamaño incorrecto

Esto es importante porque un evaluador no solo mira que el caso bueno funcione, sino que el programa sea robusto frente a entradas incorrectas.

### Excepciones
En este ejercicio la clase `RPN` lanza `std::runtime_error("Error")` cuando detecta un problema, y `main` captura esa excepción.

Esto es importante porque:
- separa la lógica del error del flujo principal
- evita llenar el código de comprobaciones repetidas
- permite centralizar el mensaje de error en `main`

---

## 2. Explicación del ejercicio

### Qué pide el ejercicio
El programa `RPN` debe:
- recibir una expresión matemática en notación polaca inversa
- evaluarla correctamente
- mostrar el resultado por salida estándar
- mostrar `Error` por error estándar si la entrada es inválida

Los operandos usados en la expresión son menores que `10`, así que en esta solución se aceptan operandos de un solo dígito.

### Cómo está resuelto en este proyecto

#### `main.cpp`
El `main`:
- comprueba que haya exactamente un argumento
- crea un objeto `RPN`
- llama a `evaluate`
- imprime el resultado si todo va bien
- captura excepciones si algo falla

La idea es dejar `main` muy limpio y que toda la lógica del ejercicio viva dentro de la clase.

#### Clase `RPN`
La clase contiene:
- un atributo privado `_stack` de tipo `std::stack<int>`
- el método `evaluate`, que procesa la expresión

Esta decisión es coherente con 42 porque:
- la clase agrupa el estado y la lógica
- respeta la estructura esperada del subject
- hace el código más defendible

### Tokenización de la expresión
La expresión se procesa usando `std::istringstream`.

Eso permite:
- leer token por token
- exigir que cada token sea una unidad separada por espacios
- rechazar casos mal formados como `89*` o `1 2+`

Esto es importante porque el subject espera una expresión correctamente separada en tokens.

### Traza completa de una evaluación

Expresión: `"3 4 + 2 *"`

| Token | Acción | Pila tras la acción |
|-------|--------|---------------------|
| `3`   | es dígito → push(3) | [3] |
| `4`   | es dígito → push(4) | [3, 4] |
| `+`   | operador: right=4, left=3, push(3+4=7) | [7] |
| `2`   | es dígito → push(2) | [7, 2] |
| `*`   | operador: right=2, left=7, push(7*2=14) | [14] |

Final: la pila tiene exactamente 1 elemento → resultado = **14**

Ejemplo del PDF: `"8 9 * 9 - 9 - 9 - 4 - 1 +"`

| Token | right | left | Operación | Pila |
|-------|-------|------|-----------|------|
| `8`   | –     | –    | push 8    | [8] |
| `9`   | –     | –    | push 9    | [8, 9] |
| `*`   | 9     | 8    | 8*9=72    | [72] |
| `9`   | –     | –    | push 9    | [72, 9] |
| `-`   | 9     | 72   | 72-9=63   | [63] |
| `9`   | –     | –    | push 9    | [63, 9] |
| `-`   | 9     | 63   | 63-9=54   | [54] |
| `9`   | –     | –    | push 9    | [54, 9] |
| `-`   | 9     | 54   | 54-9=45   | [45] |
| `4`   | –     | –    | push 4    | [45, 4] |
| `-`   | 4     | 45   | 45-4=41   | [41] |
| `1`   | –     | –    | push 1    | [41, 1] |
| `+`   | 1     | 41   | 41+1=42   | [42] |

Final: **42**

Nota importante sobre el orden al sacar operandos:
```cpp
int right = _stack.top(); _stack.pop();
int left  = _stack.top(); _stack.pop();
// Se opera: left OP right
```
El primero que se saca es `right` porque es el último que entró.
Para `+` y `*` el orden da igual, pero para `-` y `/` es esencial:
- `"5 3 -"` → right=3, left=5 → 5-3=2 ✓ (no 3-5=-2)

### Tratamiento de los números
Si el token tiene longitud `1` y es un dígito:
- se convierte a entero con `c - '0'`
- se mete en la pila

La pila va acumulando operandos hasta que aparece un operador.

### Tratamiento de los operadores
Si el token es uno de `+`, `-`, `*`, `/`:
- primero se comprueba que haya al menos dos números en la pila
- se sacan dos valores
- se aplica la operación en el orden correcto

El orden importa mucho en:
- resta
- división

Si saco primero `right` y luego `left`, la operación correcta es:
- `left - right`
- `left / right`

Ese detalle es fundamental para no invertir el resultado.

### División entre cero
Antes de dividir se comprueba si el divisor es `0`.

Si lo es:
- se lanza una excepción

Este control es importante porque evita comportamiento indefinido o errores de ejecución.

### Comprobación final
Al terminar de procesar todos los tokens:
- la pila debe contener exactamente un elemento

Si no es así, la expresión era inválida.

Esto detecta casos como:
- operandos sobrantes
- operadores insuficientes

---

## 3. Decisiones de diseño y por qué se han tomado

### Por qué `std::stack`
Porque RPN se evalúa de forma natural con una pila:
- los números se apilan
- los operadores consumen los dos últimos operandos

Usar otro contenedor como `vector` o `list` sería posible, pero no expresaría tan bien la lógica del problema.

### Por qué usar `std::istringstream`
Porque permite leer tokens correctamente separados por espacios y endurecer el parseo.

Leer carácter a carácter parecía más simple al principio, pero tenía un problema:
- aceptaba entradas mal formadas que no deberían ser válidas

Por eso la decisión final fue trabajar por tokens y no por caracteres.

### Por qué usar excepciones
Porque permiten:
- interrumpir la evaluación al detectar un error
- evitar múltiples retornos intermedios complicados
- centralizar el mensaje `Error` en `main`

### Por qué se limpia la pila al inicio de `evaluate`
El método vacía `_stack` antes de empezar a procesar una nueva expresión.

Eso es importante porque una misma instancia de `RPN` no debe conservar restos de una evaluación anterior.

---

## 4. Qué demuestra este ejercicio

Este ejercicio demuestra que se sabe:
- elegir el contenedor correcto para un problema concreto
- parsear una entrada textual
- evaluar expresiones con una estructura LIFO
- manejar errores de ejecución de forma limpia
- encapsular lógica en una clase simple y bien definida

---

## 5. Idea de defensa oral

Si me preguntan por el ejercicio, lo explicaría así:

"Este ejercicio evalúa expresiones en notación polaca inversa. Elegí `std::stack<int>` porque el modelo natural del problema es una pila: cada número se apila y cada operador consume los dos últimos operandos. En lugar de leer la expresión carácter a carácter, la parseo por tokens con `std::istringstream`, porque así solo acepto entradas bien formadas separadas por espacios. Cuando encuentro un operador, saco dos elementos, aplico la operación en el orden correcto y vuelvo a apilar el resultado. Si falta un operando, aparece un token inválido, hay división por cero o al final la pila no tiene exactamente un elemento, lanzo un error." 
