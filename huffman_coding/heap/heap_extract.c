#include <stdlib.h>
#include "heap.h"

/**
 * find_last_node - Finds the last node in level order
 *
 * @root: Root of the heap
 *
 * Return: Pointer to the last node, or NULL if none
 */
static binary_tree_node_t *find_last_node(binary_tree_node_t *root)
{
	binary_tree_node_t *queue[1024];
	binary_tree_node_t *node;
	size_t head;
	size_t tail;

	if (root == NULL)
		return (NULL);

	head = 0;
	tail = 0;
	queue[tail++] = root;

	while (head < tail)
	{
		node = queue[head++];

		if (node->left != NULL)
			queue[tail++] = node->left;

		if (node->right != NULL)
			queue[tail++] = node->right;
	}

	return (node);
}

/**
 * find_parent_of - Finds the parent of a given node
 *
 * @root: Root of the heap
 * @node: Node whose parent is required
 *
 * Return: Pointer to the parent, or NULL if none
 */
static binary_tree_node_t *find_parent_of(binary_tree_node_t *root,
					  binary_tree_node_t *node)
{
	binary_tree_node_t *queue[1024];
	binary_tree_node_t *current;
	size_t head;
	size_t tail;

	if (root == NULL || node == NULL || root == node)
		return (NULL);

	head = 0;
	tail = 0;
	queue[tail++] = root;

	while (head < tail)
	{
		current = queue[head++];

		if (current->left == node || current->right == node)
			return (current);

		if (current->left != NULL)
			queue[tail++] = current->left;

		if (current->right != NULL)
			queue[tail++] = current->right;
	}

	return (NULL);
}

/**
 * heapify_down - Restores the min-heap property
 *
 * @heap: Pointer to the heap
 * @node: Node to move down
 *
 * Return: Nothing
 */
static void heapify_down(heap_t *heap, binary_tree_node_t *node)
{
	binary_tree_node_t *smallest;
	binary_tree_node_t *left;
	binary_tree_node_t *right;
	void *tmp;

	while (node != NULL)
	{
		smallest = node;
		left = node->left;
		right = node->right;

		if (left != NULL &&
		    heap->data_cmp(left->data, smallest->data) < 0)
			smallest = left;

		if (right != NULL &&
		    heap->data_cmp(right->data, smallest->data) <= 0)
			smallest = right;

		if (smallest == node)
			break;

		tmp = node->data;
		node->data = smallest->data;
		smallest->data = tmp;
		node = smallest;
	}
}

/**
 * remove_last_node - Removes the last node
 *
 * @heap: Pointer to the heap
 *
 * Return: Data stored in the removed node, or NULL on failure
 */
static void *remove_last_node(heap_t *heap)
{
	binary_tree_node_t *last;
	binary_tree_node_t *parent_last;
	void *data;

	last = find_last_node(heap->root);
	if (last == NULL)
		return (NULL);

	data = last->data;
	parent_last = find_parent_of(heap->root, last);

	if (parent_last != NULL)
	{
		if (parent_last->left == last)
			parent_last->left = NULL;
		else
			parent_last->right = NULL;
	}

	free(last);
	heap->size--;

	return (data);
}

/**
 * heap_extract - Extracts the root value of a min binary heap
 *
 * @heap: Pointer to the heap
 *
 * Return: Pointer to the root data, or NULL on failure
 */
void *heap_extract(heap_t *heap)
{
	void *root_data;
	void *last_data;

	if (heap == NULL || heap->root == NULL)
		return (NULL);

	root_data = heap->root->data;

	if (heap->root->left == NULL && heap->root->right == NULL)
	{
		free(heap->root);
		heap->root = NULL;
		heap->size--;
		return (root_data);
	}

	last_data = remove_last_node(heap);
	if (last_data == NULL)
		return (NULL);

	heap->root->data = last_data;
	heapify_down(heap, heap->root);

	return (root_data);
}

