#include "test_helpers.h"

int main(void){
  char* input[] = {"abc", "def", NULL};
  const char* expected[] = {"cf", "be", "ad", NULL};
  char** result = rotateL(input);
  expectImage(result, expected);
  expectDistinct(result, input);
  releaseImage(result);

  char* square[] = {"ab", "cd", NULL};
  const char* squareExpected[] = {"bd", "ac", NULL};
  result = rotateL(square);
  expectImage(result, squareExpected);
  releaseImage(result);

  char* one[] = {"#", NULL};
  const char* same[] = {"#", NULL};
  result = rotateL(one);
  expectImage(result, same);
  releaseImage(result);

  char* ragged[] = {"ab", "c", NULL};
  expectNull(rotateL(NULL));
  expectNull(rotateL(ragged));
  assert(strcmp(input[0], "abc") == 0);

  testPassed("rotateL");
  return 0;
}
