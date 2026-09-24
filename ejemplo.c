#include "chess.h"
#include "figures.h"

void display(void){
  char** blackSquare = reverse(whiteSquare);
  char** pair = join(whiteSquare, blackSquare);
  char** row = repeatH(pair, 4);
  char** board = repeatV(up(row, reverse(row)), 4);
  interpreter(board);
}
