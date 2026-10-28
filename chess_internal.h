#ifndef CHESS_INTERNAL_H
#define CHESS_INTERNAL_H

/* Auxiliares proporcionados por el docente: NO son ejercicios de la etapa II.
 * chessDimensions() verifica una imagen y obtiene su alto y ancho.
 * invertColor() contiene la correspondencia de colores utilizada por reverse().
 */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static inline bool chessDimensions(char** fig, size_t* rows, size_t* cols){
  if(!fig || !rows || !cols || !fig[0])
    return false;

  size_t width = strlen(fig[0]);
  if(width == 0)
    return false;

  size_t height = 0;
  while(fig[height]){
    if(strlen(fig[height]) != width)
      return false;
    height++;
  }

  *rows = height;
  *cols = width;
  return true;
}

static inline char invertColor(char c){
  switch(c){
    case '_': return '@';
    case '@': return '_';
    case '.': return '#';
    case '#': return '.';
    default: return c;
  }
}

#endif
