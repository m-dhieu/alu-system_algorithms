#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "pathfinding.h"

/**
 * struct dijkstra_data_s - Stores Dijkstra's algorithm data
 * @vertices: Array of graph vertices
 * @distance: Shortest known distances
 * @visited: Processed-vertex flags
 * @previous: Predecessor indices
 * @count: Number of vertices
 * @start: Starting vertex
 * @start_index: Starting vertex index
 */
typedef struct dijkstra_data_s
{
	vertex_t **vertices;
	unsigned long *distance;
	char *visited;
	size_t *previous;
	size_t count;
	vertex_t const *start;
	size_t start_index;
} dijkstra_data_t;

/**
 * vertex_index - Finds a vertex's index
 * @data: Dijkstra data
 * @vertex: Vertex to locate
 *
 * Return: Vertex index, or count if not found
 */
static size_t vertex_index(dijkstra_data_t *data, vertex_t const *vertex)
{
	size_t i;

	for (i = 0; i < data->count; i++)
	{
		if (data->vertices[i] == vertex)
			return (i);
	}
	return (data->count);
}

/**
 * select_min - Selects the unvisited vertex with minimum distance
 * @data: Dijkstra data
 *
 * Return: Selected index, or count if none remains
 */
static size_t select_min(dijkstra_data_t *data)
{
	size_t i, best;
	unsigned long minimum;

	best = data->count;
	minimum = ULONG_MAX;
	for (i = 0; i < data->count; i++)
	{
		if (!data->visited[i] && data->distance[i] < minimum)
		{
			minimum = data->distance[i];
			best = i;
		}
	}
	if (best != data->count)
	{
		data->visited[best] = 1;
		printf("Checking %s, distance from %s is %lu\n",
		       data->vertices[best]->content, data->start->content,
		       data->distance[best]);
	}
	return (best);
}

/**
 * relax_edges - Updates distances through a selected vertex
 * @data: Dijkstra data
 * @current: Current vertex index
 */
static void relax_edges(dijkstra_data_t *data, size_t current)
{
	edge_t *edge;
	size_t next;
	unsigned long candidate;

	for (edge = data->vertices[current]->edges; edge; edge = edge->next)
	{
		next = vertex_index(data, edge->dest);
		if (next == data->count || data->visited[next])
			continue;
		if (edge->weight < 0 ||
		    (unsigned long)edge->weight >
		    ULONG_MAX - data->distance[current])
			continue;
		candidate = data->distance[current] + (unsigned long)edge->weight;
		if (candidate < data->distance[next])
		{
			data->distance[next] = candidate;
			data->previous[next] = current;
		}
	}
}

/**
 * build_path - Creates a queue containing the shortest path
 * @data: Dijkstra data
 * @target: Target vertex index
 *
 * Return: Path queue, or NULL on failure
 */
static queue_t *build_path(dijkstra_data_t *data, size_t target)
{
	size_t *indices, current, length, i;
	queue_t *queue;
	char *name;

	indices = malloc(data->count * sizeof(*indices));
	if (!indices)
		return (NULL);
	current = target;
	length = 0;
	while (length < data->count)
	{
		indices[length++] = current;
		if (current == data->start_index)
			break;
		current = data->previous[current];
		if (current == data->count)
			break;
	}
	if (!length || indices[length - 1] != data->start_index)
	{
		free(indices);
		return (NULL);
	}
	queue = queue_create();
	if (!queue)
	{
		free(indices);
		return (NULL);
	}
	for (i = length; i > 0; i--)
	{
		name = malloc(strlen(data->vertices[indices[i - 1]]->content) + 1);
		if (!name)
			break;
		strcpy(name, data->vertices[indices[i - 1]]->content);
		if (!queue_push_back(queue, name))
		{
			free(name);
			break;
		}
	}
	free(indices);
	if (i == 0)
		return (queue);
	while ((name = dequeue(queue)) != NULL)
		free(name);
	queue_delete(queue);
	return (NULL);
}

/**
 * dijkstra_graph - Finds the shortest path between two graph vertices
 * @graph: Graph to search
 * @start: Starting vertex
 * @target: Target vertex
 *
 * Return: Queue of vertex names, or NULL if no path exists
 */
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	dijkstra_data_t data;
	vertex_t *vertex;
	size_t i, current, target_index;
	queue_t *path;

	if (!graph || !start || !target || !graph->nb_vertices)
		return (NULL);
	data.count = graph->nb_vertices;
	data.start = start;
	data.vertices = malloc(data.count * sizeof(*data.vertices));
	data.distance = malloc(data.count * sizeof(*data.distance));
	data.visited = calloc(data.count, sizeof(*data.visited));
	data.previous = malloc(data.count * sizeof(*data.previous));
	if (!data.vertices || !data.distance || !data.visited ||
	    !data.previous)
		goto cleanup;
	for (i = 0, vertex = graph->vertices; i < data.count && vertex;
	     i++, vertex = vertex->next)
	{
		data.vertices[i] = vertex;
		data.distance[i] = ULONG_MAX;
		data.previous[i] = data.count;
	}
	if (i != data.count)
		goto cleanup;
	data.start_index = vertex_index(&data, start);
	target_index = vertex_index(&data, target);
	if (data.start_index == data.count || target_index == data.count)
		goto cleanup;
	data.distance[data.start_index] = 0;
	while ((current = select_min(&data)) != data.count)
	{
		if (current == target_index)
			break;
		relax_edges(&data, current);
	}
	path = data.distance[target_index] == ULONG_MAX ?
		NULL : build_path(&data, target_index);
	free(data.vertices);
	free(data.distance);
	free(data.visited);
	free(data.previous);
	return (path);

cleanup:
	free(data.vertices);
	free(data.distance);
	free(data.visited);
	free(data.previous);
	return (NULL);
}
