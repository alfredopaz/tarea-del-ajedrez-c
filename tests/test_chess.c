#include "chess.h"
#include "chess_internal.h"
#include "figures.h"
#include "gc.h"
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void equal(char** actual, char** expected){
  assert(actual);
  size_t i = 0;
  while(expected[i]){
    assert(actual[i]);
    assert(strcmp(actual[i], expected[i]) == 0);
    i++;
  }
  assert(actual[i] == NULL);
}

static void operations(void){
  char* a[] = {"abc", "def", NULL};
  char* b[] = {"12", "34", NULL};
  char* ej[] = {"abc12", "def34", NULL};
  char* eu[] = {"abc", "def", "abc", "def", NULL};
  char* eh[] = {"abcabcabc", "defdefdef", NULL};
  char* ev[] = {"abc", "def", "abc", "def", "abc", "def", NULL};
  char* efv[] = {"def", "abc", NULL};
  char* efh[] = {"cba", "fed", NULL};
  char* el[] = {"cf", "be", "ad", NULL};
  char* er[] = {"da", "eb", "fc", NULL};
  char* colors[] = {"_@.# =", NULL};
  char* inverted[] = {"@_#. =", NULL};
  char* front[] = {" . ", "# #", NULL};
  char* background[] = {"@@@", "___", NULL};
  char* overlaid[] = {"@.@", "#_#", NULL};
  equal(join(a, b), ej);
  equal(up(a, a), eu);
  equal(repeatH(a, 3), eh);
  equal(repeatV(a, 3), ev);
  equal(flipV(a), efv);
  equal(flipH(a), efh);
  equal(rotateL(a), el);
  equal(rotateR(a), er);
  equal(reverse(colors), inverted);
  equal(superImpose(front, background), overlaid);
  equal(reverse(reverse(colors)), colors);
  equal(rotateR(rotateL(a)), a);
  equal(rotateR(rotateR(rotateR(rotateR(a)))), a);
  equal(flipV(flipV(a)), a);
  equal(flipH(flipH(a)), a);
  char** copy = repeatH(a, 1);
  copy[0][0] = 'X';
  assert(a[0][0] == 'a');
  equal(repeatV(a, 1), a);
  char* one[] = {"#", NULL};
  equal(rotateL(one), one);
  equal(rotateR(one), one);
  garbageCollector();
  assert(countMemoryEntries() == 0);
  assert(gcBytesAllocated() == 0);
}

static void invalid(void){
  char* a[] = {"ab", NULL};
  char* b[] = {"a", "b", NULL};
  char* ragged[] = {"ab", "c", NULL};
  char* empty[] = {NULL};
  char* zero[] = {"", NULL};
  assert(!reverse(NULL) && errno == EINVAL);
  assert(!reverse(empty) && errno == EINVAL);
  assert(!reverse(zero) && errno == EINVAL);
  assert(!reverse(ragged) && errno == EINVAL);
  assert(!join(a, b) && errno == EINVAL);
  assert(!up(a, b) && errno == EINVAL);
  assert(!superImpose(a, b) && errno == EINVAL);
  assert(!repeatH(a, 0) && errno == EINVAL);
  assert(!repeatV(a, -1) && errno == EINVAL);
  assert(!join(a, NULL));
  assert(!up(NULL, a));
  assert(!repeatH(NULL, 1));
  assert(!repeatV(NULL, 1));
  assert(!superImpose(NULL, a));
  assert(!flipV(NULL));
  assert(!flipH(NULL));
  assert(!rotateL(NULL));
  assert(!rotateR(NULL));
  assert(!gcAlloc(0) && errno == ENOMEM);
  assert(countMemoryEntries() == 0);
  unsigned char* memory = gcAlloc(32);
  assert(memory && countMemoryEntries() == 1);
  for(size_t i = 0; i < 32; i++)
    assert(memory[i] == 0);
  assert(gcBytesAllocated() == 32);
  assert(!gcAlloc(SIZE_MAX) && errno == ENOMEM);
  assert(countMemoryEntries() == 1 && gcBytesAllocated() == 32);
  garbageCollector();
  garbageCollector();
  assert(countMemoryEntries() == 0);
}

static void drawings(void){
  char** figures[] = {whiteSquare, bishop, king, knight, pawn, queen, rook};
  for(size_t i = 0; i < sizeof figures / sizeof figures[0]; i++){
    size_t rows, cols;
    assert(chessDimensions(figures[i], &rows, &cols));
    assert(rows == 58 && cols == 58);
  }
  for(int frame = 0; frame < 100; frame++){
    char** row = repeatH(join(whiteSquare, reverse(whiteSquare)), 4);
    char** board = repeatV(up(row, reverse(row)), 4);
    size_t rows, cols;
    assert(chessDimensions(board, &rows, &cols));
    assert(rows == 464 && cols == 464);
    for(size_t i = 0; i < rows; i++)
      for(size_t j = 0; j < cols; j++)
        assert(board[i][j] == (((i / 58 + j / 58) % 2) ? '@' : '_'));
    assert(superImpose(reverse(knight), whiteSquare));
    assert(countMemoryEntries() == 8);
    garbageCollector();
    assert(countMemoryEntries() == 0);
    assert(gcBytesAllocated() == 0);
    assert(whiteSquare[0][0] == '_');
  }
}

int main(void){
  operations();
  invalid();
  drawings();
  puts("OK: API, errores, composición, independencia y limpieza de 100 dibujos.");
  return 0;
}
