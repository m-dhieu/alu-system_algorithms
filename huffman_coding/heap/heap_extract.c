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
 * remove_last_node - Remove last node and return its data
 *
 * @heap: Pointer to the heap
 *
 * Return: Data from the last node
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
 * extract_single_node - Handle extraction when only root exists
 *
 * @heap: Pointer to the heap
 *
 * Return: Data from the root
 */
static void *extract_single_node(heap_t *heap)
{
	void *data;

	data = heap->root->data;
	free(heap->root);
	heap->root = NULL;
	heap->size--;

	return (data);
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
	void *last_data;

	if (heap == NULL || heap->root == NULL)
		return (NULL);

	root_data = heap->root->data;

	if (heap->root->left == NULL && heap->root->right == NULL)
		return (extract_single_node(heap));

	last_data = remove_last_node(heap);
	if (last_data == NULL)
	{
		free(heap->root);
		heap->root = NULL;
		return (root_data);
	}

	heap->root->data = last_data;
	heapify_down(heap, heap->root);

	return (root_data);
}

