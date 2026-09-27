#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>
#include "heap/heap.h"

/**
 * struct symbol_s - Stores a char and its associated frequency
 *
 * @data: The character
 * @freq: The associated frequency
 */
typedef struct symbol_s
{
	char data;
	size_t freq;
} symbol_t;

/**
 * symbol_create - Creates a symbol
 *
 * @data: Character stored in the symbol
 * @freq: Frequency associated with the character
 *
 * Return: Pointer to the created symbol, or NULL on failure
 */

/* Prototypes */

/* Task 5: Create a symbol */
symbol_t *symbol_create(char data, size_t freq);
/* Task 6: Create a Huffman priority queue */
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size);
/* Task 7: Extract two nodes and insert their combined node */
int huffman_extract_and_insert(heap_t *priority_queue);
/* Task 8: Build a Huffman tree */
binary_tree_node_t *huffman_tree(char *data, size_t *freq, size_t size);
/* Free Huffman tree */
/* void free_huffman_tree(binary_tree_node_t *root); */
/* Task 9: Huffman codes */
int huffman_codes(char *data, size_t *freq, size_t size);

#endif

