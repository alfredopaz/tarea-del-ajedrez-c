#ifndef CHESS_H
#define CHESS_H

/* Una imagen es un arreglo de cadenas de igual longitud, terminado en NULL.
 * Cada fila termina en '\0'; debe existir al menos una fila y una columna.
 * Todas las operaciones devuelven una imagen NUEVA y no modifican sus
 * argumentos. Devuelven NULL si los argumentos/dimensiones no son válidos,
 * si hay desbordamiento de tamaños o si falla una reserva de memoria.
 * No es necesario usar errno.
 *
 * Tarea 1: chess.o proporciona las operaciones. El motor libera sus resultados.
 * Tarea 2: chess.c es implementado por el estudiante con malloc() y free().
 *          El llamador debe liberar cada fila devuelta y finalmente el arreglo
 *          de punteros. La biblioteca NO utiliza recolector de basura.
 */

void display(void);

/* Cambia '_'<->'@' y '.'<->'#'; conserva espacios, '=' y otros caracteres. */
char** reverse(char** fig);
/* Une left a la izquierda de right; requiere igual número de filas. */
char** join(char** left, char** right);
/* Repite la figura n veces horizontalmente; n > 0. */
char** repeatH(char** fig, int n);
/* Coloca top encima de bottom; requiere igual número de columnas. */
char** up(char** top, char** bottom);
/* Repite la figura n veces verticalmente; n > 0. */
char** repeatV(char** fig, int n);
/* Coloca front sobre back; ' ' es el único carácter transparente de front.
 * Las dos figuras deben tener dimensiones idénticas. */
char** superImpose(char** front, char** back);
/* Intercambia arriba y abajo. */
char** flipV(char** fig);
/* Intercambia izquierda y derecha. */
char** flipH(char** fig);
/* Giros de 90 grados, que intercambian alto y ancho. */
char** rotateL(char** fig);
char** rotateR(char** fig);

void interpreter(char** fig);

#endif
