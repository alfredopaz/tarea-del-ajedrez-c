#ifndef CHESS_INTERNAL_H
#define CHESS_INTERNAL_H
#include <stdbool.h>
#include <stddef.h>
/* Validación compartida por la API y el motor; no es parte de la API docente. */
bool chessDimensions(char** fig, size_t* rows, size_t* cols);
#endif
