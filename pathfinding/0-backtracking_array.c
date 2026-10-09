#include <stdio.h>
#include <stdlib.h>
#include "pathfinding.h"

static int search_path(char **map, int rows, int cols, point_t current,
		       point_t const *target, char **visited,
		       path_node_t **path);

/**
 * free_path - Frees a temporary path
 * @path: Temporary path to free
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
 * add_path_node - Adds a point to the temporary path
 * @path: Address of the temporary path
 * @current: Point to store
 *
 * Return: New path node, or NULL on failure
 */
static path_node_t *add_path_node(path_node_t **path, point_t current)
{
	path_node_t *node;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->point = current;
	node->next = *path;
	*path = node;
	return (node);
}

/**
 * search_right - Recursively checks the right neighbour
 * @map: Map to search
 * @rows: Number of rows
 * @cols: Number of columns
 * @current: Current coordinates
 * @target: Target coordinates
 * @visited: Visited-cell map
 * @path: Address of the temporary path
 *
 * Return: 1 if a route is found, otherwise 0
 */
static int search_right(char **map, int rows, int cols, point_t current,
			point_t const *target, char **visited,
			path_node_t **path)
{
	point_t next;

	if (current.x + 1 >= cols || map[current.y][current.x + 1] != '0' ||
	    visited[current.y][current.x + 1])
		return (0);
	next.x = current.x + 1;
	next.y = current.y;
	return (search_path(map, rows, cols, next, target, visited, path));
}

/**
 * search_down - Recursively checks the bottom neighbour
 * @map: Map to search
 * @rows: Number of rows
 * @cols: Number of columns
 * @current: Current coordinates
 * @target: Target coordinates
 * @visited: Visited-cell map
 * @path: Address of the temporary path
 *
 * Return: 1 if a route is found, otherwise 0
 */
static int search_down(char **map, int rows, int cols, point_t current,
		       point_t const *target, char **visited,
		       path_node_t **path)
{
	point_t next;

	if (current.y + 1 >= rows || map[current.y + 1][current.x] != '0' ||
	    visited[current.y + 1][current.x])
		return (0);
	next.x = current.x;
	next.y = current.y + 1;
	return (search_path(map, rows, cols, next, target, visited, path));
}

/**
 * search_left - Recursively checks the left neighbour
 * @map: Map to search
 * @rows: Number of rows
 * @cols: Number of columns
 * @current: Current coordinates
 * @target: Target coordinates
 * @visited: Visited-cell map
 * @path: Address of the temporary path
 *
 * Return: 1 if a route is found, otherwise 0
 */
static int search_left(char **map, int rows, int cols, point_t current,
		       point_t const *target, char **visited,
		       path_node_t **path)
{
	point_t next;

	if (current.x <= 0 || map[current.y][current.x - 1] != '0' ||
	    visited[current.y][current.x - 1])
		return (0);
	next.x = current.x - 1;
	next.y = current.y;
	return (search_path(map, rows, cols, next, target, visited, path));
}

/**
 * search_up - Recursively checks the top neighbour
 * @map: Map to search
 * @rows: Number of rows
 * @cols: Number of columns
 * @current: Current coordinates
 * @target: Target coordinates
 * @visited: Visited-cell map
 * @path: Address of the temporary path
 *
 * Return: 1 if a route is found, otherwise 0
 */
static int search_up(char **map, int rows, int cols, point_t current,
		     point_t const *target, char **visited,
		     path_node_t **path)
{
	point_t next;

	if (current.y <= 0 || map[current.y - 1][current.x] != '0' ||
	    visited[current.y - 1][current.x])
		return (0);
	next.x = current.x;
	next.y = current.y - 1;
	return (search_path(map, rows, cols, next, target, visited, path));
}

/**
 * search_path - Recursively searches for a route to the target
 * @map: Map to search
 * @rows: Number of rows
 * @cols: Number of columns
 * @current: Current coordinates
 * @target: Target coordinates
 * @visited: Visited-cell map
 * @path: Address of the temporary path
 *
 * Return: 1 if a route is found, otherwise 0
 */
static int search_path(char **map, int rows, int cols, point_t current,
		       point_t const *target, char **visited,
		       path_node_t **path)
{
	path_node_t *node;

	printf("Checking coordinates [%d, %d]\n", current.x, current.y);
	visited[current.y][current.x] = 1;
	node = add_path_node(path, current);
	if (!node)
		return (0);
	if (current.x == target->x && current.y == target->y)
		return (1);
	if (search_right(map, rows, cols, current, target, visited, path) ||
	    search_down(map, rows, cols, current, target, visited, path) ||
	    search_left(map, rows, cols, current, target, visited, path) ||
	    search_up(map, rows, cols, current, target, visited, path))
		return (1);
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
	path_node_t *path;
	queue_t *queue;

	if (!valid_points(map, rows, cols, start, target))
		return (NULL);
	visited = create_visited(rows, cols);
	if (!visited)
		return (NULL);
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
