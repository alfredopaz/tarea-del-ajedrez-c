# Ajedrez en C: composición de imágenes

Proyecto docente para construir imágenes mediante la API de `chess.h`.
Compila con **GCC y C11**, sin C++ ni advertencias ocultas.

## Compilar y ejecutar en Ubuntu

Instalar las dependencias de desarrollo si no están disponibles:

```sh
sudo apt install build-essential freeglut3-dev libgl-dev
make
./ejemplo
```

Para un archivo propio que implemente `void display(void)`:

```sh
make HW=mi_posicion
./mi_posicion
```

`HW` es el nombre sin `.c`; admite rutas como `examples/pieza`.
Los comandos se ejecutan desde la raíz del proyecto. No usar espacios en HW.
Las dependencias de cabeceras se registran automáticamente y los objetos se
construyen dentro de `build/`. `make clean HW=mi_posicion` elimina esos objetos
y los ejecutables de ese HW; no borra otros ejercicios.

### Sin OpenGL ni ventana

Solo se necesitan GCC y make. El mismo `display()` genera una imagen PPM:

```sh
make headless
./ejemplo-ppm tablero.ppm
make headless HW=examples/pieza
./examples/pieza-ppm caballo.ppm
```

Si no se indica un archivo de salida, se usa `chess.ppm` (se sobrescribe).
El ejecutable devuelve un código distinto de cero si no consigue escribir
una imagen válida. PPM es un formato de imagen que admiten diversos visores.

## Trabajo del estudiante

Incluir `chess.h` y `figures.h`, implementar `display()` y llamar una sola vez
a `interpreter(imagen)`. Se pueden crear funciones auxiliares. No se debe
escribir otro `main()`. `ejemplo.c` construye un tablero vacío y
`examples/pieza.c` muestra un caballo negro sobre una casilla blanca.

Las figuras son de 58 × 58 píxeles. Con las piezas blancas al pie del tablero,
la esquina superior izquierda (a8) es blanca y la inferior izquierda (a1)
es negra.

| Operación | Resultado / condición |
| --- | --- |
| `reverse(fig)` | Intercambia los colores de la figura. |
| `join(a, b)` | a a la izquierda de b; alturas iguales. |
| `up(a, b)` | a encima de b; anchos iguales. |
| `repeatH(fig, n)` | n copias horizontales; n > 0. |
| `repeatV(fig, n)` | n copias verticales; n > 0. |
| `superImpose(a, b)` | a sobre b; dimensiones iguales; espacio transparente. |
| `flipV(fig)` | Invierte arriba y abajo. |
| `flipH(fig)` | Invierte izquierda y derecha. |
| `rotateL(fig)` | Gira 90° en sentido antihorario. |
| `rotateR(fig)` | Gira 90° en sentido horario. |

Las funciones no modifican sus argumentos. Los resultados son independientes
de sus entradas y pueden reutilizarse dentro del mismo dibujo. Las figuras
base se declaran en `figures.h` y se definen en `figures.c`, lo que permite
incluir la cabecera desde varios módulos sin definiciones duplicadas.

### Colores y transparencia

`_` es fondo blanco y `@` es fondo negro; `.` es blanco de la pieza y `#`
es gris oscuro de la pieza. `reverse()` intercambia `_` con `@` y `.` con `#`.
El espacio es transparente al superponer; `=` conserva su gris. Los caracteres
no reconocidos se conservan y se dibujan grises, al igual que un espacio
que llega al intérprete sin haber sido superpuesto sobre un fondo.

`reverse()` también invierte un fondo ya incorporado. Para cambiar solo el
color de una pieza, invertirla ANTES de superponerla sobre su casilla.

### Errores de la API

Una imagen es un arreglo de cadenas de igual longitud, terminado en `NULL`.
Cada cadena debe terminar en `'\0'`. Se exige al menos una fila y una columna.
Los argumentos deben apuntar a arreglos válidos con sus terminadores: C no
permite validar de forma general un puntero arbitrario o un arreglo truncado.

Las operaciones retornan `NULL` y fijan `errno` ante argumentos inválidos
(`EINVAL`), desbordamiento de tamaño (`EOVERFLOW`) o falta de memoria (`ENOMEM`).
En código con diagnóstico detallado, comprobar cada resultado antes de la
siguiente operación: una llamada posterior puede cambiar `errno`.

## Memoria automática durante cada dibujo

Esta versión emplea **liberación por ciclo de dibujo**, no un recolector que
rastree las referencias alcanzables de C. El nombre histórico
`garbageCollector()` se conserva para la limpieza interna de todos los bloques.

1. Cada operación reserva una imagen en un solo bloque (punteros y caracteres).
2. El gestor guarda los bloques reservados, nunca las direcciones de variables
   locales del estudiante.
3. El motor llama a `display()`. Todas las imágenes intermedias permanecen
   válidas durante esa llamada, incluidas las de expresiones anidadas.
4. Al retornar `display()`, el motor libera todas las imágenes temporales.
5. También se registra la limpieza con `atexit()` para una salida normal.

Los estudiantes **no necesitan incluir `gc.h`, registrar referencias ni llamar
a `free()`**. No deben conservar resultados de la API en variables estáticas o
globales para usarlos en el siguiente dibujo, ni llamar al recolector desde
`display()`. Las figuras base sí permanecen disponibles durante todo el programa.

El gestor es de un solo hilo. No se usa `free()` ni `realloc()` con sus bloques.
Los temporales no se liberan antes de finalizar `display()`: el pico de memoria
corresponde a la suma de sus imágenes. Un dibujo que nunca termina tampoco
alcanza el punto de limpieza. Para objetos persistentes o recolección durante
cálculos largos haría falta otro modelo de gestión de memoria.

### Cambios respecto al prototipo

- Se eliminó el registro de direcciones locales y los casts de punteros
  tipados a `void**`.
- Se reemplazaron `memoryAlloc`, `registerPointerToMemory`, `unregisterPointer`
  y el auxiliar `unlinkMemory` por `gcAlloc(size)` y limpieza global al terminar
  el dibujo. Son cambios en la interfaz interna de memoria; las firmas de
  composición de `chess.h` se mantienen.
- `reverse()` realmente invierte los colores y se implementan las demás
  operaciones antes solamente declaradas.
- Se comprueban los fallos de reserva, las dimensiones y los desbordamientos
  en el cálculo de los tamaños de salida; los terminadores quedan establecidos.
- El motor ajusta la proyección al tamaño real de la imagen y mantiene la
  proporción al cambiar el tamaño de la ventana.
- Se añadió un modo de salida PPM, documentación y pruebas independientes de GL.

## Pruebas

```sh
make test
make sanitize
```

`make sanitize` utiliza AddressSanitizer y UndefinedBehaviorSanitizer, y activa
la detección de fugas. En entornos donde LeakSanitizer no puede ejecutarse
(por ejemplo, algunos entornos supervisados):

```sh
make sanitize ASAN_OPTIONS=detect_leaks=0
```

Las pruebas verifican resultados concretos de todas las operaciones, giros de
imágenes rectangulares, transparencia, independencia de entradas y resultados,
argumentos inválidos, fallos simulados de las dos reservas del gestor, reservas
inicializadas a cero y 100 ciclos de construcción y limpieza del tablero.

### Verificación realizada en el entorno de entrega

- Compilación y ejecución de las pruebas y de ambos ejemplos sin ventana con GCC.
- AddressSanitizer y UndefinedBehaviorSanitizer sin errores en las pruebas.
- Contadores de bloques y bytes a cero después de cada uno de los 100 dibujos.
- Salida PPM del tablero comprobada píxel a píxel: 464 × 464, casillas de 58 × 58.
- LeakSanitizer no pudo ejecutarse por restricciones del entorno; se repitieron
  las pruebas con `detect_leaks=0`. No se afirma una validación con LeakSanitizer.
- La interfaz OpenGL/GLUT requiere verificación en un equipo con esas bibliotecas:
  no estaban disponibles en el entorno de entrega.
