# Dynamic Skyline Maintenance with Persistent Range Queries

Implementación en C++ para mantener dinámicamente un skyline formado por rectángulos activos sobre una línea discreta. El programa soporta inserciones, eliminaciones y consultas de rango usando una solución offline basada en:

- Compresión de coordenadas.
- Segment tree sobre el tiempo.
- Segment tree persistente sobre coordenadas.
- Lazy propagation para actualizaciones de máximo.
- Consultas de mínimo sobre el skyline resultante.

## Descripción del problema

Se procesan `q` operaciones sobre rectángulos de la forma:

```txt
+ l r h
```

donde el rectángulo cubre el intervalo `[l, r)` con altura `h`.

En cualquier punto `x`, la altura visible del skyline es el máximo `h` entre todos los rectángulos activos que cubren `x`. Si no hay ningún rectángulo activo sobre `x`, la altura es `0`.

Las consultas preguntan por la menor altura visible dentro de un intervalo:

```txt
? l r
```

es decir, se responde:

```txt
min skyline[x] para x en [l, r)
```

También se puede eliminar un rectángulo previamente agregado:

```txt
- j
```

donde `j` es el índice, 1-indexado, de la operación de inserción correspondiente.

## Formato de entrada

La primera línea contiene:

```txt
q
```

Luego siguen `q` líneas, cada una con una operación.

### Insertar rectángulo

```txt
+ l r h
```

Agrega un rectángulo activo en el intervalo `[l, r)` con altura `h`.

### Eliminar rectángulo

```txt
- j
```

Elimina el `j`-ésimo rectángulo insertado. Se asume que la eliminación es válida.

### Consultar rango

```txt
? l r
```

Imprime la mínima altura visible del skyline en el intervalo `[l, r)`.

## Formato de salida

Por cada operación `?`, se imprime una línea con la respuesta correspondiente.

## Ejemplo

### Entrada

```txt
8
? 1 5
+ 1 5 3
? 2 4
+ 3 7 5
? 2 6
- 1
? 2 6
? 3 6
```

### Salida

```txt
0
3
3
0
5
```

## Archivos del repositorio

```txt
.
├── README.md
├── main.cpp
├── brute.cpp
├── gen.cpp
├── gen_tests.sh
├── main
├── gen
├── in
├── out
└── tests
    ├── small_cases
    ├── standar_cases
    └── big_cases
```

### `main.cpp`

Solución optimizada del problema.

Componentes principales:

- Lee todas las operaciones.
- Comprime coordenadas.
- Calcula el intervalo de vida de cada rectángulo.
- Inserta cada rectángulo en un segment tree sobre el tiempo.
- Recorre ese árbol con DFS.
- Mantiene el skyline actual usando un segment tree persistente sobre coordenadas.
- Responde las consultas `?` en las hojas del árbol de tiempo.

La solución trabaja offline: primero lee todas las operaciones y luego procesa las consultas con la información completa de inserciones y eliminaciones.

### `brute.cpp`

Solución lenta para validación.

Mantiene, para cada posición comprimida, un `multiset` con las alturas activas. Para cada consulta, revisa directamente el rango comprimido y toma el mínimo de las alturas visibles.

Útil para comparar resultados contra `main.cpp` en casos pequeños o medianos.

### `gen.cpp`

Generador aleatorio de casos.

Recibe por entrada:

```txt
q mx
```

donde:

- `q` es la cantidad de operaciones.
- `mx` es el máximo valor usado para coordenadas y alturas.

Produce un caso de prueba con operaciones aleatorias `+`, `-` y `?`.

### `gen_tests.sh`

Script para generar múltiples casos de prueba usando el ejecutable `gen`.

Parámetros configurables dentro del script:

```bash
GEN="./gen"
OUT_DIR="standar_cases"
BASE_NAME="test"
NUM_FILES=10

Q=100000
MX=1000000000
```

Por defecto genera 10 archivos:

```txt
test_01.txt
test_02.txt
...
test_10.txt
```

en la carpeta `standar_cases`.

### `tests/`

Contiene casos de prueba generados:

```txt
tests/small_cases/
tests/standar_cases/
tests/big_cases/
```

Cada carpeta contiene archivos `test_01.txt` a `test_10.txt`.

### `in` y `out`

Archivos auxiliares para entrada y salida local.

### `main` y `gen`

Ejecutables compilados. No son necesarios para entender el código fuente y normalmente deberían excluirse del repositorio mediante `.gitignore`.

## Compilación

Requiere un compilador compatible con C++17.

### Solución principal

```bash
g++ -std=c++17 -O2 -o main main.cpp
```

### Solución bruta

```bash
g++ -std=c++17 -O2 -o brute brute.cpp
```

### Generador

```bash
g++ -std=c++17 -O2 -o gen gen.cpp
```

## Uso

Ejecutar la solución principal con un caso de prueba:

```bash
./main < tests/small_cases/test_01.txt
```

Guardar la salida:

```bash
./main < tests/small_cases/test_01.txt > answer.txt
```

## Generar nuevos tests

Primero compilar el generador:

```bash
g++ -std=c++17 -O2 -o gen gen.cpp
```

Luego ejecutar:

```bash
chmod +x gen_tests.sh
./gen_tests.sh
```

El script genera casos usando los valores configurados en `gen_tests.sh`.

Si se quiere guardar los casos dentro de `tests/standar_cases`, modificar:

```bash
OUT_DIR="standar_cases"
```

por:

```bash
OUT_DIR="tests/standar_cases"
```

## Validación con fuerza bruta

Para comparar `main.cpp` contra `brute.cpp` en casos pequeños:

```bash
g++ -std=c++17 -O2 -o main main.cpp
g++ -std=c++17 -O2 -o brute brute.cpp

for f in tests/small_cases/*.txt; do
    echo "Testing $f"
    ./main < "$f" > out_main.txt
    ./brute < "$f" > out_brute.txt
    diff -u out_brute.txt out_main.txt || exit 1
done

echo "All tests passed"
```

Para casos grandes, `brute.cpp` puede ser demasiado lento.

## Idea del algoritmo

La dificultad principal es manejar rectángulos que aparecen y desaparecen en el tiempo. Para resolverlo, el algoritmo convierte cada rectángulo en un intervalo de vida temporal.

Si un rectángulo se agrega en la operación `in[i]` y se elimina en `out[i]`, entonces está activo durante ese rango de tiempo. Ese intervalo se inserta en un segment tree sobre las operaciones.

Durante un DFS sobre este árbol de tiempo:

1. Se aplican todos los rectángulos asociados al nodo actual.
2. Se actualiza una estructura persistente sobre coordenadas.
3. Si el nodo representa una sola operación y esa operación es una consulta, se responde usando el estado actual.
4. Al volver del DFS, se restauran los nodos persistentes creados en esa rama.

## Segment tree persistente

El segment tree persistente sobre coordenadas mantiene la altura visible del skyline.

Para cada rectángulo activo:

```txt
[l, r) con altura h
```

se aplica una actualización de rango:

```txt
skyline[x] = max(skyline[x], h)
```

Luego, para una consulta:

```txt
? l r
```

se calcula:

```txt
min skyline[x] en [l, r)
```

El árbol usa lazy propagation para aplicar máximos de forma eficiente.

## Compresión de coordenadas

Como las coordenadas pueden ser grandes, el código comprime los valores relevantes.

Para cada intervalo `[l, r)`, se guardan:

```txt
l
r
r - 1
```

Esto permite trabajar sobre posiciones discretas preservando la semántica de intervalos semiabiertos.

## Complejidad

Sea:

- `q` = número de operaciones.
- `m` = número de coordenadas comprimidas.
- `a` = número de rectángulos agregados.

Cada rectángulo se inserta en `O(log q)` nodos del segment tree de tiempo. Cada aplicación sobre coordenadas cuesta `O(log m)`.

Complejidad aproximada:

```txt
O(a log q log m + queries log m)
```

Memoria aproximada:

```txt
O(a log q log m)
```

Además, `main.cpp` reserva arreglos estáticos grandes para el segment tree persistente, por lo que puede requerir bastante memoria en ejecuciones con límites estrictos.

## Notas importantes

- Las eliminaciones `- j` deben referirse a rectángulos previamente insertados.
- El índice `j` es 1-indexado.
- Los intervalos se interpretan como `[l, r)`.
- Si un rango consultado tiene posiciones sin rectángulos activos, esas posiciones tienen altura `0`.
- El archivo `brute.cpp` está pensado para verificación, no para casos grandes.
- El script usa el nombre `standar_cases`; si se desea, se puede renombrar a `standard_cases` para mayor claridad.

## Recomendaciones para limpieza del repositorio

Agregar un `.gitignore` para evitar subir ejecutables y archivos temporales:

```gitignore
main
gen
brute
*.out
out
in
out_main.txt
out_brute.txt
answer.txt
```

Opcionalmente, separar los archivos generados de los archivos fuente:

```txt
src/
├── main.cpp
├── brute.cpp
└── gen.cpp

scripts/
└── gen_tests.sh

tests/
├── small_cases/
├── standard_cases/
└── big_cases/
```

## Licencia

No se especifica una licencia en el repositorio actual.
