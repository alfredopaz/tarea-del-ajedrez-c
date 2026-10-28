#include "test_helpers.h"
#include "figures.h"

/* Integracion: armar un tablero con temporales nombrados y liberarlos TODOS. */
int main(void){
  char** dark = reverse(whiteSquare);
  assert(dark);
  char** pair = join(whiteSquare, dark);
  assert(pair);
  char** first = repeatH(pair, 4);
  assert(first);
  char** second = reverse(first);
  assert(second);
  char** twoRows = up(first, second);
  assert(twoRows);
  char** board = repeatV(twoRows, 4);
  assert(board);

  for(size_t i = 0; i < 464; i++){
    assert(board[i]);
    assert(strlen(board[i]) == 464);
    for(size_t j = 0; j < 464; j++)
      assert(board[i][j] == (((i / 58 + j / 58) % 2) ? '@' : '_'));
  }
  assert(board[464] == NULL);

  char** horse = superImpose(knight, dark);
  assert(horse);
  releaseImage(horse);
  releaseImage(board);
  releaseImage(twoRows);
  releaseImage(second);
  releaseImage(first);
  releaseImage(pair);
  releaseImage(dark);

  testPassed("composicion manual sin fugas");
  return 0;
}
