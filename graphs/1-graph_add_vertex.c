#include "graphs.h"
#include <stdlib.h>
#include <string.h>

/**
 * graph_add_vertex - Adds a vertex to an existing graph.
 * @graph: Pointer to the graph structure to append to.
 * @str: String content to store inside the newly created vertex.
 *
 * Return: A pointer to the created vertex, or NULL on allocation or
 *         duplication failure. Returns NULL if a duplicate string is found.
 */
vertex_t *graph_add_vertex(graph_t *graph, const char *str)
{
	vertex_t *new_vertex, *tail;

	if (!graph || !str)
		return (NULL);

	tail = graph->vertices;
	while (tail)
	{
		if (strcmp(tail->content, str) == 0)
			return (NULL);
		if (!tail->next)
			break;
		tail = tail->next;
	}

	new_vertex = malloc(sizeof(vertex_t));
	if (!new_vertex)
		return (NULL);

	new_vertex->content = strdup(str);
	if (!new_vertex->content)
	{
		free(new_vertex);
		return (NULL);
	}

	new_vertex->index = graph->nb_vertices;
	new_vertex->nb_edges = 0;
	new_vertex->edges = NULL;
	new_vertex->next = NULL;

	if (!graph->vertices)
		graph->vertices = new_vertex;
	else
		tail->next = new_vertex;

	graph->nb_vertices++;

	return (new_vertex);
}

