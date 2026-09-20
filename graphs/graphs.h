#ifndef GRAPHS_H
#define GRAPHS_H

#include <stddef.h>

/**
 * enum edge_type_e - Enumerates the different types of
 * connection between two vertices
 *
 * @UNIDIRECTIONAL: The connection is made only in one way
 * @BIDIRECTIONAL: The connection is made in two ways
 */
typedef enum edge_type_e
{
	UNIDIRECTIONAL = 0,
	BIDIRECTIONAL
} edge_type_t;

/* Define the structure temporarily for usage in edge_t */
typedef struct vertex_s vertex_t;

/**
 * struct edge_s - Node in the linked list of edges for a given vertex
 *
 * @dest: Pointer to the connected vertex
 * @next: Pointer to the next edge
 */
typedef struct edge_s
{
	vertex_t *dest;
	struct edge_s *next;
} edge_t;

/**
 * struct vertex_s - Node in the linked list of vertices
 *
 * @index: Index of the vertex in the adjacency list
 * @content: Custom data stored in the vertex
 * @nb_edges: Number of connections with other vertices
 * @edges: Pointer to the head node of the linked list of edges
 * @next: Pointer to the next vertex in the graph
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
 * struct graph_s - Representation of a graph
 *
 * @nb_vertices: Number of vertices in the graph
 * @vertices: Pointer to the head node of the adjacency list
 */
typedef struct graph_s
{
	size_t nb_vertices;
	vertex_t *vertices;
} graph_t;

/* PROTOTYPES */
/* Task 0: Create graph */
graph_t *graph_create(void);

/* Task 1: Add a vertex */
vertex_t *graph_add_vertex(graph_t *graph, const char *str);

/* Task 2: Add an edge */
int graph_add_edge(graph_t *graph, const char *src, const char *dest,
	edge_type_t type);

/* Task 3: Delete graph */
void graph_delete(graph_t *graph);

/* Task 4: DFS*/
size_t depth_first_traverse(const graph_t *graph,
	void (*action)(const vertex_t *v, size_t depth));

#endif /* GRAPHS_H */

