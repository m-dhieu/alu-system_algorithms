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
 * huffman_extract_and_insert - Extracts two nodes and inserts their sum
 *
 * @priority_queue: Pointer to the Huffman priority queue
 *
 * Return: 1 on success, or 0 on failure
 */
int huffman_extract_and_insert(heap_t *priority_queue)
{
	binary_tree_node_t *left_nested;
	binary_tree_node_t *right_nested;
	binary_tree_node_t *parent_nested;
	symbol_t *left_symbol;
	symbol_t *right_symbol;
	symbol_t *parent_symbol;
	size_t total_frequency;

	if (priority_queue == NULL || priority_queue->size < 2)
		return (0);

	left_nested = (binary_tree_node_t *)heap_extract(priority_queue);
	right_nested = (binary_tree_node_t *)heap_extract(priority_queue);

	if (left_nested == NULL || right_nested == NULL)
	{
		free_nested(left_nested);
		free_nested(right_nested);
		return (0);
	}

	left_symbol = (symbol_t *)left_nested->data;
	right_symbol = (symbol_t *)right_nested->data;
	total_frequency = left_symbol->freq + right_symbol->freq;

	parent_symbol = symbol_create(-1, total_frequency);
	if (parent_symbol == NULL)
	{
		free_nested(left_nested);
		free_nested(right_nested);
		return (0);
	}

	parent_nested = binary_tree_node(NULL, parent_symbol);
	if (parent_nested == NULL)
	{
		free(parent_symbol);
		free_nested(left_nested);
		free_nested(right_nested);
		return (0);
	}

	parent_nested->left = left_nested;
	parent_nested->right = right_nested;
	left_nested->parent = parent_nested;
	right_nested->parent = parent_nested;

	if (heap_insert(priority_queue, parent_nested) == NULL)
	{
		free_nested(parent_nested);
		return (0);
	}

	return (1);
}

