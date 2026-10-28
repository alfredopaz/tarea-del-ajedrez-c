#include "test_helpers.h"

int main(void){
  char* top[] = {"ab", "cd", NULL};
  char* bottom[] = {"12", NULL};
  const char* expected[] = {"ab", "cd", "12", NULL};
  char** result = up(top, bottom);
  expectImage(result, expected);
  expectDistinct(result, top);
  expectDistinct(result, bottom);
  result[2][0] = 'X';
  assert(strcmp(bottom[0], "12") == 0);
  releaseImage(result);

  char* one[] = {"#", NULL};
  const char* same[] = {"#", "#", NULL};
  result = up(one, one);
  expectImage(result, same);
  releaseImage(result);

  char* wrongWidth[] = {"abc", NULL};
  char* ragged[] = {"ab", "c", NULL};
  expectNull(up(top, wrongWidth));
  expectNull(up(ragged, bottom));
  expectNull(up(NULL, bottom));
  expectNull(up(top, NULL));

  testPassed("up");
  return 0;
}
