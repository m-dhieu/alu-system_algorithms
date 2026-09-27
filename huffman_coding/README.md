# Huffman Coding – ALX System & Algorithms

This project implements Huffman coding in C, using a min binary heap to build optimal prefix codes for data compression.

## Requirements

- Compiled on Ubuntu 14.04 LTS
- Compiler: `gcc` with flags: `-Wall -Werror -Wextra -pedantic`
- Code style: Betty (`betty-style.pl` and `betty-doc.pl`)
- No global variables
- Maximum 5 functions per file
- All header files must be include-guarded

## Data structures

- Min binary heap (`heap_t`) built on binary tree nodes (`binary_tree_node_t`)
- Symbol structure (`symbol_t`) storing a character and its frequency

## Compilation example

```bash
gcc -Wall -Wextra -Werror -pedantic -Iheap/ -I./ heap/*.c *.c -o huffman_demo
```

## Usage

Create a main.c to test functions.

