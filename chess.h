#ifndef CHESS_H
#define CHESS_H

/* Una imagen es un arreglo de cadenas de igual longitud, terminado en NULL.
 * Las cadenas terminan en '\0'. Debe haber al menos una fila y una columna.
 * Las funciones no modifican sus argumentos y siempre crean una imagen nueva.
 * Retornan NULL y fijan errno: EINVAL (argumento/dimensiones/repeticiones
 * inválidos), EOVERFLOW (tamaño no representable), ENOMEM (sin memoria).
 * Las entradas deben ser arreglos válidos con sus terminadores.
 *
 * Memoria: el motor libera TODAS las imágenes creadas por la API al retornar
 * display(). No usar free(), ni guardar resultados entre llamadas a display().
 * Las figuras estáticas de figures.h permanecen disponibles.
 */

/* Implemente esta función y llame a interpreter una sola vez. */
void display(void);

/* Intercambia '_' <-> '@', '.' <-> '#'; conserva espacios y '='. */
char** reverse(char** fig);
/* Une izquierda y derecha; exige igual número de filas. */
char** join(char** left, char** right);
/* Repite horizontalmente; n > 0. */
char** repeatH(char** fig, int n);
/* Coloca top encima de bottom; exige igual número de columnas. */
char** up(char** top, char** bottom);
/* Repite verticalmente; n > 0. */
char** repeatV(char** fig, int n);
/* Superpone front sobre back, de iguales dimensiones.
 * Solo el espacio ' ' de front es transparente. */
char** superImpose(char** front, char** back);
/* Invierte el orden de las filas (arriba/abajo). */
char** flipV(char** fig);
/* Invierte el orden de las columnas (izquierda/derecha). */
char** flipH(char** fig);
/* Giros de 90 grados; intercambian alto y ancho. */
char** rotateL(char** fig);
char** rotateR(char** fig);

/* Dibuja la imagen; el espacio sin fondo se muestra gris. */
void interpreter(char** fig);

#endif
