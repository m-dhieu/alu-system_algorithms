#include <stdlib.h>
#include "huffman.h"

/**
 * subtree_min_char - Finds the smallest character in a subtree
 *
 * @node: Root of the subtree
 *
 * Return: Smallest character in the subtree
 */
static char subtree_min_char(binary_tree_node_t *node)
{
	char left_char;
	char right_char;
	char current_char;

	if (node == NULL || node->data == NULL)
		return ((char)127);

	current_char = ((symbol_t *)node->data)->data;
	if (current_char != -1)
		return (current_char);

	left_char = subtree_min_char(node->left);
	right_char = subtree_min_char(node->right);

	return (left_char < right_char ? left_char : right_char);
}

/**
 * symbol_cmp - Compares two nested symbol nodes
 *
 * @p1: First nested node
 * @p2: Second nested node
 *
 * Return: Negative value, zero, or positive value
 */
static int symbol_cmp(void *p1, void *p2)
{
	binary_tree_node_t *node1;
	binary_tree_node_t *node2;
	symbol_t *symbol1;
	symbol_t *symbol2;
	char min1;
	char min2;

	node1 = (binary_tree_node_t *)p1;
	node2 = (binary_tree_node_t *)p2;
	symbol1 = (symbol_t *)node1->data;
	symbol2 = (symbol_t *)node2->data;

	if (symbol1->freq < symbol2->freq)
		return (-1);

	if (symbol1->freq > symbol2->freq)
		return (1);

	min1 = subtree_min_char(node1);
	min2 = subtree_min_char(node2);

	if (min1 < min2)
		return (-1);

	if (min1 > min2)
		return (1);

	return (0);
}

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
 * huffman_priority_queue - Creates a Huffman priority queue
 *
 * @data: Array of characters
 * @freq: Array of associated frequencies
 * @size: Number of elements in the arrays
 *
 * Return: Pointer to the created heap, or NULL on failure
 */
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size)
{
	heap_t *heap;
	symbol_t *symbol;
	binary_tree_node_t *nested;
	size_t i;

	if (data == NULL || freq == NULL || size == 0)
		return (NULL);

	heap = heap_create(symbol_cmp);
	if (heap == NULL)
		return (NULL);

	for (i = 0; i < size; i++)
	{
		symbol = symbol_create(data[i], freq[i]);
		if (symbol == NULL)
		{
			heap_delete(heap, free_nested);
			return (NULL);
		}

		nested = binary_tree_node(NULL, symbol);
		if (nested == NULL)
		{
			free(symbol);
			heap_delete(heap, free_nested);
			return (NULL);
		}

		if (heap_insert(heap, nested) == NULL)
		{
			free_nested(nested);
			heap_delete(heap, free_nested);
			return (NULL);
		}
	}

	return (heap);
}

