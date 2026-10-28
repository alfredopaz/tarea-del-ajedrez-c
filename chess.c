/*
 * SOLUCION DEL DOCENTE — SEGUNDA ETAPA
 *
 * Las diez operaciones de chess.h reservan sus propias imágenes con malloc().
 * Ninguna modifica ni libera sus argumentos; el llamador es responsable de
 * destruir los resultados. No se utiliza gcAlloc ni gcTrackImage.
 *
 * invertColor() y chessDimensions() se proporcionan en chess_internal.h.
 */
#include "chess.h"
#include "chess_internal.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Libera una imagen creada con newImage(), incluso si faltan filas por crear. */
static void destroyImage(char** fig){
  if(!fig)
    return;

  for(size_t i = 0; fig[i]; i++)
    free(fig[i]);
  free(fig);
}

/* Reserva rows filas de cols caracteres y los terminadores correspondientes.
 * Retorna NULL si el tamaño no es representable o falla una reserva.
 * El arreglo de punteros se inicializa a NULL para facilitar la limpieza.
 */
static char** newImage(size_t rows, size_t cols){
  if(rows == 0 || cols == 0)
    return NULL;
  if(rows > SIZE_MAX / sizeof(char*) - 1 || cols == SIZE_MAX)
    return NULL;

  char** fig = malloc((rows + 1) * sizeof(*fig));
  if(!fig)
    return NULL;

  for(size_t i = 0; i <= rows; i++)
    fig[i] = NULL;

  for(size_t i = 0; i < rows; i++){
    fig[i] = malloc(cols + 1);
    if(!fig[i]){
      destroyImage(fig);
      return NULL;
    }
    fig[i][cols] = '\0';
  }

  return fig;
}

char** reverse(char** fig){
  size_t rows, cols;
  if(!chessDimensions(fig, &rows, &cols))
    return NULL;

  char** result = newImage(rows, cols);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rows; i++)
    for(size_t j = 0; j < cols; j++)
      result[i][j] = invertColor(fig[i][j]);

  return result;
}

char** join(char** left, char** right){
  size_t rowsL, colsL, rowsR, colsR;
  if(!chessDimensions(left, &rowsL, &colsL) ||
     !chessDimensions(right, &rowsR, &colsR) || rowsL != rowsR)
    return NULL;
  if(colsL > SIZE_MAX - colsR)
    return NULL;

  char** result = newImage(rowsL, colsL + colsR);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rowsL; i++){
    memcpy(result[i], left[i], colsL);
    memcpy(result[i] + colsL, right[i], colsR);
  }
  return result;
}

char** repeatH(char** fig, int n){
  size_t rows, cols;
  if(n <= 0 || !chessDimensions(fig, &rows, &cols))
    return NULL;
  if(cols > SIZE_MAX / (size_t)n)
    return NULL;

  char** result = newImage(rows, cols * (size_t)n);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rows; i++)
    for(int k = 0; k < n; k++)
      memcpy(result[i] + (size_t)k * cols, fig[i], cols);

  return result;
}

char** up(char** top, char** bottom){
  size_t rowsT, colsT, rowsB, colsB;
  if(!chessDimensions(top, &rowsT, &colsT) ||
     !chessDimensions(bottom, &rowsB, &colsB) || colsT != colsB)
    return NULL;
  if(rowsT > SIZE_MAX - rowsB)
    return NULL;

  char** result = newImage(rowsT + rowsB, colsT);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rowsT; i++)
    memcpy(result[i], top[i], colsT);
  for(size_t i = 0; i < rowsB; i++)
    memcpy(result[rowsT + i], bottom[i], colsB);

  return result;
}

char** repeatV(char** fig, int n){
  size_t rows, cols;
  if(n <= 0 || !chessDimensions(fig, &rows, &cols))
    return NULL;
  if(rows > SIZE_MAX / (size_t)n)
    return NULL;

  char** result = newImage(rows * (size_t)n, cols);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rows * (size_t)n; i++)
    memcpy(result[i], fig[i % rows], cols);

  return result;
}

char** superImpose(char** front, char** back){
  size_t rowsF, colsF, rowsB, colsB;
  if(!chessDimensions(front, &rowsF, &colsF) ||
     !chessDimensions(back, &rowsB, &colsB) ||
     rowsF != rowsB || colsF != colsB)
    return NULL;

  char** result = newImage(rowsF, colsF);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rowsF; i++)
    for(size_t j = 0; j < colsF; j++)
      result[i][j] = (front[i][j] == ' ') ? back[i][j] : front[i][j];

  return result;
}

char** flipV(char** fig){
  size_t rows, cols;
  if(!chessDimensions(fig, &rows, &cols))
    return NULL;

  char** result = newImage(rows, cols);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rows; i++)
    memcpy(result[i], fig[rows - 1 - i], cols);

  return result;
}

char** flipH(char** fig){
  size_t rows, cols;
  if(!chessDimensions(fig, &rows, &cols))
    return NULL;

  char** result = newImage(rows, cols);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rows; i++)
    for(size_t j = 0; j < cols; j++)
      result[i][j] = fig[i][cols - 1 - j];

  return result;
}

char** rotateL(char** fig){
  size_t rows, cols;
  if(!chessDimensions(fig, &rows, &cols))
    return NULL;

  char** result = newImage(cols, rows);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rows; i++)
    for(size_t j = 0; j < cols; j++)
      result[cols - 1 - j][i] = fig[i][j];

  return result;
}

char** rotateR(char** fig){
  size_t rows, cols;
  if(!chessDimensions(fig, &rows, &cols))
    return NULL;

  char** result = newImage(cols, rows);
  if(!result)
    return NULL;

  for(size_t i = 0; i < rows; i++)
    for(size_t j = 0; j < cols; j++)
      result[j][rows - 1 - i] = fig[i][j];

  return result;
}
