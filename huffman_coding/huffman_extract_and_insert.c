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
 * free_tree - Frees a binary tree recursively
 *
 * @node: Root of the tree
 *
 * Return: Nothing
 */
static void free_tree(binary_tree_node_t *node)
{
	if (node == NULL)
		return;

	free_tree(node->left);
	free_tree(node->right);
	free_nested(node);
}

/**
 * create_parent - Creates a parent node for two nested nodes
 *
 * @left: First extracted node
 * @right: Second extracted node
 *
 * Return: Pointer to the created parent node, or NULL on failure
 */
static binary_tree_node_t *create_parent(binary_tree_node_t *left,
		binary_tree_node_t *right)
{
	binary_tree_node_t *parent;
	symbol_t *left_symbol;
	symbol_t *right_symbol;
	symbol_t *symbol;
	size_t frequency;

	left_symbol = (symbol_t *)left->data;
	right_symbol = (symbol_t *)right->data;
	frequency = left_symbol->freq + right_symbol->freq;

	symbol = symbol_create(-1, frequency);
	if (symbol == NULL)
		return (NULL);

	parent = binary_tree_node(NULL, symbol);
	if (parent == NULL)
	{
		free(symbol);
		return (NULL);
	}

	parent->left = left;
	parent->right = right;
	left->parent = parent;
	right->parent = parent;

	return (parent);
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
	binary_tree_node_t *left;
	binary_tree_node_t *right;
	binary_tree_node_t *parent;

	if (priority_queue == NULL || priority_queue->size < 2)
		return (0);

	left = (binary_tree_node_t *)heap_extract(priority_queue);
	right = (binary_tree_node_t *)heap_extract(priority_queue);

	if (left == NULL || right == NULL)
	{
		free_tree(left);
		free_tree(right);
		return (0);
	}

	parent = create_parent(left, right);
	if (parent == NULL)
	{
		free_tree(left);
		free_tree(right);
		return (0);
	}

	if (heap_insert(priority_queue, parent) == NULL)
	{
		free_tree(parent);
		return (0);
	}

	return (1);
}

