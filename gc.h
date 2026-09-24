#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include <stddef.h>

/* Memoria temporal del dibujo actual; no hay rastreo de referencias.
 * gcAlloc devuelve memoria inicializada a cero o NULL (errno = ENOMEM).
 * garbageCollector libera TODOS los bloques, incluso los aún referenciados.
 * El motor la llama después de display(). No conservar imágenes entre dibujos.
 * Un solo hilo. No llamar a free/realloc sobre bloques gestionados.
 */
void* gcAlloc(size_t size);
void garbageCollector(void);
size_t countMemoryEntries(void);
size_t gcBytesAllocated(void);

#endif
