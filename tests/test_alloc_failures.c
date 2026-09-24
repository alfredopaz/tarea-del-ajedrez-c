/* Inyección local de fallos, sin modificar el asignador de producción. */
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
static int failMetadata = 0;
static int failPayload = 0;
static void* testMalloc(size_t size){
  return failMetadata ? NULL : malloc(size);
}
static void* testCalloc(size_t count, size_t size){
  return failPayload ? NULL : calloc(count, size);
}
#define malloc testMalloc
#define calloc testCalloc
#include "../gc.c"
#undef malloc
#undef calloc
int main(void){
  void* alive = gcAlloc(12);
  assert(alive);
  failMetadata = 1;
  assert(!gcAlloc(16) && errno == ENOMEM);
  assert(countMemoryEntries() == 1 && gcBytesAllocated() == 12);
  failMetadata = 0;
  failPayload = 1;
  assert(!gcAlloc(16) && errno == ENOMEM);
  assert(countMemoryEntries() == 1 && gcBytesAllocated() == 12);
  failPayload = 0;
  assert(gcAlloc(16));
  assert(countMemoryEntries() == 2 && gcBytesAllocated() == 28);
  garbageCollector();
  assert(countMemoryEntries() == 0 && gcBytesAllocated() == 0);
  puts("OK: fallos de reserva y recuperación sin pérdida de bloques previos.");
  return 0;
}
