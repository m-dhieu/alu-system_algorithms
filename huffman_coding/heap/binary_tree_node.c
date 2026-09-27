#include <stdlib.h>
#include "heap.h"

/**
 * binary_tree_node - Creates a generic Binary Tree node
 *
 * @parent: Pointer to the parent node
 * @data: Pointer to the data to store in the node
 *
 * Return: Pointer to the created node, or NULL on failure
 */
binary_tree_node_t *binary_tree_node(binary_tree_node_t *parent, void *data)
{
	binary_tree_node_t *node;

	if (data == NULL)
		return (NULL);

	node = malloc(sizeof(binary_tree_node_t));
	if (node == NULL)
		return (NULL);

	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;

	return (node);
}

