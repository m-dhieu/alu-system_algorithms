#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "huffman.h"

void binary_tree_print(const binary_tree_node_t *heap,
		       int (*print_data)(char *, void *));

/**
 * symbol_print - Prints a symbol structure
 *
 * @buffer: Buffer to print into
 * @data: Pointer to a symbol
 *
 * Return: Number of bytes written to the buffer
 */
int symbol_print(char *buffer, void *data)
{
	symbol_t *symbol;
	char c;
	int length;

	symbol = (symbol_t *)data;
	c = symbol->data;

	if (c == -1)
		c = '$';

	length = sprintf(buffer, "(%c/%lu)", c, symbol->freq);

	return (length);
}

/**
 * free_tree - Frees a Huffman tree
 *
 * @node: Root of the tree
 *
 * Return: Nothing
 */
void free_tree(binary_tree_node_t *node)
{
	if (node == NULL)
		return;

	free_tree(node->left);
	free_tree(node->right);
	free(node->data);
	free(node);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	binary_tree_node_t *root;
	char data[] = {'a', 'b', 'c', 'd', 'e', 'f'};
	size_t freq[] = {6, 11, 12, 13, 16, 36};
	size_t size = sizeof(data) / sizeof(data[0]);

	root = huffman_tree(data, freq, size);
	if (root == NULL)
	{
		fprintf(stderr, "Failed to build Huffman tree\n");
		return (EXIT_FAILURE);
	}

	binary_tree_print(root, symbol_print);
	free_tree(root);

	return (EXIT_SUCCESS);
}

