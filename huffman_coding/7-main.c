#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "huffman.h"

void binary_tree_print(const binary_tree_node_t *heap,
		       int (*print_data)(char *, void *));

/**
 * nested_print - Prints a symbol stored in a nested node
 *
 * @buffer: Buffer to print into
 * @data: Pointer to a nested node
 *
 * Return: Number of bytes written to the buffer
 */
int nested_print(char *buffer, void *data)
{
	binary_tree_node_t *nested;
	symbol_t *symbol;
	char c;
	int length;

	nested = (binary_tree_node_t *)data;
	symbol = (symbol_t *)nested->data;
	c = symbol->data;

	if (c == -1)
		c = '$';

	length = sprintf(buffer, "(%c/%lu)", c, symbol->freq);

	return (length);
}

/**
 * free_nested - Frees a nested node and its symbol
 *
 * @data: Pointer to a nested node
 *
 * Return: Nothing
 */
void free_nested(void *data)
{
	binary_tree_node_t *nested;

	nested = (binary_tree_node_t *)data;
	if (nested == NULL)
		return;

	free(nested->data);
	free(nested);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	heap_t *priority_queue;
	char data[] = {'a', 'b', 'c', 'd', 'e', 'f'};
	size_t freq[] = {6, 11, 12, 13, 16, 36};
	size_t size = sizeof(data) / sizeof(data[0]);

	priority_queue = huffman_priority_queue(data, freq, size);
	if (priority_queue == NULL)
	{
		fprintf(stderr, "Failed to create priority queue\n");
		return (EXIT_FAILURE);
	}

	binary_tree_print(priority_queue->root, nested_print);
	printf("\n");

	if (huffman_extract_and_insert(priority_queue) == 0)
	{
		fprintf(stderr, "Failed to extract and insert nodes\n");
		heap_delete(priority_queue, free_nested);
		return (EXIT_FAILURE);
	}

	binary_tree_print(priority_queue->root, nested_print);
	printf("\n");

	if (huffman_extract_and_insert(priority_queue) == 0)
	{
		fprintf(stderr, "Failed to extract and insert nodes\n");
		heap_delete(priority_queue, free_nested);
		return (EXIT_FAILURE);
	}

	binary_tree_print(priority_queue->root, nested_print);
	printf("\n");

	heap_delete(priority_queue, free_nested);

	return (EXIT_SUCCESS);
}

