/*
 * Simula fallos sucesivos de malloc() de cada operacion. Comprueba que
 * ninguna reserva parcial quede sin liberar. Exclusivo GCC + enlazador GNU.
 * Este archivo NO proporciona soluciones de las funciones de chess.h.
 */
#include "test_helpers.h"

#include <stddef.h>

void* __real_malloc(size_t size);
void __real_free(void* p);

static size_t calls;
static size_t failAt;
static size_t allocations;
static size_t releases;

void* __wrap_malloc(size_t size){
  calls++;
  if(calls == failAt)
    return NULL;
  void* p = __real_malloc(size);
  if(p)
    allocations++;
  return p;
}

void __wrap_free(void* p){
  if(p)
    releases++;
  __real_free(p);
}

static void reset(size_t fail){
  calls = 0;
  failAt = fail;
  allocations = 0;
  releases = 0;
}

static void check(size_t fail, const char* function){
  if(allocations != releases){
    fprintf(stderr, "%s: fuga con fallo en malloc numero %zu (%zu reservas, %zu liberaciones)\n",
            function, fail, allocations, releases);
    assert(0);
  }
}

static void unaryTest(const char* name, char** (*operation)(char**), char** fig){
  for(size_t fail = 1; fail <= 12; fail++){
    reset(fail);
    char** result = operation(fig);
    releaseImage(result);
    check(fail, name);
  }
}

static void binaryTest(const char* name, char** (*operation)(char**, char**),
                       char** a, char** b){
  for(size_t fail = 1; fail <= 12; fail++){
    reset(fail);
    char** result = operation(a, b);
    releaseImage(result);
    check(fail, name);
  }
}

static void repeatTest(const char* name, char** (*operation)(char**, int), char** fig){
  for(size_t fail = 1; fail <= 12; fail++){
    reset(fail);
    char** result = operation(fig, 3);
    releaseImage(result);
    check(fail, name);
  }
}

int main(void){
  char* a[] = {"ab", "cd", NULL};
  char* b[] = {"12", "34", NULL};
  unaryTest("reverse", reverse, a);
  unaryTest("flipH", flipH, a);
  unaryTest("flipV", flipV, a);
  unaryTest("rotateL", rotateL, a);
  unaryTest("rotateR", rotateR, a);
  binaryTest("join", join, a, b);
  binaryTest("up", up, a, b);
  binaryTest("superImpose", superImpose, a, b);
  repeatTest("repeatH", repeatH, a);
  repeatTest("repeatV", repeatV, a);

  testPassed("reservas fallidas: diez operaciones, sin fugas parciales");
  return 0;
}
