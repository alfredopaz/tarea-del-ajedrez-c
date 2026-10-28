#include "test_helpers.h"

int main(void){
  char* input[] = {"abc", "def", NULL};
  const char* expected[] = {"da", "eb", "fc", NULL};
  char** result = rotateR(input);
  expectImage(result, expected);
  expectDistinct(result, input);
  releaseImage(result);

  char* square[] = {"ab", "cd", NULL};
  const char* squareExpected[] = {"ca", "db", NULL};
  result = rotateR(square);
  expectImage(result, squareExpected);
  releaseImage(result);

  char* one[] = {"#", NULL};
  const char* same[] = {"#", NULL};
  result = rotateR(one);
  expectImage(result, same);
  releaseImage(result);

  char* ragged[] = {"ab", "c", NULL};
  expectNull(rotateR(NULL));
  expectNull(rotateR(ragged));
  assert(strcmp(input[0], "abc") == 0);

  testPassed("rotateR");
  return 0;
}
