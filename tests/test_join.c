#include "test_helpers.h"

int main(void){
  char* left[] = {"ab", "cd", NULL};
  char* right[] = {"123", "456", NULL};
  const char* expected[] = {"ab123", "cd456", NULL};
  char** result = join(left, right);
  expectImage(result, expected);
  expectDistinct(result, left);
  expectDistinct(result, right);
  result[0][0] = 'X';
  assert(strcmp(left[0], "ab") == 0);
  releaseImage(result);

  char* a[] = {"_", NULL};
  char* b[] = {"@", NULL};
  const char* simple[] = {"_@", NULL};
  result = join(a, b);
  expectImage(result, simple);
  releaseImage(result);

  char* wrongHeight[] = {"x", NULL};
  char* ragged[] = {"ab", "c", NULL};
  expectNull(join(left, wrongHeight));
  expectNull(join(ragged, right));
  expectNull(join(NULL, right));
  expectNull(join(left, NULL));

  testPassed("join");
  return 0;
}
