#include <stdlib.h>
#include "huffman.h"

/**
 * symbol_create - Creates a symbol
 *
 * @data: Character stored in the symbol
 * @freq: Frequency associated with the character
 *
 * Return: Pointer to the created symbol, or NULL on failure
 */
symbol_t *symbol_create(char data, size_t freq)
{
	symbol_t *symbol;

	symbol = malloc(sizeof(*symbol));
	if (symbol == NULL)
		return (NULL);

	symbol->data = data;
	symbol->freq = freq;

	return (symbol);
}

