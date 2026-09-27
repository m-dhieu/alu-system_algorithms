#include <stdlib.h>
#include "heap.h"

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
 * find_insert_parent - Find parent node for level-order insertion
 *
 * @root: Root of the heap
 *
 * Return: Pointer to parent where new node should be attached
 */
static binary_tree_node_t *find_insert_parent(binary_tree_node_t *root)
{
	binary_tree_node_t *queue[1024];
	size_t head;
	size_t tail;
	binary_tree_node_t *node;

	if (root == NULL)
		return (NULL);

	head = 0;
	tail = 0;
	queue[tail++] = root;

	while (head < tail)
	{
		node = queue[head++];

		if (node->left == NULL || node->right == NULL)
			return (node);

		queue[tail++] = node->left;
		queue[tail++] = node->right;
	}

	return (node);
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

	parent = find_insert_parent(heap->root);
	if (parent == NULL)
	{
		free(new_node);
		return (NULL);
	}

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

