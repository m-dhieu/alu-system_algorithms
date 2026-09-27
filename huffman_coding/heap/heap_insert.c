#include <stdlib.h>
#include "heap.h"

/**
 * get_parent_for_insert - Find parent where new node should be attached
 *
 * @root: Root of the heap
 *
 * Return: Pointer to parent node for insertion
 */
static binary_tree_node_t *get_parent_for_insert(binary_tree_node_t *root)
{
	binary_tree_node_t *node;
	binary_tree_node_t *parent;
	size_t depth, height, i;

	/* Compute height */
	height = 0;
	node = root;
	while (node != NULL)
	{
		height++;
		node = node->left;
	}

	/* Traverse to the second-last level */
	depth = 0;
	parent = root;
	while (depth < height - 2)
	{
		if (parent->right != NULL && parent->right->left == NULL)
		{
			parent = parent->right;
			break;
		}
		if (parent->left == NULL || parent->right == NULL)
			break;
		parent = (parent->left->left == NULL || parent->left->right == NULL) ?
			parent->left : parent->right;
		depth++;
	}

	/* Choose leftmost available spot on that level */
	node = parent;
	while (node->left != NULL && node->right != NULL)
	{
		if (node->right->left == NULL)
			return (node->right);
		node = node->right;
	}
	return (node);
}

/**
 * heapify_up - Bubble node up to maintain min-heap property
 *
 * @heap: Pointer to the heap
 * @node: Node to bubble up
 */
static void heapify_up(heap_t *heap, binary_tree_node_t *node)
{
	void *tmp;

	while (node->parent != NULL)
	{
		if (heap->data_cmp(node->data, node->parent->data) < 0)
		{
			tmp = node->data;
			node->data = node->parent->data;
			node->parent->data = tmp;
			node = node->parent;
		}
		else
		{
			break;
		}
	}
}

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
	binary_tree_node_t *parent;

	if (heap == NULL || data == NULL)
		return (NULL);

	new_node = binary_tree_node(NULL, data);
	if (new_node == NULL)
		return (NULL);

	if (heap->root == NULL)
	{
		heap->root = new_node;
		heap->size++;
		return (new_node);
	}

	parent = get_parent_for_insert(heap->root);
	if (parent->left == NULL)
	{
		parent->left = new_node;
		new_node->parent = parent;
	}
	else
	{
		parent->right = new_node;
		new_node->parent = parent;
	}

	heapify_up(heap, new_node);
	heap->size++;
	return (new_node);
}

