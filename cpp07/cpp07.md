# CPP07 — C++ Templates

## Indice

1. [Contexto: ¿Qué son los templates y por qué existen?](#1-contexto-qué-son-los-templates-y-por-qué-existen)
2. [Mecanismo interno: cómo el compilador procesa templates](#2-mecanismo-interno-cómo-el-compilador-procesa-templates)
3. [EX00 — Whatever (function templates basicos)](#3-ex00--whatever-function-templates-basicos)
4. [EX01 — Iter (function template con funcion como parametro)](#4-ex01--iter-function-template-con-función-como-parámetro)
5. [EX02 — Array (class template)](#5-ex02--array-class-template)
6. [El archivo .tpp: por qué la implementacion va en el header](#6-el-archivo-tpp-por-qué-la-implementación-va-en-el-header)
7. [Tabla resumen final](#7-tabla-resumen-final)

---

## 1. Contexto: ¿Qué son los templates y por qué existen?

### El problema antes de los templates

Imagina que quieres una funcion `swap` que intercambie dos variables. En C tendrias que escribir una version para cada tipo:

```c
void swap_int(int *a, int *b)   { int tmp = *a; *a = *b; *b = tmp; }
void swap_float(float *a, float *b) { float tmp = *a; *a = *b; *b = tmp; }
void swap_char(char *a, char *b) { char tmp = *a; *a = *b; *b = tmp; }
// ... una version por cada tipo
```

O podrias usar el macro de C:

```c
#define SWAP(T, a, b) do { T tmp = (a); (a) = (b); (b) = tmp; } while(0)
```

Pero los macros son peligrosos: no hay verificacion de tipos, el compilador no puede detectar errores, y se expanden de forma textual causando bugs sutiles.

### La solucion: templates

Un template es una **plantilla** que el compilador usa para generar codigo concreto. Tu describes la logica una sola vez usando un tipo generico (llamado parametro de template), y el compilador genera automaticamente la version para cada tipo que uses:

```cpp
template<typename T>
void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
```

Cuando escribes `swap(myInt, otherInt)`, el compilador genera `swap<int>`. Cuando escribes `swap(myStr, otherStr)`, genera `swap<std::string>`. Todo en compile-time, con verificacion de tipos completa.

Esto se llama **programacion generica**: escribir codigo que funciona con cualquier tipo que cumpla ciertos requisitos (en este caso, que sea copiable).

---

## 2. Mecanismo interno: cómo el compilador procesa templates

### Template instantiation (instanciacion de template)

Cuando el compilador ve esto:

```cpp
template<typename T>
T const& min(T const& a, T const& b)
{
    return (a < b) ? a : b;
}
```

**No genera codigo todavia**. El template es solo una receta. El codigo real se genera cuando lo usas:

```cpp
int x = 3, y = 7;
min(x, y);           // compilador genera min<int>
std::string s1, s2;
min(s1, s2);         // compilador genera min<std::string>
```

Para cada tipo distinto que uses, el compilador genera una funcion separada en el binario. Si usas `min` con 5 tipos distintos, hay 5 funciones en el ejecutable.

### Deduccion de tipo (template argument deduction)

En la mayoria de casos no necesitas especificar el tipo explicitamente:

```cpp
int a = 1, b = 2;
swap(a, b);          // el compilador deduce T = int
swap<int>(a, b);     // equivalente, pero explícito y redundante
```

El compilador mira los argumentos que pasas y deduce `T`. Si los argumentos son de tipos incompatibles, falla:

```cpp
swap(a, myFloat);    // ERROR: T no puede ser int y float al mismo tiempo
```

### typename vs class

En la declaracion del template, `typename` y `class` son intercambiables:

```cpp
template<typename T>  // equivalente a:
template<class T>     // esta forma (historica, de C++98)
```

Ambas significan "T puede ser cualquier tipo". Se prefiere `typename` en codigo moderno porque es mas claro: no implica que T tenga que ser una clase, puede ser `int`, `float`, o cualquier tipo primitivo.

---

## 3. EX00 — Whatever (function templates basicos)

### 3.1 Objetivo

Implementar tres function templates: `swap`, `min` y `max` que funcionen con cualquier tipo que soporte los operadores `<`, `>` y la asignacion.

### 3.2 El codigo completo

```cpp
// Whatever.hpp:4-25
template<typename T> void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename T> T const& min(T const& a, T const& b)
{
    if (a < b)
        return (a);
    else
        return (b);
}

template<typename T> T const& max(T const& a, T const& b)
{
    if (a > b)
        return (a);
    else
        return (b);
}
```

### 3.3 Analisis de `swap`

```cpp
template<typename T> void swap(T &a, T &b)
{
    T tmp = a;   // copia a en tmp (usa el constructor de copia de T)
    a = b;       // asigna b en a  (usa operator= de T)
    b = tmp;     // asigna tmp en b
}
```

Los parametros son **referencias** (`T &a`, `T &b`). Esto es fundamental: si fueran por valor, el swap ocurrirria en copias locales y el cambio no se veria fuera de la funcion.

`T tmp = a` usa el **constructor de copia** de `T`. Por eso el template requiere que `T` sea copiable. Si intentases usar `swap` con un tipo no copiable (ej: un stream de archivo), el compilador fallaria al instanciar el template.

### 3.4 Analisis de `min` y `max`

```cpp
template<typename T> T const& min(T const& a, T const& b)
{
    if (a < b)
        return (a);
    else
        return (b);
}
```

Devuelve `T const&` (referencia constante). Esto tiene dos razones:

1. **Eficiencia**: no copia el objeto al devolverlo. Si `T` es un `std::string` grande, una copia seria costosa.
2. **Const-correctness**: los parametros son `const`, por tanto la referencia que devolvemos tambien debe ser `const` (no podemos devolver una referencia no-const a algo que recibimos como const).

El operador `<` de `T` se usa para comparar. Esto funciona con `int`, `float`, `char`, `std::string` (que compara lexicograficamente) y cualquier tipo que defina `operator<`.

### 3.5 El operador `::` en el main

```cpp
// main.cpp:11-14
::swap(a, b);
::min(a, b);
::max(a, b);
```

El `::` es el **operador de resolucion de ambito global**. Significa "busca esta funcion en el espacio de nombres global, no en otro". Se usa aqui para evitar ambiguedad con `std::swap`, `std::min`, `std::max` que existen en la libreria estandar. Sin `::`, el compilador podria elegir la version de `std` en vez de la tuya.

### 3.6 Lo que ocurre en el main

```cpp
int a = 2;
int b = 3;
::swap(a, b);   // genera swap<int>: a=3, b=2
::min(a, b);    // genera min<int>: devuelve 2 (b)
::max(a, b);    // genera max<int>: devuelve 3 (a)

std::string c = "chaine1";
std::string d = "chaine2";
::swap(c, d);   // genera swap<std::string>: c="chaine2", d="chaine1"
::min(c, d);    // genera min<std::string>: devuelve "chaine1" (menor lexicograficamente)
::max(c, d);    // genera max<std::string>: devuelve "chaine2"
```

El mismo template funciona para `int` y para `std::string` sin ningun cambio. Esta es la potencia de los templates.

---

## 4. EX01 — Iter (function template con función como parámetro)

### 4.1 Objetivo

Implementar una funcion `iter` que recorra un array y aplique una funcion dada a cada elemento. El tipo del array y el tipo de la funcion son ambos parametros de template.

### 4.2 El codigo

```cpp
// Iter.hpp:6-10
template<typename T, typename F> void iter(T *ptr, const size_t len, F f)
{
    for (size_t i = 0; i < len; i++)
        f(ptr[i]);
}
```

### 4.3 Dos parametros de template

Este template tiene **dos parametros**:

- `T`: el tipo de los elementos del array
- `F`: el tipo de la funcion (o callable) que se aplica a cada elemento

```cpp
template<typename T, typename F>
```

Esto permite que `f` sea cualquier cosa "llamable": una funcion normal, un puntero a funcion, un functor (objeto con `operator()`), o una lambda (en C++11).

### 4.4 `size_t` como tipo del indice

`size_t` esta definido en `<cstddef>` y es el tipo que usa C++ para tamaños e indices de arrays. Es un entero sin signo del tamaño de la arquitectura (32 o 64 bits). Se usa para indices porque:

- Un array nunca tiene indices negativos
- `sizeof` y el operador `[]` trabajan internamente con `size_t`
- Comparar un `int` con el resultado de `sizeof` (que es `size_t`) genera warnings

### 4.5 Por qué F es un parametro de template y no un puntero a funcion

Podrias pensar en declarar `iter` asi:

```cpp
template<typename T>
void iter(T *ptr, size_t len, void (*f)(T&));  // puntero a funcion
```

Esto funcionaria para funciones normales pero fallaria con functors y lambdas. Al hacer `F` un parametro de template, el compilador acepta cualquier cosa que pueda llamarse con `(T&)` como argumento:

```cpp
void myFunc(int& x) { x++; }      // funcion normal: F = void(*)(int&)
struct MyFunctor {
    void operator()(int& x) { x++; }
};
MyFunctor obj;
iter(arr, 5, obj);                 // functor: F = MyFunctor
iter(arr, 5, [](int& x){ x++; }); // lambda en C++11: F = closure type
```

### 4.6 Las funciones del main

```cpp
// main.cpp:7-15
template<typename T> void printElement(const T &str)
{
    std::cout << str << std::endl;
}

template<typename T> void modify(T &str)
{
    str++;
}
```

Estas funciones son a su vez templates. Cuando se pasan a `iter`, el compilador necesita saber el tipo concreto para instanciarlas, por eso en el main se pasan con tipo explicito:

```cpp
iter(nums, 10, printElement<int>);   // instancia printElement<int>
iter(nums, 10, modify<int>);          // instancia modify<int>
```

Sin `<int>`, el compilador no podria deducir el tipo porque `printElement` sola es un template (una receta), no una funcion concreta.

### 4.7 Lo que ocurre en el main paso a paso

```cpp
int nums[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// 1. Imprime cada elemento
iter(nums, 10, printElement<int>);
// Genera: iter<int, void(*)(const int&)>
// Para cada i: printElement<int>(nums[i]) → imprime el numero

// 2. Incrementa cada elemento
iter(nums, 10, modify<int>);
// Para cada i: modify<int>(nums[i]) → nums[i]++
// nums ahora es {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}

// 3. Vuelve a imprimir para verificar el cambio
iter(nums, 10, printElement<int>);
```

Lo mismo con el array de chars:

```cpp
char letters[10] = {'a','b','c','d','e','f','g','h','i','j'};
iter(letters, 10, modify<char>);
// Cada char se incrementa: 'a'→'b', 'b'→'c', ..., 'j'→'k'
```

`'a'++` funciona porque `char` en C++ es un tipo numerico: incrementar el caracter da el siguiente en la tabla ASCII.

---

## 5. EX02 — Array (class template)

### 5.1 Objetivo

Implementar una clase `Array<T>` que actue como un array dinamico con las siguientes caracteristicas:
- Construccion con tamaño arbitrario
- Acceso con `operator[]` con comprobacion de rango (lanza excepcion si fuera de rango)
- Copia profunda (deep copy): copiar un `Array` no comparte la memoria interna
- Metodo `size()` que devuelve el numero de elementos

### 5.2 Declaracion del class template

```cpp
// Array.hpp:7-28
template<typename T> class Array
{
    private:
        T *_ptr;       // puntero al array en heap
        size_t _len;   // numero de elementos

    public:
        Array();
        ~Array();
        Array(const Array &);
        Array &operator=(const Array &);

        Array(unsigned int n);

        const T &operator[](const size_t n) const;
        T &operator[](size_t n);

        size_t size() const;
};

#include "Array.tpp"   // incluye la implementacion (se explica en seccion 6)
```

La sintaxis `template<typename T> class Array` declara una clase donde `T` es el tipo de los elementos. Cada vez que uses `Array<int>`, `Array<std::string>`, etc., el compilador genera una clase concreta diferente.

### 5.3 Constructor por defecto

```cpp
// Array.tpp:3-7
template<typename T>
Array<T>::Array() : _ptr(NULL), _len(0)
{

}
```

`:_ptr(NULL), _len(0)` es la **lista de inicializacion**. Es la forma correcta de inicializar miembros en el constructor: mas eficiente que asignar en el cuerpo porque los miembros se inicializan directamente en vez de primero default-construirse y luego asignarse.

Un array vacio tiene puntero nulo y longitud cero. Esto es importante: `delete[] NULL` es una operacion valida en C++ (no hace nada), lo que hace que el destructor sea seguro aunque nunca se haya asignado memoria.

### 5.4 Constructor con tamaño

```cpp
// Array.tpp:9-14
template<typename T>
Array<T>::Array(unsigned int n)
{
    this->_ptr = new T[n]();
    this->_len = n;
}
```

`new T[n]()` hace dos cosas:
1. Aloca memoria para `n` objetos de tipo `T` en el heap
2. El `()` al final **inicializa por valor** (value-initializes) cada elemento:
   - Para tipos primitivos (`int`, `float`, etc.): los pone a cero
   - Para objetos: llama a su constructor por defecto

Sin el `()`, los enteros tendrian basura de memoria. Con el, `Array<int>(5)` crea un array de 5 ceros. Esto se ve en el main cuando imprime el array recien creado: todos los valores son 0.

### 5.5 Constructor de copia (deep copy)

```cpp
// Array.tpp:16-23
template<typename T>
Array<T>::Array(const Array &src)
{
    this->_len = src.size();
    this->_ptr = new T[this->_len];    // nueva alocation de memoria
    for (size_t i = 0; i < this->_len; i++)
        this->_ptr[i] = src[i];        // copia elemento a elemento
}
```

La copia profunda (deep copy) es critica aqui. Si en vez de esto hicieras:

```cpp
// MAL: shallow copy
this->_ptr = src._ptr;  // ambos objetos apuntan al mismo array
```

Entonces `Array<int> copy(nums)` haria que `copy._ptr` y `nums._ptr` apuntasen al mismo array en memoria. Al modificar `copy[0]`, tambien modificarias `nums[0]`. Y al destruir uno de los dos, el destructor del otro intentaria liberar memoria ya liberada (double free → crash).

Con deep copy, `copy` tiene su propia memoria. El main lo verifica:

```cpp
Array<int> copy(nums);
copy[0] = 999;
std::cout << "copy[0] = " << copy[0] << std::endl;  // 999
std::cout << "nums[0] = " << nums[0] << std::endl;  // 0 (sin cambios)
```

### 5.6 Operador de asignacion (tambien deep copy)

```cpp
// Array.tpp:25-37
template<typename T>
Array<T> &Array<T>::operator=(const Array &src)
{
    if (this == &src)          // auto-asignacion: arr = arr
        return (*this);

    delete [] this->_ptr;      // libera memoria anterior
    this->_ptr = NULL;         // NULL antes del new, por seguridad

    this->_len = src.size();
    this->_ptr = new T[this->_len];
    for (size_t i = 0; i < this->_len; i++)
        this->_ptr[i] = src[i];
    return (*this);
}
```

Tres puntos importantes:

**Guarda de auto-asignacion** (`if (this == &src)`): si alguien escribe `arr = arr`, sin esta guarda harias `delete[] arr._ptr` y luego intentarias leer de esa memoria ya liberada. El resultado seria corrupcion de memoria.

**`delete[] this->_ptr` antes del `new`**: el objeto ya existia y tenia su propia memoria. Hay que liberarla antes de alocar la nueva, o tendriamos un memory leak.

**`this->_ptr = NULL` entre delete y new**: si el `new` lanzase una excepcion (por falta de memoria), el destructor se llamaria con `_ptr` apuntando a memoria ya liberada. Ponerlo a `NULL` primero hace que el destructor haga `delete[] NULL` (seguro) en vez de double free.

### 5.7 Destructor

```cpp
// Array.tpp:39-43
template<typename T>
Array<T>::~Array()
{
    delete [] this->_ptr;
}
```

`delete[]` (con corchetes) es obligatorio cuando la memoria fue alojada con `new T[n]`. Sin los corchetes, `delete this->_ptr` solo llamaria al destructor del primer elemento y liberaria la memoria del bloque entero, pero sin llamar a los destructores de los demas elementos. Para tipos con destructores no triviales (como `std::string`), esto causaria fugas de recursos.

### 5.8 `operator[]` — dos versiones

```cpp
// Array.tpp:45-61
template<typename T>
const T &Array<T>::operator[](const size_t n) const  // version const
{
    if (n >= this->_len)
        throw std::exception();
    return (this->_ptr[n]);
}

template<typename T>
T &Array<T>::operator[](size_t n)                     // version no-const
{
    if (n >= this->_len)
        throw std::exception();
    return (this->_ptr[n]);
}
```

Hay dos versiones del mismo operador:

- **Version const** (`const T& ... const`): se llama cuando el array es un objeto const. Devuelve referencia const para que no puedas modificar el elemento.
- **Version no-const** (`T& ...`): se llama en arrays normales. Devuelve referencia no-const, permitiendo modificar: `arr[2] = 42`.

El compilador elige automaticamente cual llamar segun si el objeto es const o no.

La comprobacion `n >= this->_len` usa `>=` y no `>` porque los indices validos van de `0` a `_len - 1`. Si `_len` es 5, el ultimo indice valido es 4. Con `>` en vez de `>=`, el indice 5 no lanzaria excepcion y accederia a memoria fuera del array.

Se usa `size_t` para el indice (sin signo), por lo que no es necesario comprobar que sea `>= 0`: un `size_t` nunca puede ser negativo.

### 5.9 `size()`

```cpp
// Array.tpp:63-66
template<typename T>
size_t Array<T>::size() const
{
    return (this->_len);
}
```

El `const` al final del metodo significa que este metodo no modifica el estado del objeto. Esto permite llamarlo sobre arrays constantes. Sin ese `const`, no podrias hacer:

```cpp
const Array<int> arr(5);
arr.size();   // ERROR si size() no es const
```

### 5.10 Lo que demuestra el main paso a paso

```cpp
// Array vacio
Array<int> empty;
empty.size();   // → 0

// Array de 5 ints, inicializados a 0
Array<int> nums(5);
// nums[0]=0, nums[1]=0, nums[2]=0, nums[3]=0, nums[4]=0

// Modificar elementos
for (size_t i = 0; i < nums.size(); i++)
    nums[i] = i * 10;
// nums[0]=0, nums[1]=10, nums[2]=20, nums[3]=30, nums[4]=40

// Deep copy con constructor de copia
Array<int> copy(nums);
copy[0] = 999;
// copy[0]=999, nums[0]=0 (independientes en memoria)

// Deep copy con operador =
Array<int> assigned;
assigned = nums;
assigned[1] = 777;
// assigned[1]=777, nums[1]=10 (independientes)

// Excepcion en acceso fuera de rango
try {
    nums[100];   // _len=5, 100>=5 → throw std::exception()
}
catch (std::exception &e) {
    // capturado aqui
}

// Template funciona con cualquier tipo
Array<std::string> strs(3);
strs[0] = "hola";
strs[1] = "mundo";
strs[2] = "!";
```

---

## 6. El archivo .tpp: por qué la implementación va en el header

### El problema

Cuando el compilador procesa un `.cpp`, solo ve ese archivo (y sus includes). Para instanciar un template, necesita ver la definicion completa de la funcion en el momento en que la usa.

Si pusieras la implementacion de `Array<T>` en `Array.cpp`:

```
Array.hpp   → solo la declaracion: template<typename T> class Array { ... };
Array.cpp   → la implementacion: template<typename T> Array<T>::Array() { ... }
main.cpp    → usa Array<int>
```

Al compilar `main.cpp`, el compilador intenta generar `Array<int>` pero solo ve la declaracion en `Array.hpp`. No tiene la implementacion (esta en `Array.cpp`). Resultado: error de linker "undefined reference to Array<int>::Array()".

### La solucion: incluir la implementacion en el header

```cpp
// Array.hpp (final)
#ifndef ARRAY_HPP
# define ARRAY_HPP
// ...declaracion de la clase...

#include "Array.tpp"   // ← incluye la implementacion directamente
#endif
```

Cuando `main.cpp` incluye `Array.hpp`, por transitividad tambien incluye `Array.tpp`. El compilador tiene toda la informacion necesaria para instanciar `Array<int>`, `Array<std::string>`, etc.

### ¿Por qué .tpp y no meter todo en .hpp?

Podrias poner la implementacion directamente en el `.hpp`, y funcionaria igual. El `.tpp` es una convencion de estilo que separa la declaracion de la interfaz (`.hpp`) de la implementacion (`.tpp`), manteniendo el codigo mas legible. No hay diferencia tecnica para el compilador: ambos archivos acaban siendo texto incluido en el mismo punto.

La extension `.tpp` (o a veces `.ipp`, `.inl`) es convencion del proyecto/equipo. El compilador no le da ningun significado especial.

---

## 7. Tabla resumen final

### Los conceptos nuevos de cpp07

| Concepto | ¿Donde aparece? | Descripcion |
|----------|----------------|-------------|
| Function template | ex00, ex01 | Funcion generica parametrizada por tipo |
| Class template | ex02 | Clase generica parametrizada por tipo |
| Template instantiation | todo | El compilador genera codigo concreto por cada tipo usado |
| Template argument deduction | ex00 | El compilador infiere T de los argumentos |
| Typename vs class | todo | Intercambiables en parametros de template |
| Dos parametros de template | ex01 | `template<typename T, typename F>` |
| Deep copy | ex02 | Constructor de copia y operator= que alocan memoria propia |
| `new T[n]()` | ex02 | Alocacion con value-initialization (pone a cero tipos primitivos) |
| `delete[]` | ex02 | Liberacion de arrays (distinto de `delete` simple) |
| `operator[]` const y no-const | ex02 | Dos versiones segun si el objeto es const |
| `.tpp` file | ex02 | Convencion para separar la implementacion del class template |

### Comparacion: templates vs alternativas

| | Templates | Macros (#define) | void* (C) |
|--|-----------|-----------------|-----------|
| Verificacion de tipos | Si (compile-time) | No | No |
| Legibilidad | Alta | Baja | Media |
| Debugging | Facil | Muy dificil | Dificil |
| Performance | Igual (inline) | Igual | Peor (puede necesitar cast) |
| Funcionan con clases | Si | Parcialmente | No |

### El requisito implicito de los templates

Los templates imponen requisitos sobre el tipo `T` sin declararlos explicitamente. Por ejemplo, `swap` requiere que `T` sea:
- **Copiable**: `T tmp = a` (usa constructor de copia)
- **Asignable**: `a = b` (usa operator=)

`min` y `max` requieren ademas:
- **Comparable**: `a < b` / `a > b` (usa operator< y operator>)

Si intentas usar un template con un tipo que no cumple estos requisitos, obtienes un **error de compilacion** (a veces largo y confuso) en el momento de la instanciacion. En C++20 esto se mejoro con `concepts`, que permiten declarar explicitamente los requisitos.
