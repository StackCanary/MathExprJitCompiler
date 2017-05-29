#ifndef RUN_H
#define RUN_H

#include <stddef.h>
#include <string.h>
#include <sys/mman.h>

typedef struct {
	size_t size;
	unsigned char *executable;  
} runnable_t;

int run(runnable_t *runnable);
void free_page();

#endif
