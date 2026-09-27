#include <stdlib.h>
#include "heap.h"

/**
 * find_last_node - Find the last node in level-order
 *
 * @root: Root of the heap
 *
 * Return: Pointer to the last node, or NULL if none
 */
static binary_tree_node_t *find_last_node(binary_tree_node_t *root)
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
		if (node->left != NULL)
			queue[tail++] = node->left;
		if (node->right != NULL)
			queue[tail++] = node->right;
	}

	return (node);
}

/**
 * find_parent_of - Find parent of a given node
 *
 * @root: Root of the tree
 * @node: Node whose parent to find
 *
 * Return: Pointer to parent node, or NULL if none
 */
static binary_tree_node_t *find_parent_of(binary_tree_node_t *root,
					  binary_tree_node_t *node)
{
	binary_tree_node_t *queue[1024];
	size_t head;
	size_t tail;
	binary_tree_node_t *current;

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
 * heapify_down - Bubble node down to maintain min-heap property
 *
 * @heap: Pointer to the heap
 * @node: Node to bubble down
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

		if (left != NULL && heap->data_cmp(left->data, smallest->data) < 0)
			smallest = left;
		if (right != NULL && heap->data_cmp(right->data, smallest->data) < 0)
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
 * heap_extract - Extracts the root value of a Min Binary Heap
 *
 * @heap: Pointer to the heap
 *
 * Return: Pointer to the data from the root, or NULL on failure
 */
void *heap_extract(heap_t *heap)
{
	void *root_data;
	binary_tree_node_t *last;
	binary_tree_node_t *parent_last;
	binary_tree_node_t *old_root;

	if (heap == NULL || heap->root == NULL)
		return (NULL);

	root_data = heap->root->data;
	old_root = heap->root;

	if (heap->root->left == NULL && heap->root->right == NULL)
	{
		/* Only one node */
		heap->root = NULL;
		heap->size--;
		free(old_root);
		return (root_data);
	}

	/* Find last node in level-order */
	last = find_last_node(heap->root);
	if (last == NULL)
	{
		free(old_root);
		heap->root = NULL;
		heap->size--;
		return (root_data);
	}

	/* Find parent of last node */
	parent_last = find_parent_of(heap->root, last);

	/* Move last node's data to root */
	heap->root->data = last->data;

	/* Remove last node from its parent */
	if (parent_last != NULL)
	{
		if (parent_last->left == last)
			parent_last->left = NULL;
		else
			parent_last->right = NULL;
	}

	free(last);
	heap->size--;

	/* Restore heap property */
	heapify_down(heap, heap->root);

	return (root_data);
}

