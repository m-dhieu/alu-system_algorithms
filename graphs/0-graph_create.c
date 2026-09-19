#ifndef GRAPHS_H
#define GRAPHS_H

#include <stddef.h>

/**
 * enum edge_type_e - enumerates different connection types
 * between two vertices
 *
 * @UNIDIRECTIONAL: connection is one way
 * @BIDIRECTIONAL: connection is two ways
 */
typedef enum edge_type_e
{
	UNIDIRECTIONAL = 0,
	BIDIRECTIONAL
} edge_type_t;

/* define structure for usage in edge_t */
typedef struct vertex_s vertex_t;

/**
 * struct edge_s - node in the linked list of edges for a given vertex
 *
 * @dest: pointer to the connected vertex
 * @next: pointer to the next edge
 */
typedef struct edge_s
{
	vertex_t *dest;
	struct edge_s *next;
} edge_t;

/**
 * struct vertex_s - node in the linked list of vertices
 *
 * @index: index of the vertex in the adjacency list
 * @content: custom data stored in the vertex
 * @nb_edges: number of connections with other vertices
 * @edges: pointer to the head node of the linked list of edges
 * @next: pointer to the next vertex in the graph
 */
struct vertex_s
{
	size_t index;
	char *content;
	size_t nb_edges;
	edge_t *edges;
	struct vertex_s *next;
};

/**
 * struct graph_s - graph representation
 *
 * @nb_vertices: number of vertices in the graph
 * @vertices: pointer to the head node of the adjacency list
 */
typedef struct graph_s
{
	size_t nb_vertices;
	vertex_t *vertices;
} graph_t;

graph_t *graph_create(void);

#endif /* GRAPHS_H */

