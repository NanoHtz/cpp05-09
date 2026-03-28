# CPP06 — C++ Type Casting

## Indice

1. [Contexto: ¿Por qué C++ necesita sus propios casts?](#1-contexto-por-qué-c-necesita-sus-propios-casts)
2. [Los 4 casts de C++](#2-los-4-casts-de-c)
3. [EX00 — ScalarConverter (static_cast)](#3-ex00--scalarconverter-static_cast)
4. [EX01 — Serializer (reinterpret_cast)](#4-ex01--serializer-reinterpret_cast)
5. [EX02 — Type Identification (dynamic_cast)](#5-ex02--type-identification-dynamic_cast)
6. [Tabla resumen final](#6-tabla-resumen-final)

---

## 1. Contexto: ¿Por qué C++ necesita sus propios casts?

En C, para convertir un tipo a otro escribías:

```c
float f = 3.14;
int i = (int)f;        // cast estilo C
int j = *(int*)&f;     // también cast estilo C, pero reinterpreta los bits
```

El problema es que el cast de C es un **comodin**: hace lo que sea necesario para forzar la conversion, sin distinguir si es una conversion numerica segura, una reinterpretacion de bits peligrosa, o un descarte de `const`. El compilador no puede decirte si lo que hiciste tiene sentido.

C++ introduce 4 operadores de cast distintos, cada uno con una responsabilidad clara:

- `static_cast`      → conversiones entre tipos compatibles conocidas en compile-time
- `reinterpret_cast` → reinterpretacion de bits en memoria (conversiones de bajo nivel)
- `dynamic_cast`     → conversiones seguras en jerarquias de herencia en runtime
- `const_cast`       → anadir o quitar `const`/`volatile`

Cada uno comunica **exactamente** tu intencion. Si usas el incorrecto, el compilador falla. Si usas el correcto pero en un contexto equivocado, el error es mucho mas facil de encontrar.

---

## 2. Los 4 casts de C++

### 2.1 `static_cast`

```cpp
static_cast<tipo_destino>(expresion)
```

Es el cast mas comun. Realiza conversiones que el compilador puede verificar en **tiempo de compilacion**. Ejemplos validos:

```cpp
float f = 3.14f;
int i = static_cast<int>(f);          // float → int (trunca decimales)

char c = 'A';
int ascii = static_cast<int>(c);       // char → int (valor ASCII: 65)

int n = 65;
char letra = static_cast<char>(n);     // int → char (si esta en rango)
```

No permite conversiones que no tienen sentido:

```cpp
int* p = static_cast<int*>(0xDEAD);   // ERROR de compilacion
```

### 2.2 `reinterpret_cast`

```cpp
reinterpret_cast<tipo_destino>(expresion)
```

Reinterpreta los bytes en memoria como si fueran de otro tipo. No hay conversion de datos: los bits son exactamente los mismos, solo cambia como el compilador los interpreta. Es el unico cast que puede convertir un puntero a un entero y viceversa.

```cpp
int x = 65;
int* ptr = &x;
uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);  // puntero → entero
int* ptr2 = reinterpret_cast<int*>(addr);            // entero → puntero
// ptr2 == ptr (misma direccion)
```

Es potencialmente peligroso: el compilador no verifica nada. Depende completamente del programador saber que lo que esta haciendo es seguro.

### 2.3 `dynamic_cast`

```cpp
dynamic_cast<tipo_destino>(expresion)
```

Solo funciona con punteros o referencias a clases que tienen al menos una funcion `virtual` (clases polimorficas). Verifica en **tiempo de ejecucion** si la conversion es valida:

- Con punteros: devuelve `nullptr` si falla
- Con referencias: lanza `std::bad_cast` si falla

```cpp
Base* b = new A;               // upcast implicito (siempre seguro)
A* a = dynamic_cast<A*>(b);    // downcast: devuelve A* si b apunta a un A
if (a)
    // conversion exitosa
else
    // b no era un A
```

### 2.4 `const_cast`

```cpp
const_cast<tipo_destino>(expresion)
```

El unico cast que puede quitar o poner `const` o `volatile`. Se usa para interactuar con APIs antiguas que no usan `const` aunque deberian.

```cpp
void legacyPrint(char* s);  // API antigua sin const

void myFunc(const char* s) {
    legacyPrint(const_cast<char*>(s));  // quitamos const para poder llamarla
}
```

Nunca se usa para modificar un objeto que fue declarado `const` original — eso es comportamiento indefinido.

---

## 3. EX00 — ScalarConverter (`static_cast`)

### 3.1 Objetivo del ejercicio

Recibir un `string` que representa un literal de C++ (como `"42"`, `"3.14f"`, `"a"`, `"nan"`) y convertirlo a los 4 tipos escalares basicos: `char`, `int`, `float`, `double`. Mostrando cada resultado por pantalla, o `"impossible"` si la conversion no tiene sentido.

Ejemplos de entrada/salida:

```
Input: "42"
char: '*'
int: 42
float: 42.0f
double: 42.0

Input: "nan"
char: impossible
int: impossible
float: nanf
double: nan
```

### 3.2 Patron: clase no instanciable (utility class)

```cpp
// ScalarConverter.hpp:22-33
class ScalarConverter
{
    private:
        ScalarConverter();
        ~ScalarConverter();
        ScalarConverter(const ScalarConverter &);
        ScalarConverter &operator=(const ScalarConverter &);

    public:
        static void convert(const std::string &str);
};
```

Todos los constructores y el destructor estan en `private`. Esto hace que sea **imposible crear un objeto** de este tipo:

```cpp
ScalarConverter sc;    // ERROR: constructor privado
ScalarConverter* p = new ScalarConverter;  // ERROR: constructor privado
```

Solo se puede usar el metodo estatico directamente sobre la clase:

```cpp
ScalarConverter::convert("42");
```

Este patron se llama **utility class**: una clase que no representa un objeto, sino que agrupa funciones relacionadas. En la practica, funciona como un namespace con metodos. La razon de hacerlo clase en vez de funciones sueltas es para cumplir el enunciado y aplicar encapsulamiento.

### 3.3 El `enum Type`

```cpp
// ScalarConverter.hpp:12-19
enum Type {
    CHAR,
    INT,
    FLOAT,
    DOUBLE,
    PSEUDO,
    ERROR
};
```

Un `enum` es un tipo enumerado: una lista de constantes enteras con nombre. Internamente `CHAR=0`, `INT=1`, etc., pero el codigo usa los nombres para que sea legible.

Se usa para clasificar el tipo del string de entrada antes de procesarlo. Esto evita repetir la logica de deteccion en cada funcion de conversion.

### 3.4 `str_type()` — Deteccion del tipo del string

```cpp
// ScalarConverter.cpp:125-153
Type str_type(const std::string &str, size_t len)
{
    size_t dot = str.find('.');

    if (dot == std::string::npos)   // no hay punto decimal
    {
        if (isPseudo(str))   return PSEUDO;
        if (isChar(str, len)) return CHAR;
        if (isInt(str, len))  return INT;
    }
    else                            // hay punto decimal
    {
        size_t f = str.find('f');
        if (f != std::string::npos)
        {
            if (isFloat(str, len, f)) return FLOAT;
        }
        else
        {
            if (isDouble(str, len))   return DOUBLE;
        }
    }
    return ERROR;
}
```

El algoritmo de deteccion en orden de prioridad:

```
¿Tiene '.' en algun lugar?
│
├── NO:
│    ├── ¿Es nan/nanf/+inf/-inf/+inff/-inff?  → PSEUDO
│    ├── ¿Es 1 caracter no-digito?             → CHAR
│    ├── ¿Son solo digitos (con signo opcional)?→ INT
│    └── ninguno                               → ERROR
│
└── SI:
     ├── ¿Tiene 'f' en alguna posicion?
     │    ├── SI: ¿Formato valido N.Nf?        → FLOAT
     │    └── NO: (otro caracter extraño)      → ERROR
     └── NO tiene 'f':
          ├── ¿Formato valido N.N?             → DOUBLE
          └── NO                               → ERROR
```

**`isPseudo`**: compara literalmente con las 6 cadenas especiales de IEEE 754.

```cpp
// ScalarConverter.cpp:51-56
static bool isPseudo(const std::string& str)
{
    if (str == "nan" || str == "nanf" || str == "+inf" ||
        str == "+inff" || str == "-inf" || str == "-inff")
        return true;
    return false;
}
```

**`isChar`**: un solo caracter que no sea un digito. El cast `(unsigned char)str[0]` en `isdigit` es necesario porque `isdigit` espera un `unsigned char` o `EOF`; pasar un `char` con signo negativo (ej: caracteres con tilde) seria comportamiento indefinido.

```cpp
// ScalarConverter.cpp:58-63
static bool isChar(const std::string &str, size_t len)
{
    if (len == 1 && !std::isdigit((unsigned char)str[0]))
        return true;
    return false;
}
```

**`isInt`**: permite un signo `+` o `-` al inicio, luego solo digitos.

```cpp
// ScalarConverter.cpp:65-78
static bool isInt(const std::string &str, size_t len)
{
    if (str.empty()) return false;
    size_t i = 0;
    if ((str[0] == '+' || str[0] == '-') && len > 1)
        i++;
    for (; i < len; i++)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}
```

**`isFloat`**: verifica que la `f` este exactamente en la ultima posicion (`len - 1 != f` falla si no es asi), un solo punto decimal y al menos un digito.

```cpp
// ScalarConverter.cpp:80-102
static bool isFloat(const std::string &str, size_t len, size_t f)
{
    if (len - 1 != f)      // 'f' debe ser el ultimo caracter
        return false;

    size_t i = 0;
    int dots = 0, digits = 0;
    if ((str[0] == '+' || str[0] == '-') && len > 1)
        i++;
    for (; i < f; i++)     // itera hasta donde esta la 'f', sin incluirla
    {
        if (!std::isdigit(str[i]) && str[i] != '.')
            return false;
        if (str[i] == '.') dots++;
        else digits++;
    }
    if (dots != 1 || digits < 1)
        return false;
    return true;
}
```

### 3.5 Las funciones de conversion (`Transform.cpp`)

#### `printSpecial` — valores pseudo-literales de IEEE 754

```cpp
// Transform.cpp:3-26
void printSpecial(const std::string& str)
{
    if (str == "nan" || str == "nanf")
    {
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: nanf" << std::endl;
        std::cout << "double: nan" << std::endl;
    }
    // ...
}
```

`nan` (Not a Number) e `inf` (infinito) son valores especiales definidos por el estandar IEEE 754 para punto flotante. Representan resultados de operaciones invalidas (0.0/0.0 = nan) o desbordamiento (1.0/0.0 = inf). No tienen representacion en `char` ni `int`, por eso se imprime "impossible".

#### `convertChar`

```cpp
// Transform.cpp:28-40
void convertChar(const std::string& str)
{
    char c = str[0];

    std::cout << "char: ";
    if (isprint((unsigned char)c))
        std::cout << "'" << c << "'" << std::endl;
    else
        std::cout << "Non displayable" << std::endl;

    std::cout << "int: "    << static_cast<int>(c)    << std::endl;
    std::cout << "float: "  << static_cast<float>(c)  << ".0f" << std::endl;
    std::cout << "double: " << static_cast<double>(c) << ".0"  << std::endl;
}
```

- `isprint(c)` devuelve true si el caracter tiene representacion visual (letras, numeros, simbolos). Los caracteres de control (0-31) no son imprimibles.
- `static_cast<int>(c)` da el valor ASCII del caracter. 'A' = 65, 'a' = 97, '0' = 48.
- Los floats y doubles de un char siempre seran enteros, por eso se imprime `.0f` / `.0` al final de forma fija.

#### `convertInt`

```cpp
// Transform.cpp:42-65
void convertInt(const std::string& str)
{
    long l = std::atol(str.c_str());  // parsea como long

    std::cout << "char: ";
    if (l < 0 || l > 127)
        std::cout << "impossible" << std::endl;
    else
    {
        if (isprint(l))
            std::cout << "'" << static_cast<char>(l) << "'" << std::endl;
        else
            std::cout << "Non displayable" << std::endl;
    }

    std::cout << "int: ";
    if (l < INT_MIN || l > INT_MAX)
        std::cout << "impossible" << std::endl;
    else
        std::cout << static_cast<int>(l) << std::endl;

    std::cout << "float: "  << std::fixed << std::setprecision(1)
              << static_cast<float>(l) << "f" << std::endl;
    std::cout << "double: " << std::fixed << std::setprecision(1)
              << static_cast<double>(l) << std::endl;

    std::cout.unsetf(std::ios::floatfield);  // restaura formato por defecto
    std::cout << std::setprecision(6);
}
```

Puntos importantes:

- Se usa `long` en vez de `int` para poder detectar overflow. Si el numero es `99999999999`, cabe en `long` pero no en `int`. Luego se compara con `INT_MIN`/`INT_MAX` (de `<climits>`).
- `ASCII` va de 0 a 127. Si el numero esta fuera de ese rango, no tiene representacion como `char`.
- `std::fixed` y `std::setprecision(1)` fuerzan el formato `42.0` en vez de `42`.
- Al final se restaura el formato del stream con `unsetf` para no afectar futuros `cout`.

#### `convertFloat`

```cpp
// Transform.cpp:67-90
void convertFloat(const std::string& str)
{
    float f = std::atof(str.c_str());
    bool tolerance = (static_cast<long>(f) >= INT_MIN &&
                      static_cast<long>(f) <= INT_MAX) &&
                     std::fabs(f - static_cast<long>(f)) < 0.0000000000001;

    std::cout << "char: ";
    if (f < 0 || f > 127)
        std::cout << "impossible" << std::endl;
    else
    {
        if (isprint(f))
            std::cout << "'" << static_cast<char>(f) << "'" << std::endl;
        else
            std::cout << "Non displayable" << std::endl;
    }

    std::cout << "int: ";
    if (static_cast<long>(f) < INT_MIN || static_cast<long>(f) > INT_MAX)
        std::cout << "impossible" << std::endl;
    else
        std::cout << static_cast<int>(f) << std::endl;

    std::cout << "float: "  << f << (tolerance ? ".0f" : "f") << std::endl;
    std::cout << "double: " << static_cast<double>(f) << (tolerance ? ".0" : "") << std::endl;
}
```

La variable `tolerance` merece explicacion detallada:

```cpp
bool tolerance = (static_cast<long>(f) >= INT_MIN && static_cast<long>(f) <= INT_MAX) &&
                 std::fabs(f - static_cast<long>(f)) < 0.0000000000001;
```

- Primera parte: verifica que el valor quepa en un `long` (para poder hacer la resta sin overflow).
- `std::fabs(f - static_cast<long>(f))`: calcula la diferencia entre el float y su parte entera. Si el float es `42.0f`, la diferencia es exactamente `0.0`. Si es `42.5f`, es `0.5`.
- Si esa diferencia es menor que `1e-13` (practicamente cero), el float representa un numero entero. En ese caso imprimimos `.0f` para indicarlo visualmente.

Esto distingue `"42.0f"` (tolerance = true → imprime `42.0f`) de `"42.5f"` (tolerance = false → imprime `42.5f`).

#### `convertDouble`

```cpp
// Transform.cpp:92-119
void convertDouble(const std::string& str)
{
    double d = std::atof(str.c_str());
    bool tolerance = (static_cast<long long>(d) >= INT_MIN &&
                      static_cast<long long>(d) <= INT_MAX) &&
                     std::fabs(d - static_cast<long long>(d)) < 0.0000000000001;
    // ...
    std::cout << "float: ";
    if (d < -FLT_MAX || d > FLT_MAX)
        std::cout << "impossible" << std::endl;
    else
        std::cout << static_cast<float>(d) << (tolerance ? ".0f" : "f") << std::endl;
    // ...
}
```

Similar a `convertFloat`, pero usa `long long` en vez de `long` porque `double` tiene mayor precision (64 bits vs 32 bits del float). Ademas agrega una verificacion de rango para `float`: si el double es mayor que `FLT_MAX` (maximo valor de float, ~3.4e38), la conversion a float es imposible.

---

## 4. EX01 — Serializer (`reinterpret_cast`)

### 4.1 Objetivo del ejercicio

Demostrar que un puntero puede ser convertido a un entero y recuperado de vuelta sin perdida de informacion. Esto es la base conceptual de la "serializacion": transformar algo a un formato que se puede transmitir o almacenar, y luego restaurarlo.

### 4.2 El tipo `uintptr_t`

```cpp
// Serializer.hpp:7
#include <stdint.h>
```

`uintptr_t` es un tipo definido en `<stdint.h>` (o `<cstdint>` en C++). Significa "unsigned integer capable of holding a pointer" (entero sin signo capaz de almacenar un puntero).

¿Por que no usar simplemente `int` o `long`?

- En sistemas de 32 bits, los punteros tienen 4 bytes → `int` funcionaria
- En sistemas de 64 bits, los punteros tienen 8 bytes → `int` (4 bytes) perderia informacion

`uintptr_t` siempre es del tamano correcto para la arquitectura actual. En 64 bits es equivalente a `unsigned long long` (8 bytes).

### 4.3 La clase `Data`

```cpp
// Data.hpp:6-10
struct Data
{
    int         _num;
    std::string _str;
};
```

Una estructura simple con dos campos. En C++, `struct` y `class` son casi identicos; la unica diferencia es que en `struct` los miembros son `public` por defecto. `Data` solo existe para tener algo concreto que serializar.

### 4.4 `serialize` y `deserialize`

```cpp
// Serializer.cpp:23-31
uintptr_t Serializer::serialize(Data* ptr)
{
    return (reinterpret_cast<uintptr_t>(ptr));
}

Data* Serializer::deserialize(uintptr_t raw)
{
    return (reinterpret_cast<Data*>(raw));
}
```

Estas dos funciones son inversas:

```
Data*  ──[serialize]──►  uintptr_t  ──[deserialize]──►  Data*
  &data        →          0x7fff...        →              &data
```

`reinterpret_cast<uintptr_t>(ptr)` toma los bytes que forman la direccion de memoria del puntero y los interpreta como un numero entero. No hay ninguna conversion aritmetica: los bits son identicos.

`reinterpret_cast<Data*>(raw)` hace lo inverso: toma los bytes del entero y los interpreta como si fuesen una direccion de memoria a `Data`.

Si los bits se conservan identicos en ambas operaciones (lo cual garantiza el estandar de C++ para `uintptr_t`), el puntero resultante apunta exactamente al mismo objeto.

### 4.5 Lo que demuestra el `main`

```cpp
// main.cpp:6-33
Data data;
data._num = 42;
data._str = "hello";

std::cout << "Address of data: " << &data << std::endl;
// Ejemplo de salida: Address of data: 0x7ffee4b2a9a0

uintptr_t raw = Serializer::serialize(&data);
std::cout << "Result of serialize(&data): " << raw << std::endl;
// Ejemplo: Result of serialize(&data): 140732511111584
// (mismo numero que la direccion, expresado en decimal)

Data *ptr = Serializer::deserialize(raw);
std::cout << "Deserialized address: " << ptr << std::endl;
// Ejemplo: Deserialized address: 0x7ffee4b2a9a0  (identico al original)

if (ptr == &data)
    std::cout << "Pointers are equal: serialization works correctly" << std::endl;
```

El valor `raw` es literalmente la direccion de memoria de `data` expresada como un numero. `0x7ffee4b2a9a0` en hexadecimal = `140732511111584` en decimal. Son los mismos bits.

La prueba final `ptr == &data` verifica que el puntero recuperado apunta exactamente al mismo objeto original. Si es true, la serializacion funciona.

### 4.6 Uso real de `reinterpret_cast`

En la practica real, `reinterpret_cast` aparece en:

- **Programacion de sistemas**: acceder a registros de hardware a direcciones especificas
- **Protocolos de red**: interpretar buffers de bytes como estructuras
- **Hashing de punteros**: usar la direccion como identificador unico
- **Interoperabilidad con C**: APIs que pasan datos como `void*` o `uintptr_t`

---

## 5. EX02 — Type Identification (`dynamic_cast`)

### 5.1 Objetivo del ejercicio

Dado un puntero `Base*` que en realidad apunta a un objeto de tipo `A`, `B` o `C`, identificar el tipo real sin usar `typeinfo` ni ninguna variable que guarde el tipo. La unica herramienta permitida es `dynamic_cast`.

### 5.2 La jerarquia de clases

```
Base  (clase abstracta con destructor virtual)
 ├── A  (hereda de Base, sin miembros propios)
 ├── B  (hereda de Base, sin miembros propios)
 └── C  (hereda de Base, sin miembros propios)
```

```cpp
// Base.hpp:4-9
class Base
{
    public:
        virtual ~Base();  // destructor virtual: clave para dynamic_cast
};

// A.hpp
class A : public Base {};

// B.hpp
class B : public Base {};

// C.hpp
class C : public Base {};
```

A, B y C no tienen ningun miembro adicional. Solo existen para ser tipos distintos en la jerarquia.

### 5.3 ¿Por qué `virtual ~Base()` es obligatorio?

`dynamic_cast` requiere que la clase tenga **RTTI** (Run-Time Type Information). El compilador genera RTTI para una clase cuando tiene al menos una funcion `virtual`. Sin RTTI, `dynamic_cast` no puede consultar el tipo real en runtime y el compilador rechaza el codigo con error.

El destructor virtual ademas tiene otra funcion importante: asegura que al hacer `delete base_ptr` se llame al destructor correcto:

```cpp
Base* p = new A;
delete p;   // Sin virtual: llama ~Base() y puede dejar memoria sin liberar
            // Con virtual: llama ~A() primero, luego ~Base()
```

En este ejercicio A, B y C no tienen destructores propios, pero el patron es la buena practica.

### 5.4 `generate()` — Polimorfismo y upcast

```cpp
// main.cpp:10-19
static Base *generate(void)
{
    int i = rand() % 3;
    if (i == 0) return (new A);
    else if (i == 1) return (new B);
    else return (new C);
}
```

`rand() % 3` da un numero entre 0 y 2 de forma aleatoria. Se usa `std::srand(std::time(NULL))` en el main para semillar el generador con la hora actual y que sea distinto cada vez.

La funcion retorna `Base*` aunque crea `A`, `B` o `C`. Esto es el **upcast**: convertir un puntero de clase derivada a clase base. Es siempre implicito y seguro porque un `A` "es un" `Base` (herencia publica).

```cpp
A* a = new A;
Base* b = a;    // upcast implicito: siempre valido
```

El resultado es que el llamante solo ve un `Base*` y no sabe que tipo concreto hay debajo.

### 5.5 `identify(Base* p)` — Downcast con puntero

```cpp
// main.cpp:21-29
static void identify(Base *p)
{
    if (dynamic_cast<A*>(p))
        std::cout << "A" << std::endl;
    else if (dynamic_cast<B*>(p))
        std::cout << "B" << std::endl;
    else if (dynamic_cast<C*>(p))
        std::cout << "C" << std::endl;
}
```

`dynamic_cast<A*>(p)` hace el **downcast**: intenta convertir `Base*` a `A*`. En runtime consulta la RTTI del objeto apuntado por `p`:

- Si el objeto real es un `A` (o deriva de `A`): devuelve un `A*` valido (diferente de nullptr)
- Si no: devuelve `nullptr`

Cuando se usa en condicion `if`, `nullptr` es `false` y cualquier otro puntero es `true`. Por eso el codigo funciona sin necesidad de guardar el resultado:

```cpp
if (dynamic_cast<A*>(p))  // true solo si p apunta a un A
```

Esto es el downcast seguro. El cast de C equivalente `(A*)p` tambien funcionaria aqui, pero si `p` no apunta a un `A`, tendriamos un puntero invalido sin ningun aviso — comportamiento indefinido.

### 5.6 `identify(Base& p)` — Downcast con referencia

```cpp
// main.cpp:31-63
static void identify(Base &p)
{
    try
    {
        A &a = dynamic_cast<A&>(p);  // lanza std::bad_cast si p no es A
        (void)a;                      // evita warning: variable 'a' no usada
        std::cout << "A" << std::endl;
        return;
    }
    catch (std::exception &e) {}     // falla silenciosamente, prueba B

    try
    {
        B &b = dynamic_cast<B&>(p);
        (void)b;
        std::cout << "B" << std::endl;
        return;
    }
    catch (std::exception &e) {}

    try
    {
        C &c = dynamic_cast<C&>(p);
        (void)c;
        std::cout << "C" << std::endl;
        return;
    }
    catch (std::exception &e) {}
}
```

Las referencias en C++ no pueden ser `nullptr`. Una referencia siempre apunta a algo valido. Por eso `dynamic_cast` con referencias no puede devolver "nada" en caso de fallo: en su lugar lanza la excepcion `std::bad_cast` (que hereda de `std::exception`).

El patron es: intentar el cast en un `try`, capturar el fallo en el `catch` y continuar con el siguiente tipo.

`(void)a` es un cast a `void` que no hace nada en runtime: solo le dice al compilador "se que esta variable no se usa, no me des el warning". Sin el, el compilador avisaria que `a` fue declarada pero nunca utilizada.

### 5.7 Llamada desde el main

```cpp
// main.cpp:65-83
int main()
{
    std::srand(std::time(NULL));  // semilla para rand() basada en tiempo actual

    for (int i = 0; i < 5; i++)
    {
        Base *obj = generate();    // crea A, B o C aleatoriamente como Base*

        std::cout << "Object " << i + 1 << ": pointer: ";
        identify(obj);             // version con puntero

        std::cout << "Object " << i + 1 << ": reference: ";
        identify(*obj);            // *obj desreferencia el puntero → referencia Base&

        delete obj;                // libera memoria del objeto
    }
    return 0;
}
```

`identify(obj)` llama a la version que recibe `Base*`.
`identify(*obj)` desreferencia el puntero: `*obj` es un `Base&` (referencia al objeto), y llama a la version que recibe `Base&`.

Ambas deben dar el mismo resultado para el mismo objeto.

---

## 6. Tabla resumen final

| Cast | ¿Cuando usarlo? | ¿Que verifica? | ¿Puede fallar? |
|------|----------------|----------------|----------------|
| `static_cast` | Conversiones numericas, upcasts/downcasts sin polimorfismo | Compile-time | No en runtime (puede truncar datos) |
| `reinterpret_cast` | Reinterpretar bits: puntero↔entero, puntero↔void* | Nada | No (pero es inseguro si se usa mal) |
| `dynamic_cast` | Downcast seguro en jerarquias polimorficas (con virtual) | Runtime (RTTI) | Si: nullptr o bad_cast |
| `const_cast` | Quitar/poner const o volatile | Compile-time | No (UB si modificas un const real) |

### Relacion entre los ejercicios y los casts

| Ejercicio | Cast usado | Concepto principal |
|-----------|-----------|-------------------|
| ex00 ScalarConverter | `static_cast` | Conversion entre tipos numericos compatibles |
| ex01 Serializer | `reinterpret_cast` | Reinterpretacion de bits (puntero ↔ entero) |
| ex02 Type Identification | `dynamic_cast` | Identificacion de tipo en runtime via RTTI |

### Conceptos adicionales que aparecen en el modulo

- **Utility class (clase no instanciable)**: constructor privado, solo metodos estaticos
- **`uintptr_t`**: entero garantizado de tener el mismo tamano que un puntero
- **IEEE 754**: estandar de punto flotante, define nan, inf, -inf
- **RTTI** (Run-Time Type Information): informacion de tipo generada por el compilador para clases con funciones virtuales
- **Polimorfismo**: usar un `Base*` para apuntar a objetos de tipos derivados
- **Upcast**: derivada → base, siempre seguro e implicito
- **Downcast**: base → derivada, puede fallar si el tipo real no coincide
- **Destructor virtual**: necesario en clases base para correcta destruccion de objetos derivados y para habilitar RTTI
