#ifndef HUFF_H
#define HUFF_H

#include "concat_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT_SYMBOLS 256
#define MAX_SIZE_CODE 64

struct Node
{
    struct Node* next;
    struct Node* left;
    struct Node* right;

    size_t data;
    int symbol;
};

struct Code
{
    int size;
    char code[MAX_SIZE_CODE];
};

void printTree(struct Node* node, int n);
void printList(struct Node* head);
struct Node* createNode(size_t data, int symbol);
void insertNodeInList(struct Node** list, struct Node* node);
struct Node* createList(size_t* table);
struct Node* createTree(struct Node* list);
void createCodes(struct Code* codes, struct Node* node, char* buf);
size_t* createTableFrequencies(FILE* file);
void writeHuffTreeInBuffer(struct Node* node, char* buffer, int* lengthTree, char* countUsedBits);
void printTable(size_t* table);
void printCodes(struct Code* codes);

// THE MOST IMPORTANT FUNCTION, BITCH!!!!
void coding(struct Code* codes, char* bytesForCoding, int countBytesForCoding, unsigned char* codingBits, int* countCodingBits);

#endif // HUFF_H
