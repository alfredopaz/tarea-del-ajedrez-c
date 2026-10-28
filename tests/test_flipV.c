#include "test_helpers.h"

int main(void){
  char* input[] = {"abc", "def", NULL};
  const char* expected[] = {"def", "abc", NULL};
  char** result = flipV(input);
  expectImage(result, expected);
  expectDistinct(result, input);
  releaseImage(result);

  char* one[] = {"#", NULL};
  const char* same[] = {"#", NULL};
  result = flipV(one);
  expectImage(result, same);
  releaseImage(result);

  char* ragged[] = {"ab", "c", NULL};
  expectNull(flipV(NULL));
  expectNull(flipV(ragged));
  assert(strcmp(input[1], "def") == 0);

  testPassed("flipV");
  return 0;
}
