#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pathfinding.h"

/**
 * is_visited - Checks whether a vertex name has been recorded
 * @visited: Array of visited vertex names
 * @count: Number of recorded vertex names
 * @name: Vertex name to check
 *
 * Return: 1 if already recorded, otherwise 0
 */
static int is_visited(char **visited, size_t count, const char *name)
{
	size_t i;

	for (i = 0; i < count; i++)
	{
		if (strcmp(visited[i], name) == 0)
			return (1);
	}
	return (0);
}

/**
 * visit_vertex - Records and prints a vertex if it is new
 * @visited: Address of the visited vertex array
 * @count: Address of the number of recorded names
 * @vertex: Vertex to record
 *
 * Return: 1 if recorded, 0 if already visited, or -1 on failure
 */
static int visit_vertex(char ***visited, size_t *count,
			const vertex_t *vertex)
{
	char **new_visited;

	if (is_visited(*visited, *count, vertex->content))
		return (0);

	new_visited = realloc(*visited, (*count + 1) * sizeof(**visited));
	if (!new_visited)
		return (-1);

	*visited = new_visited;
	(*visited)[*count] = vertex->content;
	(*count)++;
	printf("Checking %s\n", vertex->content);
	return (1);
}

/**
 * remove_path_back - Removes the last node from a path queue
 * @path: Queue representing the current path
 *
 * Return: Pointer to the removed data, or NULL if empty
 */
static void *remove_path_back(queue_t *path)
{
	queue_node_t *node;
	void *data;

	if (!path || !path->back)
		return (NULL);

	node = path->back;
	data = node->ptr;
	path->back = node->prev;

	if (path->back)
		path->back->next = NULL;
	else
		path->front = NULL;

	free(node);
	return (data);
}

/**
 * search_path - Recursively searches adjacent vertices
 * @visited: Address of the visited vertex array
 * @count: Address of the number of recorded names
 * @path: Queue holding the current path
 * @current: Vertex currently being explored
 * @target: Destination vertex
 *
 * Return: 1 if found, 0 if not found, or -1 on failure
 */
static int search_path(char ***visited, size_t *count, queue_t *path,
		       const vertex_t *current, const vertex_t *target)
{
	edge_t *edge;
	char *name;
	int result;

	result = visit_vertex(visited, count, current);
	if (result <= 0)
		return (result);

	name = strdup(current->content);
	if (!name)
		return (-1);

	if (!queue_push_back(path, name))
	{
		free(name);
		return (-1);
	}

	if (current == target)
		return (1);

	for (edge = current->edges; edge; edge = edge->next)
	{
		result = search_path(visited, count, path, edge->dest, target);
		if (result == 1)
			return (1);

		if (result == -1)
		{
			free(remove_path_back(path));
			return (-1);
		}
	}

	free(remove_path_back(path));
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
	char **visited;
	size_t count;
	queue_t *path;
	int result;

	if (!graph || !start || !target)
		return (NULL);

	path = queue_create();
	if (!path)
		return (NULL);

	visited = NULL;
	count = 0;

	result = search_path(&visited, &count, path, start, target);

	free(visited);

	if (result != 1)
	{
		queue_delete(path);
		return (NULL);
	}

	return (path);
}

