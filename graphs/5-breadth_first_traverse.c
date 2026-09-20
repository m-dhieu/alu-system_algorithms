#include <stdlib.h>

#include "graphs.h"

/**
 * vertex_is_visited - Checks whether a vertex has already been visited
 *
 * @visited: Array of visited vertices
 * @visited_count: Number of visited vertices
 * @vertex: Vertex to check
 *
 * Return: 1 if the vertex was visited, otherwise 0
 */
static int vertex_is_visited(vertex_t **visited, size_t visited_count,
			     vertex_t *vertex)
{
	size_t i;

	for (i = 0; i < visited_count; i++)
	{
		if (visited[i] == vertex)
			return (1);
	}

	return (0);
}

/**
 * breadth_first_traverse - Traverses a graph using breadth-first traversal
 *
 * @graph: Pointer to the graph to traverse
 * @action: Function called for each visited vertex
 *
 * Return: Greatest vertex depth, or 0 on failure
 */
size_t breadth_first_traverse(const graph_t *graph,
			      void (*action)(const vertex_t *v,
					     size_t depth))
{
	vertex_t **queue;
	vertex_t **visited;
	size_t *depths;
	size_t queue_front = 0;
	size_t queue_rear = 0;
	size_t visited_count = 0;
	size_t max_depth = 0;
	vertex_t *vertex;
	edge_t *edge;

	if (graph == NULL || graph->vertices == NULL || action == NULL)
		return (0);

	queue = malloc(sizeof(vertex_t *) * graph->nb_vertices);
	visited = malloc(sizeof(vertex_t *) * graph->nb_vertices);
	depths = malloc(sizeof(size_t) * graph->nb_vertices);

	if (queue == NULL || visited == NULL || depths == NULL)
	{
		free(queue);
		free(visited);
		free(depths);
		return (0);
	}

	queue[queue_rear] = graph->vertices;
	depths[queue_rear] = 0;
	queue_rear++;

	visited[visited_count] = graph->vertices;
	visited_count++;

	while (queue_front < queue_rear)
	{
		vertex = queue[queue_front];
		action(vertex, depths[queue_front]);

		if (depths[queue_front] > max_depth)
			max_depth = depths[queue_front];

		edge = vertex->edges;
		while (edge != NULL)
		{
			if (!vertex_is_visited(visited, visited_count, edge->dest))
			{
				if (queue_rear >= graph->nb_vertices)
					break;

				visited[visited_count] = edge->dest;
				visited_count++;

				queue[queue_rear] = edge->dest;
				depths[queue_rear] = depths[queue_front] + 1;
				queue_rear++;
			}

			edge = edge->next;
		}

		queue_front++;
	}

	free(queue);
	free(visited);
	free(depths);

	return (max_depth);
}

