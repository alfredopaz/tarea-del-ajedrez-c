#include "test_helpers.h"

int main(void){
  char* input[] = {"ab", "cd", NULL};
  const char* one[] = {"ab", "cd", NULL};
  const char* three[] = {"ababab", "cdcdcd", NULL};
  char** result = repeatH(input, 1);
  expectImage(result, one);
  expectDistinct(result, input);
  releaseImage(result);

  result = repeatH(input, 3);
  expectImage(result, three);
  releaseImage(result);

  char* pixel[] = {"#", NULL};
  const char* twice[] = {"##", NULL};
  result = repeatH(pixel, 2);
  expectImage(result, twice);
  releaseImage(result);

  char* ragged[] = {"ab", "c", NULL};
  expectNull(repeatH(input, 0));
  expectNull(repeatH(input, -2));
  expectNull(repeatH(ragged, 2));
  expectNull(repeatH(NULL, 2));
  assert(strcmp(input[0], "ab") == 0);

  testPassed("repeatH");
  return 0;
}
