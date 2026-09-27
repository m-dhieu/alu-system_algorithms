#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

int int_cmp(void *p1, void *p2)
{
	int *n1, *n2;

	n1 = (int *)p1;
	n2 = (int *)p2;
	return (*n1 - *n2);
}

int main(void)
{
	heap_t *heap;

	heap = heap_create(int_cmp);
	if (heap == NULL)
	{
		fprintf(stderr, "Failed to create the heap\n");
		return (EXIT_FAILURE);
	}
	printf("Heap size: %lu\n", heap->size);
	printf("Heap root: %p\n", (void *)heap->root);
	return (EXIT_SUCCESS);
}

