#include "run.h"
	
#include <stdio.h>

int (*region)() = NULL;

runnable_t make_job(size_t size)
{
	return (runnable_t) {size, calloc(size, sizeof(unsigned char))};
}

void free_job(runnable_t job)
{
	free(job.executable);
}

int run(runnable_t runnable)
{
	if (region == NULL)
		region = mmap(NULL, 4096, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

	memcpy(region, runnable.executable, runnable.size);

	return region();
}

void free_page() 
{
	if (region)
		munmap(region, 4096);
}
