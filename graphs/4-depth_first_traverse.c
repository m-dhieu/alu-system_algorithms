#include <stdlib.h>

#include "graphs.h"

/**
 * vertex_is_visited - Checks whether a vertex has already been visited
 *
 * @visited: Array of visited vertices
 * @nb_vertices: Number of vertices in the graph
 * @vertex: Vertex to check
 *
 * Return: 1 if the vertex was visited, otherwise 0
 */
static int vertex_is_visited(vertex_t **visited, size_t nb_vertices,
			     vertex_t *vertex)
{
	size_t i;

	for (i = 0; i < nb_vertices; i++)
	{
		if (visited[i] == vertex)
			return (1);
	}

	return (0);
}

/**
 * depth_first_recursive - Recursively traverses a graph using DFS
 *
 * @vertex: Current vertex
 * @depth: Current vertex depth
 * @graph: Graph being traversed
 * @visited: Array of visited vertices
 * @visited_count: Number of visited vertices
 * @action: Function called for each visited vertex
 * @max_depth: Pointer to the greatest depth found
 */
static void depth_first_recursive(vertex_t *vertex, size_t depth,
				   const graph_t *graph, vertex_t **visited,
				   size_t *visited_count,
				   void (*action)(const vertex_t *v,
						  size_t depth),
				   size_t *max_depth)
{
	edge_t *edge;

	if (vertex == NULL || *visited_count >= graph->nb_vertices)
		return;

	if (vertex_is_visited(visited, graph->nb_vertices, vertex))
		return;

	visited[*visited_count] = vertex;
	*visited_count += 1;

	action(vertex, depth);

	if (depth > *max_depth)
		*max_depth = depth;

	edge = vertex->edges;
	while (edge != NULL)
	{
		depth_first_recursive(edge->dest, depth + 1, graph, visited,
				      visited_count, action, max_depth);
		edge = edge->next;
	}
}

/**
 * depth_first_traverse - Traverses a graph using depth-first traversal
 *
 * @graph: Pointer to the graph to traverse
 * @action: Function called for each visited vertex
 *
 * Return: Greatest vertex depth, or 0 on failure
 */
size_t depth_first_traverse(const graph_t *graph,
			    void (*action)(const vertex_t *v, size_t depth))
{
	vertex_t **visited;
	size_t visited_count = 0;
	size_t max_depth = 0;

	if (graph == NULL || graph->vertices == NULL || action == NULL)
		return (0);

	visited = malloc(sizeof(vertex_t *) * graph->nb_vertices);
	if (visited == NULL)
		return (0);

	depth_first_recursive(graph->vertices, 0, graph, visited,
			      &visited_count, action, &max_depth);

	free(visited);

	return (max_depth);
}

