#include "test_helpers.h"

int main(void){
  char* input[] = {"abc", "def", NULL};
  const char* expected[] = {"cba", "fed", NULL};
  char** result = flipH(input);
  expectImage(result, expected);
  expectDistinct(result, input);
  releaseImage(result);

  char* one[] = {"#", NULL};
  const char* same[] = {"#", NULL};
  result = flipH(one);
  expectImage(result, same);
  releaseImage(result);

  char* ragged[] = {"ab", "c", NULL};
  expectNull(flipH(NULL));
  expectNull(flipH(ragged));
  assert(strcmp(input[0], "abc") == 0);

  testPassed("flipH");
  return 0;
}
