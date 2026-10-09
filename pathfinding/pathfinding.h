#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "graphs.h"
#include "queues.h"

/**
 * struct point_s - Structure storing coordinates
 * @x: X coordinate
 * @y: Y coordinate
 */
typedef struct point_s
{
	int x;
	int y;
} point_t;

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

queue_t *backtracking_array(char **map, int rows, int cols,
			    point_t const *start, point_t const *target);
queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
			    vertex_t const *target);
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target);
queue_t *a_star_graph(graph_t *graph, vertex_t const *start,
		      vertex_t const *target);

#endif /* PATHFINDING_H */
