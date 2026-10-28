#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "chess.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Los datos de entrada son estaticos: NUNCA se liberan con free(). */
static inline void expectImage(char** got, const char* const expected[]){
  assert(got);
  size_t i = 0;
  while(expected[i]){
    assert(got[i]);
    if(strcmp(got[i], expected[i]) != 0){
      fprintf(stderr, "Fila %zu: obtenido <%s>, esperado <%s>\n",
              i, got[i], expected[i]);
      assert(0 && "Imagen incorrecta");
    }
    i++;
  }
  assert(got[i] == NULL);
}

/* Comprobacion del contrato: memoria independiente, tambien por fila. */
static inline void expectDistinct(char** got, char** original){
  assert(got && original && got != original);
  for(size_t i = 0; original[i]; i++)
    for(size_t j = 0; got[j]; j++)
      assert(got[j] != original[i]);
}

/* Auxiliar EXCLUSIVO de las pruebas: no forma parte de la API chess.h. */
static inline void releaseImage(char** fig){
  if(!fig)
    return;
  for(size_t i = 0; fig[i]; i++)
    free(fig[i]);
  free(fig);
}

static inline void expectNull(char** fig){
  if(fig){
    releaseImage(fig);
    assert(0 && "Se esperaba NULL");
  }
}

static inline void testPassed(const char* name){
  printf("OK: %s\n", name);
}

#endif
