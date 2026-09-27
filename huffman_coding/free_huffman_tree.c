#include <stdlib.h>
#include "huffman.h"
#include "heap.h"

/**
 * free_huffman_tree - Frees a Huffman tree
 *
 * @root: Root of the tree to free
 */
void free_huffman_tree(binary_tree_node_t *root)
{
	if (root == NULL)
		return;

	free_huffman_tree(root->left);
	free_huffman_tree(root->right);

	/* Free symbol stored in data if it's a leaf */
	if (root->left == NULL && root->right == NULL)
		free(root->data);

	free(root);
}

