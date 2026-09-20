# Graphs

This project implements graph creation, modification, deletion, and traversal functions in C.

Graphs are represented using adjacency lists. Each graph contains a linked list of vertices,
and each vertex contains a linked list of connected edges.

## Project Files

| File                         | Description                                                             |
| ---------------------------- | ----------------------------------------------------------------------- |
| `graphs.h`                   | Header file containing graph structures, types, and function prototypes |
| `0-graph_create.c`           | Creates and initializes an empty graph                                  |
| `1-graph_add_vertex.c`       | Adds a vertex to a graph                                                |
| `2-graph_add_edge.c`         | Adds a directed or bidirectional edge between vertices                  |
| `3-graph_delete.c`           | Deletes a graph, including all vertices and edges                       |
| `4-depth_first_traverse.c`   | Traverses a graph using depth-first search                              |
| `5-breadth_first_traverse.c` | Traverses a graph using breadth-first search                            |

## Data Structures

The graph uses the following main structures:

- `graph_t`: Represents the graph and stores the number of vertices and the linked list of vertices.
- `vertex_t`: Represents a vertex and stores its index, content, number of edges, edge list, and next vertex.
- `edge_t`: Represents an edge and stores the destination vertex and the next edge.
- `edge_type_t`: Specifies whether an edge is unidirectional or bidirectional.

## Functions

### `graph_create`

```
graph_t *graph_create(void);
```

Allocates and initializes an empty graph.

The function returns a pointer to the new graph or `NULL` if memory allocation fails.

### `graph_add_vertex`

```
vertex_t *graph_add_vertex(graph_t *graph, const char *str);
```

Adds a new vertex containing the supplied string to the graph.

The function returns a pointer to the new vertex or `NULL` if the graph, string, or
memory allocation is invalid.

### `graph_add_edge`

```
int graph_add_edge(graph_t *graph, const char *src, const char *dest,
		   edge_type_t type);
```

Adds an edge from the source vertex to the destination vertex.

- `UNIDIRECTIONAL` adds an edge in one direction.
- `BIDIRECTIONAL` adds an edge in both directions.

The function returns `1` on success and `0` on failure.

### `graph_delete`

```
void graph_delete(graph_t *graph);
```

Completely deletes a graph by freeing:

- All edge nodes.
- All vertex contents.
- All vertices.
- The graph structure itself.

### `depth_first_traverse`

```
size_t depth_first_traverse(const graph_t *graph,
			    void (*action)(const vertex_t *v, size_t depth));
```

Traverses the graph using the depth-first search algorithm.

Traversal starts from the first vertex in the graph's vertex list. The `action` function
is called for each visited vertex with its depth from the starting vertex.

The function returns the greatest depth reached or `0` on failure.

### `breadth_first_traverse`

```
size_t breadth_first_traverse(const graph_t *graph,
			      void (*action)(const vertex_t *v,
					     size_t depth));
```

Traverses the graph using the breadth-first search algorithm.

Traversal starts from the first vertex in the graph's vertex list. Vertices are visited
level by level, and the `action` function is called for each visited vertex with its
depth from the starting vertex.

The function returns the greatest depth reached or `0` on failure.

## Edge Types

The project defines two edge types:

```
typedef enum edge_type_e
{
	UNIDIRECTIONAL = 0,
	BIDIRECTIONAL
} edge_type_t;
```

A unidirectional edge connects the source vertex to the destination vertex in one direction:

```
A -> B
```

A bidirectional edge creates connections in both directions:

```
A <-> B
```

## Compilation

Compile the project using GCC with the required flags:

```
gcc -Wall -Wextra -Werror -pedantic
```

For example, to compile the depth-first traversal test:

```
gcc -Wall -Wextra -Werror -pedantic -g3 \
4-main.c graph_create.c graph_add_vertex.c graph_add_edge.c \
graph_delete.c 4-depth_first_traverse.c graph_display.c \
-o 4-depth_first_traverse
```

To compile the breadth-first traversal test:

```
gcc -Wall -Wextra -Werror -pedantic -g3 \
5-main.c graph_create.c graph_add_vertex.c graph_add_edge.c \
graph_delete.c 5-breadth_first_traverse.c graph_display.c \
-o 5-breadth_first_traverse
```

## Execution

Run a compiled program as follows:

```bash
./4-depth_first_traverse
```

or:

```bash
./5-breadth_first_traverse
```

## Requirements

- Ubuntu 14.04 LTS compatibility.
- GCC 4.8.4 compatibility.
- Compilation with `-Wall -Werror -Wextra -pedantic`.
- Betty coding style.
- Betty documentation style.
- No global variables.
- All function prototypes included in `graphs.h`.
- Include guards used in the header file.
- No more than five functions per source file.
- Every file ends with a newline.
