#include <stdio.h>
#include <stdlib.h>
#include "pathfinding.h"

/**
 * struct path_node_s - Node in a temporary path
 * @point: Coordinates stored in the node
 * @next: Pointer to the next node
 */
typedef struct path_node_s
{
	point_t point;
	struct path_node_s *next;
} path_node_t;

/**
 * free_path - Frees a temporary path
 * @path: Path to free
 */
static void free_path(path_node_t *path)
{
	path_node_t *next;

	while (path)
	{
		next = path->next;
		free(path);
		path = next;
	}
}

/**
 * search_path - Recursively searches for a route to the target
 * @map: Map to search
 * @rows: Number of rows in the map
 * @cols: Number of columns in the map
 * @current: Current coordinates
 * @target: Target coordinates
 * @visited: Marks cells already explored
 * @path: Head of the temporary path
 *
 * Return: 1 if a route is found, otherwise 0
 */
static int search_path(char **map, int rows, int cols, point_t current,
		       point_t const *target, char **visited,
		       path_node_t **path)
{
	point_t next;
	path_node_t *node;

	printf("Checking coordinates [%d, %d]\n", current.x, current.y);
	visited[current.y][current.x] = 1;
	node = malloc(sizeof(*node));
	if (!node)
		return (0);
	node->point = current;
	node->next = *path;
	*path = node;
	if (current.x == target->x && current.y == target->y)
		return (1);
	if (current.x + 1 < cols &&
	    map[current.y][current.x + 1] == '0' &&
	    !visited[current.y][current.x + 1])
	{
		next.x = current.x + 1;
		next.y = current.y;
		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	if (current.y + 1 < rows &&
	    map[current.y + 1][current.x] == '0' &&
	    !visited[current.y + 1][current.x])
	{
		next.x = current.x;
		next.y = current.y + 1;
		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	if (current.x > 0 && map[current.y][current.x - 1] == '0' &&
	    !visited[current.y][current.x - 1])
	{
		next.x = current.x - 1;
		next.y = current.y;
		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	if (current.y > 0 && map[current.y - 1][current.x] == '0' &&
	    !visited[current.y - 1][current.x])
	{
		next.x = current.x;
		next.y = current.y - 1;
		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	*path = node->next;
	free(node);
	return (0);
}

/**
 * free_visited - Frees the visited-cell map
 * @visited: Visited-cell map
 * @rows: Number of rows
 */
static void free_visited(char **visited, int rows)
{
	int i;

	for (i = 0; i < rows; i++)
		free(visited[i]);
	free(visited);
}

/**
 * path_to_queue - Copies the temporary path into a queue
 * @path: Temporary path, stored target-first
 *
 * Return: Queue containing the path from start to target, or NULL
 */
static queue_t *path_to_queue(path_node_t *path)
{
	queue_t *queue;
	path_node_t *node;
	point_t *point;

	queue = queue_create();
	if (!queue)
		return (NULL);
	for (node = path; node; node = node->next)
	{
		point = malloc(sizeof(*point));
		if (!point)
		{
			queue_delete(queue);
			return (NULL);
		}
		*point = node->point;
		if (!queue_push_front(queue, point))
		{
			free(point);
			queue_delete(queue);
			return (NULL);
		}
	}
	return (queue);
}

/**
 * backtracking_array - Finds a path through a two-dimensional array
 * @map: Map to search
 * @rows: Number of rows in the map
 * @cols: Number of columns in the map
 * @start: Starting coordinates
 * @target: Target coordinates
 *
 * Return: Queue containing the path, or NULL if no path is found
 */
queue_t *backtracking_array(char **map, int rows, int cols,
			    point_t const *start, point_t const *target)
{
	char **visited;
	path_node_t *path;
	queue_t *queue;
	int i;

	if (!map || !start || !target || rows <= 0 || cols <= 0)
		return (NULL);
	if (start->x < 0 || start->x >= cols || start->y < 0 ||
	    start->y >= rows || target->x < 0 || target->x >= cols ||
	    target->y < 0 || target->y >= rows)
		return (NULL);
	if (map[start->y][start->x] != '0' ||
	    map[target->y][target->x] != '0')
		return (NULL);
	visited = malloc(rows * sizeof(*visited));
	if (!visited)
		return (NULL);
	for (i = 0; i < rows; i++)
	{
		visited[i] = calloc(cols, sizeof(**visited));
		if (!visited[i])
		{
			while (i > 0)
				free(visited[--i]);
			free(visited);
			return (NULL);
		}
	}
	path = NULL;
	if (!search_path(map, rows, cols, *start, target, visited, &path))
	{
		free_visited(visited, rows);
		return (NULL);
	}
	queue = path_to_queue(path);
	free_path(path);
	free_visited(visited, rows);
	return (queue);
}

