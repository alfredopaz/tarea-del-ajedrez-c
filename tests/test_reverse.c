#include "test_helpers.h"

int main(void){
  char* input[] = {"_@.# =x", "_ #@   ", NULL};
  const char* expected[] = {"@_#. =x", "@ ._   ", NULL};
  char** result = reverse(input);
  expectImage(result, expected);
  expectDistinct(result, input);
  assert(strcmp(input[0], "_@.# =x") == 0);

  char** second = reverse(result);
  const char* original[] = {"_@.# =x", "_ #@   ", NULL};
  expectImage(second, original);
  expectDistinct(second, result);
  releaseImage(second);
  releaseImage(result);

  char* one[] = {" ", NULL};
  const char* space[] = {" ", NULL};
  result = reverse(one);
  expectImage(result, space);
  releaseImage(result);

  char* ragged[] = {"ab", "c", NULL};
  char* empty[] = {NULL};
  char* zero[] = {"", NULL};
  expectNull(reverse(NULL));
  expectNull(reverse(ragged));
  expectNull(reverse(empty));
  expectNull(reverse(zero));

  testPassed("reverse");
  return 0;
}
