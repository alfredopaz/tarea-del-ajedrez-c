#include "chess.h"
#include "figures.h"

void display(void){
  char** blackKnight = reverse(knight);
  interpreter(superImpose(blackKnight, whiteSquare));
}
