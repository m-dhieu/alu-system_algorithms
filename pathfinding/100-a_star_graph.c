#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "pathfinding.h"

/**
 * select_min - Selects the unvisited vertex with the lowest A* score
 * @vertices: Array of graph vertices
 * @distance: Actual distances from the starting vertex
 * @visited: Array marking visited vertices
 * @count: Number of vertices
 * @target: Target vertex
 *
 * Return: Index of selected vertex, or count if none remains
 */
static size_t select_min(vertex_t **vertices, unsigned long *distance,
			 char *visited, size_t count, vertex_t const *target)
{
	size_t i, best = count;
	double dx, dy, h, score, minimum = HUGE_VAL;

	for (i = 0; i < count; i++)
	{
		if (visited[i] || distance[i] == ULONG_MAX)
			continue;
		dx = vertices[i]->x - target->x;
		dy = vertices[i]->y - target->y;
		h = sqrt(dx * dx + dy * dy);
		score = distance[i] + h;
		if (score < minimum)
		{
			minimum = score;
			best = i;
		}
	}
	if (best != count)
	{
		visited[best] = 1;
		dx = vertices[best]->x - target->x;
		dy = vertices[best]->y - target->y;
		printf("Checking %s, distance to %s is %d\n",
		       vertices[best]->content, target->content,
		       (int)sqrt(dx * dx + dy * dy));
	}
	return (best);
}

/**
 * relax_edges - Updates distances through the current vertex
 * @vertices: Array of graph vertices
 * @distance: Actual distances from the starting vertex
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
	size_t i, next;
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
 * build_path - Creates a queue containing the path to the target
 * @vertices: Array of graph vertices
 * @previous: Array of predecessor indices
 * @count: Number of vertices
 * @start: Starting vertex index
 * @target: Target vertex index
 *
 * Return: Queue of allocated vertex names, or NULL on failure
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
 * search_path - Searches for the shortest path using A*
 * @vertices: Array of graph vertices
 * @distance: Array of actual distances
 * @visited: Array marking visited vertices
 * @previous: Array of predecessor indices
 * @count: Number of vertices
 * @start: Starting vertex index
 * @target: Target vertex index
 * @target_vertex: Target vertex
 *
 * Return: Queue containing the path, or NULL if no path exists
 */
static queue_t *search_path(vertex_t **vertices, unsigned long *distance,
			    char *visited, size_t *previous, size_t count,
			    size_t start, size_t target,
			    vertex_t const *target_vertex)
{
	size_t current;
	queue_t *path = NULL;

	distance[start] = 0;
	while ((current = select_min(vertices, distance, visited, count,
				     target_vertex)) != count)
	{
		if (current == target)
			break;
		relax_edges(vertices, distance, previous, visited, count,
			    current);
	}
	if (distance[target] != ULONG_MAX)
		path = build_path(vertices, previous, count, start, target);
	return (path);
}

/**
 * a_star_graph - Finds the shortest path using the A* algorithm
 * @graph: Graph to search
 * @start: Starting vertex
 * @target: Target vertex
 *
 * Return: Queue of allocated vertex names, or NULL on failure
 */
queue_t *a_star_graph(graph_t *graph, vertex_t const *start,
		      vertex_t const *target)
{
	vertex_t **vertices, *vertex;
	unsigned long *distance;
	size_t *previous, count, i, s = 0, t = 0;
	char *visited;
	queue_t *path = NULL;

	if (!graph || !start || !target || !graph->nb_vertices)
		return (NULL);
	count = graph->nb_vertices;
	vertices = malloc(count * sizeof(*vertices));
	distance = malloc(count * sizeof(*distance));
	visited = calloc(count, sizeof(*visited));
	previous = malloc(count * sizeof(*previous));
	if (!vertices || !distance || !visited || !previous)
		goto cleanup;
	for (i = 0, vertex = graph->vertices; i < count && vertex;
	     i++, vertex = vertex->next)
	{
		vertices[i] = vertex;
		distance[i] = ULONG_MAX;
		previous[i] = count;
		if (vertex == start)
			s = i;
		if (vertex == target)
			t = i;
	}
	if (i == count)
		path = search_path(vertices, distance, visited, previous,
				   count, s, t, target);

cleanup:
	free(vertices);
	free(distance);
	free(visited);
	free(previous);
	return (path);
}
