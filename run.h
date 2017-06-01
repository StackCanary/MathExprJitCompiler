#ifndef RUN_H
#define RUN_H

#include <stddef.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

typedef struct {
	size_t size;
	unsigned char *executable;  
} runnable_t;


runnable_t make_job(size_t size);
void free_job(runnable_t job);
int run(runnable_t runnable);
void free_page();

#endif
