#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "pathfinding.h"

/**
 * select_min - Selects the unvisited vertex with minimum distance
 * @vertices: Array of graph vertices
 * @distance: Array of distances
 * @visited: Array marking visited vertices
 * @count: Number of vertices
 * @start: Starting vertex name
 *
 * Return: Index of selected vertex, or count if none remains
 */
static size_t select_min(vertex_t **vertices, unsigned long *distance,
			 char *visited, size_t count, char const *start)
{
	size_t i, best = count;
	unsigned long minimum = ULONG_MAX;

	for (i = 0; i < count; i++)
		if (!visited[i] && distance[i] < minimum)
		{
			minimum = distance[i];
			best = i;
		}
	if (best != count)
	{
		visited[best] = 1;
		printf("Checking %s, distance from %s is %lu\n",
		       vertices[best]->content, start, distance[best]);
	}
	return (best);
}

/**
 * relax_edges - Updates distances through the current vertex
 * @vertices: Array of graph vertices
 * @distance: Array of distances
 * @previous: Array of predecessor indices
 * @visited: Array marking visited vertices
 * @count: Number of vertices
 * @current: Current vertex index
 */
static void relax_edges(vertex_t **vertices, unsigned long *distance,
			size_t *previous, char *visited, size_t count,
			size_t current)
{
	edge_t *edge;
	size_t next, i;
	unsigned long candidate;

	for (edge = vertices[current]->edges; edge; edge = edge->next)
	{
		for (next = count, i = 0; i < count; i++)
			if (vertices[i] == edge->dest)
			{
				next = i;
				break;
			}
		if (next == count || visited[next] || edge->weight < 0)
			continue;
		if ((unsigned long)edge->weight >
		    ULONG_MAX - distance[current])
			continue;
		candidate = distance[current] + (unsigned long)edge->weight;
		if (candidate < distance[next])
		{
			distance[next] = candidate;
			previous[next] = current;
		}
	}
}

/**
 * build_path - Creates a queue containing the shortest path
 * @vertices: Array of graph vertices
 * @previous: Array of predecessor indices
 * @count: Number of vertices
 * @start: Starting vertex index
 * @target: Target vertex index
 *
 * Return: Queue containing allocated vertex names, or NULL on failure
 */
static queue_t *build_path(vertex_t **vertices, size_t *previous,
			   size_t count, size_t start, size_t target)
{
	size_t *indices, length = 0, current = target, i;
	queue_t *path;
	char *name;

	indices = malloc(count * sizeof(*indices));
	if (!indices)
		return (NULL);
	while (length < count)
	{
		indices[length++] = current;
		if (current == start)
			break;
		current = previous[current];
		if (current == count)
			break;
	}
	if (indices[length - 1] != start)
		goto fail;
	path = queue_create();
	if (!path)
		goto fail;
	for (i = length; i > 0; i--)
	{
		name = strdup(vertices[indices[i - 1]]->content);
		if (!name || !queue_push_back(path, name))
		{
			free(name);
			while ((name = dequeue(path)) != NULL)
				free(name);
			queue_delete(path);
			goto fail;
		}
	}
	free(indices);
	return (path);

fail:
	free(indices);
	return (NULL);
}

/**
 * search_path - Finds the shortest path using Dijkstra's algorithm
 * @vertices: Array of graph vertices
 * @count: Number of vertices
 * @start: Starting vertex index
 * @target: Target vertex index
 * @start_name: Starting vertex name
 *
 * Return: Queue of allocated vertex names, or NULL if no path exists
 */
static queue_t *search_path(vertex_t **vertices, size_t count,
			    size_t start, size_t target,
			    char const *start_name)
{
	unsigned long *distance;
	char *visited;
	size_t *previous;
	size_t i, current;
	queue_t *path = NULL;

	distance = malloc(count * sizeof(*distance));
	visited = calloc(count, sizeof(*visited));
	previous = malloc(count * sizeof(*previous));
	if (!distance || !visited || !previous)
		goto cleanup;
	for (i = 0; i < count; i++)
	{
		distance[i] = ULONG_MAX;
		previous[i] = count;
	}
	distance[start] = 0;
	while ((current = select_min(vertices, distance, visited,
				     count, start_name)) != count)
	{
		if (current == target)
			break;
		relax_edges(vertices, distance, previous, visited,
			    count, current);
	}
	if (distance[target] != ULONG_MAX)
		path = build_path(vertices, previous, count, start, target);

cleanup:
	free(distance);
	free(visited);
	free(previous);
	return (path);
}

/**
 * dijkstra_graph - Finds the shortest path between two graph vertices
 * @graph: Graph to search
 * @start: Starting vertex
 * @target: Target vertex
 *
 * Return: Queue of allocated vertex names, or NULL if no path exists
 */
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	vertex_t **vertices, *vertex;
	size_t count, i, s = 0, t = 0;
	queue_t *path = NULL;
	int found_start = 0, found_target = 0;

	if (!graph || !start || !target || !graph->nb_vertices)
		return (NULL);
	count = graph->nb_vertices;
	vertices = malloc(count * sizeof(*vertices));
	if (!vertices)
		return (NULL);
	for (i = 0, vertex = graph->vertices; i < count && vertex;
	     i++, vertex = vertex->next)
	{
		vertices[i] = vertex;
		if (vertex == start)
		{
			s = i;
			found_start = 1;
		}
		if (vertex == target)
		{
			t = i;
			found_target = 1;
		}
	}
	if (i == count && found_start && found_target)
		path = search_path(vertices, count, s, t, start->content);
	free(vertices);
	return (path);
}
