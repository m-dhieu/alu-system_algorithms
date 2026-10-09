
#include <stdio.h>
#include <stdlib.h>
#include "pathfinding.h"

/**
 * search_path - Recursively searches for a path to the target
 * @map: Map to search
 * @rows: Number of rows
 * @cols: Number of columns
 * @current: Current coordinates
 * @target: Target coordinates
 * @visited: Visited-cell map
 * @path: Address of the temporary path
 *
 * Return: 1 if a path is found, otherwise 0
 */
static int search_path(char **map, int rows, int cols, point_t current,
		       point_t const *target, char **visited,
		       path_node_t **path)
{
	int dx[] = {1, 0, -1, 0};
	int dy[] = {0, 1, 0, -1};
	int i, x, y;
	path_node_t *node;
	point_t next;

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
	for (i = 0; i < 4; i++)
	{
		x = current.x + dx[i];
		y = current.y + dy[i];
		if (x < 0 || x >= cols || y < 0 || y >= rows)
			continue;
		if (map[y][x] != '0' || visited[y][x])
			continue;
		next.x = x;
		next.y = y;
		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	*path = node->next;
	free(node);
	return (0);
}

/**
 * create_visited - Allocates a visited-cell map
 * @rows: Number of rows
 * @cols: Number of columns
 *
 * Return: Allocated map, or NULL on failure
 */
static char **create_visited(int rows, int cols)
{
	char **visited;
	int i;

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
	return (visited);
}

/**
 * path_to_queue - Copies the temporary path into a queue
 * @path: Temporary path, stored target-first
 *
 * Return: Queue from start to target, or NULL on failure
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
			break;
		*point = node->point;
		if (!queue_push_front(queue, point))
		{
			free(point);
			break;
		}
	}
	if (node)
	{
		while ((point = dequeue(queue)) != NULL)
			free(point);
		queue_delete(queue);
		return (NULL);
	}
	return (queue);
}

/**
 * valid_points - Checks the starting and target coordinates
 * @map: Map to check
 * @rows: Number of rows
 * @cols: Number of columns
 * @start: Starting coordinates
 * @target: Target coordinates
 *
 * Return: 1 if both points are valid walkable cells, otherwise 0
 */
static int valid_points(char **map, int rows, int cols,
			point_t const *start, point_t const *target)
{
	if (!map || !start || !target || rows <= 0 || cols <= 0)
		return (0);
	if (start->x < 0 || start->x >= cols || start->y < 0 ||
	    start->y >= rows || target->x < 0 || target->x >= cols ||
	    target->y < 0 || target->y >= rows)
		return (0);
	return (map[start->y][start->x] == '0' &&
		map[target->y][target->x] == '0');
}

/**
 * backtracking_array - Finds a path through a two-dimensional array
 * @map: Map to search
 * @rows: Number of rows
 * @cols: Number of columns
 * @start: Starting coordinates
 * @target: Target coordinates
 *
 * Return: Queue containing the path, or NULL if no path is found
 */
queue_t *backtracking_array(char **map, int rows, int cols,
			    point_t const *start, point_t const *target)
{
	char **visited;
	path_node_t *path, *next;
	queue_t *queue;
	int i, found;

	if (!valid_points(map, rows, cols, start, target))
		return (NULL);
	visited = create_visited(rows, cols);
	if (!visited)
		return (NULL);
	path = NULL;
	found = search_path(map, rows, cols, *start, target, visited, &path);
	for (i = 0; i < rows; i++)
		free(visited[i]);
	free(visited);
	if (!found)
	{
		while (path)
		{
			next = path->next;
			free(path);
			path = next;
		}
		return (NULL);
	}
	queue = path_to_queue(path);
	while (path)
	{
		next = path->next;
		free(path);
		path = next;
	}
	return (queue);
}
