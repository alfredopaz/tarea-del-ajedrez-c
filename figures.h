#ifndef FIGURES_H
#define FIGURES_H

/* Figuras base de 58 x 58 píxeles, terminadas en NULL.
 * Solo lectura por contrato: no modificar ni liberar estas figuras.
 * Se definen una sola vez en figures.c; pueden incluirse en varios módulos.
 */
extern char* whiteSquare[];
extern char* bishop[];
extern char* king[];
extern char* knight[];
extern char* pawn[];
extern char* queen[];
extern char* rook[];

#endif
