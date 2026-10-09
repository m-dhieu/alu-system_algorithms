#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pathfinding.h"

/**
 * struct search_s - State used by the recursive graph search
 * @visited: Names of vertices already explored
 * @count: Number of recorded vertex names
 * @path: Queue holding the path found so far
 */
typedef struct search_s
{
	char **visited;
	size_t count;
	queue_t *path;
} search_t;

/**
 * is_visited - Checks whether a vertex name has been recorded
 * @search: Search state
 * @name: Vertex name to check
 *
 * Return: 1 if already recorded, otherwise 0
 */
static int is_visited(search_t *search, const char *name)
{
	size_t i;

	for (i = 0; i < search->count; i++)
	{
		if (strcmp(search->visited[i], name) == 0)
			return (1);
	}
	return (0);
}

/**
 * visit_vertex - Records and prints a vertex if it is new
 * @search: Search state
 * @vertex: Vertex to record
 *
 * Return: 1 if recorded, 0 if already visited, or -1 on failure
 */
static int visit_vertex(search_t *search, const vertex_t *vertex)
{
	char **new_visited;

	if (is_visited(search, vertex->content))
		return (0);

	new_visited = realloc(search->visited,
			      (search->count + 1) * sizeof(*search->visited));
	if (!new_visited)
		return (-1);

	search->visited = new_visited;
	search->visited[search->count] = vertex->content;
	search->count++;
	printf("Checking %s\n", vertex->content);
	return (1);
}

/**
 * search_path - Recursively searches adjacent vertices
 * @search: Search state
 * @current: Vertex currently being explored
 * @target: Destination vertex
 *
 * Return: 1 if a path was found, 0 if not found, or -1 on failure
 */
static int search_path(search_t *search, const vertex_t *current,
		       const vertex_t *target)
{
	edge_t *edge;
	char *name;
	int result;

	result = visit_vertex(search, current);
	if (result <= 0)
		return (result);

	name = strdup(current->content);
	if (!name)
		return (-1);

	if (!queue_push_back(search->path, name))
	{
		free(name);
		return (-1);
	}

	if (current == target)
		return (1);

	for (edge = current->edges; edge; edge = edge->next)
	{
		result = search_path(search, edge->dest, target);
		if (result == 1)
			return (1);
		if (result == -1)
		{
			free(dequeue(search->path));
			return (-1);
		}
	}

	free(dequeue(search->path));
	return (0);
}

/**
 * backtracking_graph - Finds the first path between two graph vertices
 * @graph: Graph to search
 * @start: Starting vertex
 * @target: Target vertex
 *
 * Return: Queue containing the path, or NULL if no path is found
 */
queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
			    vertex_t const *target)
{
	search_t search;
	int result;

	if (!graph || !start || !target)
		return (NULL);

	search.path = queue_create();
	if (!search.path)
		return (NULL);

	search.visited = NULL;
	search.count = 0;

	result = search_path(&search, start, target);

	free(search.visited);

	if (result != 1)
	{
		queue_delete(search.path);
		return (NULL);
	}

	return (search.path);
}

