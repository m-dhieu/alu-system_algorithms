#include <stdlib.h>
#include "heap.h"

/**
 * heap_insert - Inserts a value in a Min Binary Heap
 *
 * @heap: Pointer to the heap
 * @data: Pointer to the data to store in the new node
 *
 * Return: Pointer to the created node, or NULL on failure
 */
binary_tree_node_t *heap_insert(heap_t *heap, void *data)
{
	binary_tree_node_t *new_node;
	binary_tree_node_t *current;
	binary_tree_node_t *parent;
	void *tmp_data;

	if (heap == NULL || data == NULL)
		return (NULL);

	new_node = binary_tree_node(NULL, data);
	if (new_node == NULL)
		return (NULL);

	/* Insert as root if heap is empty */
	if (heap->root == NULL)
	{
		heap->root = new_node;
		heap->size += 1;
		return (new_node);
	}

	/* Insert following a left-heavy path */
	current = heap->root;
	parent = NULL;

	while (current != NULL)
	{
		parent = current;
		current = current->left;
	}

	/* Attach new_node as left child */
	parent->left = new_node;
	new_node->parent = parent;

	/* Heapify up to maintain min-heap property */
	current = new_node;
	while (current->parent != NULL)
	{
		if (heap->data_cmp(current->data, current->parent->data) < 0)
		{
			/* Swap data pointers */
			tmp_data = current->data;
			current->data = current->parent->data;
			current->parent->data = tmp_data;
			current = current->parent;
		}
		else
		{
			break;
		}
	}

	heap->size += 1;
	return (new_node);
}

