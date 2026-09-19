#include "graphs.h"
#include <stdlib.h>

/**
 * graph_create - Allocates memory for and initializes a graph_t structure
 *
 * Return: A pointer to the newly allocated graph_t structure,
 *         or NULL if the allocation fails
 */
graph_t *graph_create(void)
{
	graph_t *graph;

	graph = malloc(sizeof(graph_t));
	if (graph == NULL)
		return (NULL);

	graph->nb_vertices = 0;
	graph->vertices = NULL;

	return (graph);
}

