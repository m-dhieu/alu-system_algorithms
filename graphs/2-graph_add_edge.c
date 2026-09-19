#include "graphs.h"
#include <stdlib.h>
#include <string.h>

/**
 * connect_edge - Helper function to connect an edge from src to dest vertex
 * @src_v: Pointer to the source vertex
 * @dest_v: Pointer to the destination vertex
 *
 * Return: 1 on success, 0 on failure
 */
static int connect_edge(vertex_t *src_v, vertex_t *dest_v)
{
	edge_t *new_edge, *tail;

	new_edge = malloc(sizeof(edge_t));
	if (!new_edge)
		return (0);

	new_edge->dest = dest_v;
	new_edge->next = NULL;

	if (!src_v->edges)
	{
		src_v->edges = new_edge;
	}
	else
	{
		tail = src_v->edges;
		while (tail->next)
			tail = tail->next;
		tail->next = new_edge;
	}

	src_v->nb_edges++;
	return (1);
}

/**
 * graph_add_edge - Adds an edge between two vertices in an existing graph
 * @graph: Pointer to the graph
 * @src: String identifying the source vertex
 * @dest: String identifying the destination vertex
 * @type: Type of edge (UNIDIRECTIONAL or BIDIRECTIONAL)
 *
 * Return: 1 on success, 0 on failure
 */
int graph_add_edge(graph_t *graph, const char *src, const char *dest,
		   edge_type_t type)
{
	vertex_t *v, *src_v = NULL, *dest_v = NULL;

	if (!graph || !src || !dest)
		return (0);

	if (type != UNIDIRECTIONAL && type != BIDIRECTIONAL)
		return (0);

	v = graph->vertices;
	while (v)
	{
		if (strcmp(v->content, src) == 0)
			src_v = v;
		if (strcmp(v->content, dest) == 0)
			dest_v = v;
		v = v->next;
	}

	if (!src_v || !dest_v)
		return (0);

	if (!connect_edge(src_v, dest_v))
		return (0);

	if (type == BIDIRECTIONAL)
	{
		if (!connect_edge(dest_v, src_v))
			return (0);
	}

	return (1);
}

