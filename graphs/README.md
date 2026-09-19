# Graphs

This project contains functions for creating and manipulating graphs
represented using adjacency linked lists.

## Task 0: Create graph

The `graph_create` function allocates memory for a `graph_t` structure
and initializes it with:

- No vertices.
- A `NULL` vertex list.

## Prototype

```c
graph_t *graph_create(void);
```

## Compilation

```bash
gcc -Wall -Wextra -Werror -pedantic 0-main.c 0-graph_create.c -o 0-graph_create
```

## Execution

```bash
./0-graph_create
```

Expected output:

```text
Successfully created graph
```

