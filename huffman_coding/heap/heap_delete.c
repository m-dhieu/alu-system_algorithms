#include <stdlib.h>
#include "heap.h"

/**
 * free_tree - Frees a binary tree recursively
 *
 * @node: Pointer to the root node
 * @free_data: Function to free node data
 *
 * Return: Nothing
 */
static void free_tree(binary_tree_node_t *node, void (*free_data)(void *))
{
	if (node == NULL)
		return;

	free_tree(node->left, free_data);
	free_tree(node->right, free_data);

	if (free_data != NULL)
		free_data(node->data);

	free(node);
}

/**
 * heap_delete - Deallocates a heap
 *
 * @heap: Pointer to the heap to delete
 * @free_data: Function to free node data
 *
 * Return: Nothing
 */
void heap_delete(heap_t *heap, void (*free_data)(void *))
{
	if (heap == NULL)
		return;

	free_tree(heap->root, free_data);
	free(heap);
}

