#include <stdlib.h>
#include "huffman.h"

/**
 * free_nested - Frees a nested node and its symbol
 *
 * @data: Pointer to a nested node
 *
 * Return: Nothing
 */
static void free_nested(void *data)
{
	binary_tree_node_t *nested;

	nested = (binary_tree_node_t *)data;
	if (nested == NULL)
		return;

	free(nested->data);
	free(nested);
}

/**
 * huffman_tree - Builds a Huffman tree
 *
 * @data: Array of characters
 * @freq: Array of character frequencies
 * @size: Number of elements in the arrays
 *
 * Return: Root of the Huffman tree, or NULL on failure
 */
binary_tree_node_t *huffman_tree(char *data, size_t *freq, size_t size)
{
	heap_t *priority_queue;
	binary_tree_node_t *root;

	if (data == NULL || freq == NULL || size == 0)
		return (NULL);

	priority_queue = huffman_priority_queue(data, freq, size);
	if (priority_queue == NULL)
		return (NULL);

	while (priority_queue->size > 1)
	{
		if (huffman_extract_and_insert(priority_queue) == 0)
		{
			heap_delete(priority_queue, free_nested);
			return (NULL);
		}
	}

	root = (binary_tree_node_t *)heap_extract(priority_queue);
	free(priority_queue);

	return (root);
}

