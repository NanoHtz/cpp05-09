# EX01 — Span

## Que hace

Implementa una clase `Span` que almacena hasta `N` enteros y puede calcular el **span mas corto** y el **span mas largo** entre todos los numeros almacenados.

> El "span" (distancia) entre dos numeros es la diferencia absoluta entre ellos.

---

## Span.hpp — Explicacion detallada

```cpp
class Span
{
    private:
        unsigned int        _n;
        std::vector<int>    _numbers;

    public:
        Span();
        Span(unsigned int n);
        Span(const Span &src);
        Span &operator=(const Span &src);
        ~Span();

        void    addNumber(int number);
        int     shortestSpan() const;
        int     longestSpan() const;
};
```

---

### Atributos privados

#### `unsigned int _n`
La capacidad maxima del Span. Se fija en el constructor y no cambia. Si `_numbers` ya tiene `_n` elementos, `addNumber` debe lanzar excepcion.

#### `std::vector<int> _numbers`
El contenedor interno donde se guardan los numeros. Se usa `vector` porque:
- Tiene acceso aleatorio O(1)
- Es compatible con `std::sort` y algoritmos STL
- Soporta `begin()` / `end()` para iteradores

---

### Constructor

```cpp
Span(unsigned int n);
```
Recibe la capacidad maxima. Inicializa `_n = n` y deja `_numbers` vacio.

---

### `addNumber(int number)`

Añade un numero al vector interno. Debe:
1. Comprobar si `_numbers.size() >= _n` → si es asi, lanzar excepcion
2. Hacer `_numbers.push_back(number)`

---

### `shortestSpan() const`

Encuentra la menor diferencia entre cualquier par de numeros. El truco STL:
1. Hacer una copia del vector y ordenarla con `std::sort`
2. Recorrer la copia comparando elementos **adyacentes** (tras ordenar, los mas cercanos siempre son vecinos)
3. Retornar la minima diferencia encontrada

Si hay 0 o 1 elementos → lanzar excepcion (no hay par posible).

```
Vector ordenado: [3, 6, 9, 17, 11]
→ sort → [3, 6, 9, 11, 17]
→ diferencias adyacentes: 3, 3, 2, 6
→ shortestSpan = 2
```

---

### `longestSpan() const`

Diferencia entre el maximo y el minimo del vector. Con STL:

```cpp
*std::max_element(_numbers.begin(), _numbers.end())
- *std::min_element(_numbers.begin(), _numbers.end())
```

Si hay 0 o 1 elementos → lanzar excepcion.

```
Vector: [6, 3, 17, 9, 11]
→ max = 17, min = 3
→ longestSpan = 14
```

---

### Forma canonica ortodoxa

El subject exige que las clases de cpp02 a cpp09 cumplan la **Orthodox Canonical Form**:
- Constructor por defecto
- Constructor por copia
- Operador de asignacion
- Destructor

---

## Ejemplo del subject

```cpp
Span sp = Span(5);
sp.addNumber(6);
sp.addNumber(3);
sp.addNumber(17);
sp.addNumber(9);
sp.addNumber(11);

std::cout << sp.shortestSpan() << std::endl;  // 2
std::cout << sp.longestSpan()  << std::endl;  // 14
```

---

## Bonus — Añadir con rango de iteradores

El subject pide tambien un metodo para añadir varios numeros de golpe usando iteradores:

```cpp
template<typename Iterator>
void addRange(Iterator begin, Iterator end);
```

Esto permite hacer:
```cpp
std::vector<int> src = { ... };
sp.addRange(src.begin(), src.end());
```

En lugar de llamar `addNumber` miles de veces.

---

## Resumen del flujo

```
Span(N)
    → guarda capacidad maxima

addNumber(x)
    → si lleno → exception
    → push_back(x)

shortestSpan()
    → si < 2 elementos → exception
    → sort copia → min diferencia adyacente

longestSpan()
    → si < 2 elementos → exception
    → max_element - min_element
```

## Conceptos STL usados

| Funcion STL | Para que se usa |
|------------|----------------|
| `std::sort` | Ordenar el vector para shortestSpan |
| `std::min_element` | Encontrar el minimo para longestSpan |
| `std::max_element` | Encontrar el maximo para longestSpan |
| `std::vector::push_back` | Añadir elementos |
| `std::vector::size` | Comprobar si esta lleno |
