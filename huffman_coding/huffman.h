#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>

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

#endif

