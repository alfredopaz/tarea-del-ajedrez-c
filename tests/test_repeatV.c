#include "test_helpers.h"

int main(void){
  char* input[] = {"ab", "cd", NULL};
  const char* one[] = {"ab", "cd", NULL};
  const char* three[] = {"ab", "cd", "ab", "cd", "ab", "cd", NULL};
  char** result = repeatV(input, 1);
  expectImage(result, one);
  expectDistinct(result, input);
  releaseImage(result);

  result = repeatV(input, 3);
  expectImage(result, three);
  releaseImage(result);

  char* pixel[] = {"#", NULL};
  const char* twice[] = {"#", "#", NULL};
  result = repeatV(pixel, 2);
  expectImage(result, twice);
  releaseImage(result);

  char* ragged[] = {"ab", "c", NULL};
  expectNull(repeatV(input, 0));
  expectNull(repeatV(input, -2));
  expectNull(repeatV(ragged, 2));
  expectNull(repeatV(NULL, 2));
  assert(strcmp(input[1], "cd") == 0);

  testPassed("repeatV");
  return 0;
}
