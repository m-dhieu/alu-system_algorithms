#include <stdlib.h>

#include "graphs.h"

/**
 * graph_delete - Deletes a graph and all its vertices and edges
 *
 * @graph: Pointer to the graph to delete
 */
void graph_delete(graph_t *graph)
{
	vertex_t *vertex;
	vertex_t *next_vertex;
	edge_t *edge;
	edge_t *next_edge;

	if (graph == NULL)
		return;

	vertex = graph->vertices;
	while (vertex != NULL)
	{
		next_vertex = vertex->next;
		edge = vertex->edges;

		while (edge != NULL)
		{
			next_edge = edge->next;
			free(edge);
			edge = next_edge;
		}

		free(vertex->content);
		free(vertex);
		vertex = next_vertex;
	}

	free(graph);
}

