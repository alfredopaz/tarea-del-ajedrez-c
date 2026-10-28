#include "test_helpers.h"

int main(void){
  char* front[] = {" . ", "# #", NULL};
  char* back[] = {"@@@", "___", NULL};
  const char* expected[] = {"@.@", "#_#", NULL};
  char** result = superImpose(front, back);
  expectImage(result, expected);
  expectDistinct(result, front);
  expectDistinct(result, back);
  assert(strcmp(front[0], " . ") == 0);
  assert(strcmp(back[1], "___") == 0);
  releaseImage(result);

  char* opaque[] = {"= ", NULL};
  char* background[] = {"#@", NULL};
  const char* withOtherChars[] = {"=@", NULL};
  result = superImpose(opaque, background);
  expectImage(result, withOtherChars);
  releaseImage(result);

  char* wrongWidth[] = {"@@", "__", NULL};
  char* wrongHeight[] = {"@@@", NULL};
  expectNull(superImpose(front, wrongWidth));
  expectNull(superImpose(front, wrongHeight));
  expectNull(superImpose(NULL, back));
  expectNull(superImpose(front, NULL));

  testPassed("superImpose");
  return 0;
}
