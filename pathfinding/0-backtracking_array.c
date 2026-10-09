#include <stdio.h>
#include <stdlib.h>
#include "pathfinding.h"

/**
 * search_path - Recursively searches for a route to the target
 * @map: The map to search
 * @rows: Number of rows in the map
 * @cols: Number of columns in the map
 * @current: Current coordinates
 * @target: Target coordinates
 * @visited: Marks cells already explored
 * @path: Queue holding the current route
 *
 * Return: 1 if a route is found, otherwise 0
 */
static int search_path(char **map, int rows, int cols, point_t current,
		       point_t const *target, char **visited, queue_t *path)
{
	point_t *point;

	printf("Checking coordinates [%d, %d]\n", current.x, current.y);
	visited[current.y][current.x] = 1;
	point = malloc(sizeof(*point));
	if (!point)
		return (0);
	*point = current;
	enqueue(path, point);
	if (current.x == target->x && current.y == target->y)
		return (1);
	if (current.x + 1 < cols &&
	    map[current.y][current.x + 1] == '0' &&
	    !visited[current.y][current.x + 1])
	{
		point_t next = {current.x + 1, current.y};

		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	if (current.y + 1 < rows &&
	    map[current.y + 1][current.x] == '0' &&
	    !visited[current.y + 1][current.x])
	{
		point_t next = {current.x, current.y + 1};

		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	if (current.x > 0 && map[current.y][current.x - 1] == '0' &&
	    !visited[current.y][current.x - 1])
	{
		point_t next = {current.x - 1, current.y};

		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	if (current.y > 0 && map[current.y - 1][current.x] == '0' &&
	    !visited[current.y - 1][current.x])
	{
		point_t next = {current.x, current.y - 1};

		if (search_path(map, rows, cols, next, target, visited, path))
			return (1);
	}
	free(dequeue_back(path));
	return (0);
}

/**
 * free_queue_points - Frees all points in a queue
 * @path: Queue whose points should be freed
 */
static void free_queue_points(queue_t *path)
{
	point_t *point;

	while (path->front)
	{
		point = dequeue(path);
		free(point);
	}
	free(path);
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
 * backtracking_array - Finds a path through a two-dimensional array
 * @map: The map to search
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
	queue_t *path;
	int i;

	if (!map || !start || !target || rows <= 0 || cols <= 0)
		return (NULL);
	if (start->x < 0 || start->x >= cols || start->y < 0 ||
	    start->y >= rows || target->x < 0 || target->x >= cols ||
	    target->y < 0 || target->y >= rows)
		return (NULL);
	if (map[start->y][start->x] != '0' || map[target->y][target->x] != '0')
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
	path = malloc(sizeof(*path));
	if (!path)
	{
		free_visited(visited, rows);
		return (NULL);
	}
	queue_init(path);
	if (!search_path(map, rows, cols, *start, target, visited, path))
	{
		free(path);
		path = NULL;
	}
	free_visited(visited, rows);
	return (path);
}

