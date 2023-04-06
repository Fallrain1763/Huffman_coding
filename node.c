#include <stdio.h>
#include <stdlib.h>
#include "node.h"

Node *node_create(uint8_t symbol, uint64_t frequency)
{   // alloc
    Node *node = (Node *)malloc(sizeof(Node));

    // asgn var
    if(node != NULL)
    {
        node->symbol = symbol;
        node->frequency = frequency;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

void node_delete(Node **n)
{
    if(*n != NULL)
    {
        free(*n);
        *n = NULL;
    }
}

Node *node_join(Node *left, Node *right)
{
    uint64_t frequency = left->frequency + right->frequency;
    uint8_t symbol = '$';
    Node *parent = node_create(symbol, frequency);
    parent->left = left;
    parent->right = right;

    return parent;
}

void node_print(Node *n)
{
    if(n == NULL)
        printf("NULL\n");
    else
        printf("%c: %lu\n", n->symbol, n->frequency);
    
}

bool node_cmp(Node *n, Node *m)
{
    return n->frequency > m->frequency;    
}

void node_print_sym(Node *n)
{
    if(n == NULL)
        printf("NULL\n");
    else
        printf("%c\n", n->symbol);
}
