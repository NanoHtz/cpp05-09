# CPP06 — C++ Casts

## Regla adicional del módulo

En cada ejercicio, las conversiones de tipo **deben hacerse con un cast específico de C++**
(`static_cast`, `reinterpret_cast`, `dynamic_cast`). El evaluador preguntará por qué
elegiste cada uno.

---

## Exercise 00 — Conversion of scalar types

### Qué hace

La clase `ScalarConverter` expone un único método estático `convert(string)` que recibe
la representación en string de un literal de C++ y lo muestra convertido a los cuatro
tipos escalares: `char`, `int`, `float` y `double`.

### Conceptos clave

#### `static_cast`
El cast que se usa aquí. Es la conversión explícita "segura" entre tipos relacionados o
numéricos. El compilador la verifica en tiempo de compilación.

```cpp
static_cast<int>(97.5)   // → 97
static_cast<char>(65)    // → 'A'
static_cast<float>(42)   // → 42.0f
```

Frente al cast de C (`(int)x`), `static_cast` es preferible porque:
- Es visible en el código y fácil de buscar con grep.
- El compilador rechaza conversiones que no tienen sentido (p.ej. punteros sin relación).

#### Detección del tipo del literal
El programa debe distinguir si el string es un `char`, `int`, `float`, `double` o
pseudo-literal (`nan`, `nanf`, `+inf`, etc.) antes de convertir. Esto se hace
manualmente inspeccionando el string (longitud, presencia de `.`, de `f`, de signos...).

#### Límites y casos especiales
- `<climits>` → `INT_MIN`, `INT_MAX`
- `<cfloat>` → `FLT_MAX`
- `<cmath>` → `std::fabs`, `std::isinf`, `std::isnan`
- `<iomanip>` → `std::fixed`, `std::setprecision` para formatear la salida decimal

Casos que hay que manejar:
| Input | Char | Int | Float | Double |
|---|---|---|---|---|
| `0` | Non displayable | 0 | 0.0f | 0.0 |
| `nan` / `nanf` | impossible | impossible | nanf | nan |
| `+inf` / `+inff` | impossible | impossible | +inff | +inf |
| `-42` | impossible | -42 | -42.0f | -42.0 |
| `42.0f` | `'*'` | 42 | 42.0f | 42.0 |

#### Por qué la clase no es instanciable
El subject exige que `ScalarConverter` **no pueda instanciarse**. Se consigue poniendo
constructor, destructor, copia y asignación en `private`. Esto modela la idea de una
clase que es solo un namespace con funcionalidad — no tiene estado propio.

### Importancia
Ejercita la comprensión de los tipos escalares de C++, sus rangos y cómo se
representan. También obliga a pensar en overflow, valores no representables e imprecisión
de punto flotante.

---

## Exercise 01 — Serialization

### Qué hace

La clase `Serializer` convierte un puntero `Data*` a un entero sin signo (`uintptr_t`)
y viceversa, demostrando que el puntero original y el recuperado son idénticos.

```
Data* ptr → serialize → uintptr_t → deserialize → Data* (== ptr original)
```

### Conceptos clave

#### `reinterpret_cast`
El cast que se usa aquí. Reinterpreta los bits de un tipo como si fueran de otro tipo
completamente distinto, sin ninguna conversión. Es el cast más peligroso y el menos
verificado por el compilador.

```cpp
uintptr_t raw = reinterpret_cast<uintptr_t>(ptr);   // puntero → entero
Data* p       = reinterpret_cast<Data*>(raw);        // entero → puntero
```

Cuándo usarlo: únicamente cuando sabes exactamente qué estás haciendo a nivel de bits
(serialización, bajo nivel, hardware). En código de aplicación normal no debe aparecer.

#### `uintptr_t`
Tipo entero sin signo garantizado para ser suficientemente grande para almacenar un
puntero. Viene de `<stdint.h>` (C++98) o `<cstdint>` (C++11). Su tamaño depende de la
arquitectura: 32 bits en x86, 64 bits en x86-64.

#### Por qué no se puede hacer con `static_cast`
`static_cast` no permite convertir entre punteros y enteros porque no existe una
relación de herencia ni una conversión numérica estándar entre ellos. El compilador lo
rechaza. Hay que usar `reinterpret_cast` explícitamente para indicar que sabes que es
una operación de bajo nivel.

#### `Data` no vacía
El subject exige que `Data` tenga al menos un miembro. Esto importa para que la
serialización tenga sentido real: se puede verificar que, tras el viaje
`ptr → raw → ptr`, los datos siguen accesibles y son los mismos.

### Importancia
Enseña qué significa realmente un puntero: es una dirección de memoria, que puede
representarse como un entero. Este concepto es la base de la programación de sistemas,
drivers y cualquier código que interactúe con memoria directamente.

---

## Exercise 02 — Identify real type

### Qué hace

Dado un puntero o referencia a `Base`, determina en tiempo de ejecución si el objeto
real es de tipo `A`, `B` o `C`.

```cpp
Base *obj = generate();   // crea A, B o C aleatoriamente
identify(obj);            // imprime "A", "B" o "C" — versión puntero
identify(*obj);           // imprime "A", "B" o "C" — versión referencia
```

### Conceptos clave

#### `dynamic_cast`
El cast que se usa aquí. Realiza una conversión segura en la jerarquía de herencia,
verificada **en tiempo de ejecución** mediante RTTI (Run-Time Type Information).

- Con **punteros**: devuelve `nullptr` si el cast falla.
- Con **referencias**: lanza `std::bad_cast` si el cast falla.

```cpp
// Versión puntero — fallo → nullptr
if (dynamic_cast<A*>(p))
    std::cout << "A";

// Versión referencia — fallo → excepción
try { A &a = dynamic_cast<A&>(p); std::cout << "A"; }
catch (std::exception &) {}
```

#### Por qué `Base` necesita el destructor virtual
`dynamic_cast` requiere que la clase base tenga **al menos una función virtual**. Sin
ella, el compilador no genera la vtable y el RTTI no existe → error de compilación.
El destructor virtual también es correcto por diseño: sin él, hacer `delete base_ptr`
cuando el objeto real es `A` causaría **undefined behavior** (destructor incorrecto).

#### Por qué está prohibido `<typeinfo>` y usar puntero en `identify(Base&)`
El objetivo del ejercicio es aprender a usar `dynamic_cast` correctamente. `typeid`
(de `<typeinfo>`) sería una trampa que evita entender el mecanismo real. Igualmente,
la versión de referencia obliga a manejar excepciones en lugar de comparar con nullptr
como en la versión de puntero.

#### RTTI (Run-Time Type Information)
Es el mecanismo interno del compilador que permite conocer el tipo real de un objeto
polimórfico en tiempo de ejecución. Se almacena en la vtable. `dynamic_cast` y `typeid`
lo usan. Tiene un pequeño coste en rendimiento, motivo por el que en algunos proyectos
de sistemas se desactiva (`-fno-rtti`).

### Importancia
Ejercita el polimorfismo real de C++: trabajar con objetos a través de su tipo base sin
saber su tipo concreto en tiempo de compilación. `dynamic_cast` es la herramienta
correcta cuando necesitas hacer downcasting de forma segura. Si te ves usando
`dynamic_cast` mucho en código de producción, es una señal de mal diseño (prefiere
funciones virtuales), pero saber cómo funciona es fundamental.

---

## Comparativa de los cuatro casts de C++

| Cast | Uso | Verificación | Riesgo |
|---|---|---|---|
| `static_cast` | Conversiones numéricas, jerarquía conocida | Compilación | Bajo |
| `dynamic_cast` | Downcast seguro en jerarquía polimórfica | Ejecución (RTTI) | Bajo |
| `reinterpret_cast` | Reinterpretación de bits, bajo nivel | Ninguna | Alto |
| `const_cast` | Añadir/quitar `const` | Compilación | Medio |

Los casts de C (`(int)x`) hacen lo que sea necesario (intentan `static`, luego `reinterpret`,
luego `const_cast`) sin dejar rastro claro en el código. Los casts de C++ son explícitos,
buscables y comunican la intención al lector.
