#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"
#include "heap.h"

/**
 * print_codes_recursive - Recursively traverse tree and print Huffman codes
 *
 * @node: Current node in the Huffman tree
 * @code: Current code buffer
 * @len: Current length of code
 */
static void print_codes_recursive(const binary_tree_node_t *node,
				  char *code, size_t len)
{
	symbol_t *sym;

	if (node == NULL)
		return;

	if (node->left == NULL && node->right == NULL)
	{
		code[len] = '\0';
		sym = (symbol_t *)node->data;
		printf("%c: %s\n", sym->data, code);
		return;
	}

	if (node->left != NULL)
	{
		code[len] = '0';
		print_codes_recursive(node->left, code, len + 1);
	}

	if (node->right != NULL)
	{
		code[len] = '1';
		print_codes_recursive(node->right, code, len + 1);
	}
}

/**
 * huffman_codes - Build Huffman tree and print Huffman codes
 *
 * @data: Array of characters
 * @freq: Array of frequencies
 * @size: Number of symbols
 *
 * Return: 1 on success, 0 on failure
 */
int huffman_codes(char *data, size_t *freq, size_t size)
{
	binary_tree_node_t *root;
	char code[256];

	if (data == NULL || freq == NULL || size == 0)
		return (0);

	/* Build Huffman tree using your existing helper */
	root = huffman_tree(data, freq, size);
	if (root == NULL)
		return (0);

	/* Print codes via DFS */
	print_codes_recursive(root, code, 0);

	/* Free the tree */
	/* free_huffman_tree(root); */

	return (1);
}

