#include <stdio.h>
#include "pq.h"
#include "stack.h"
#include "huffman.h"
#include "io.h"

static Code c;

Node *build_tree(uint64_t hist[static ALPHABET])
{
    // create a pq
    PriorityQueue *q = pq_create(ALPHABET);
    for(int i=0; i<ALPHABET; i++)
    {   
        // if the symb freq is not 0, create a node put into pq
        if(hist[i] != 0)
        {
            Node *n = node_create(i, hist[i]);
            enqueue(q, n);
        }
    }

    Node *left;
    Node *right;
    Node *parent;

    // pop two node, join together and push back to pq, repeat until there is only one node in pq
    // first is left, second is right
    while(pq_size(q) > 1)
    {
        dequeue(q, &left);
        dequeue(q, &right);
        parent = node_join(left, right);
        enqueue(q, parent);
    }

    Node *root;
    dequeue(q, &root);
    pq_delete(&q);
    
    return root;
}

void build_codes(Node *root, Code table[static ALPHABET])
{
    if(root != NULL)
    {
        // if leaf, put the code into code table
        if(!(root->left) && !(root->right))
            table[root->symbol] = c;
        else
        {
            uint8_t bit;
            
            // 0 go left, 1  go right
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, &bit);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &bit);
        }
    }
}

void dump_tree(int outfile, Node *root)
{
    if(root)
    {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);

        // if leaf, write 'L' follow by symb, else write 'I'
        if(!(root->left) && !(root->right))
        {
            uint8_t buf1[2];
            buf1[0] = 'L';
            buf1[1] = root->symbol;
            write_bytes(outfile, buf1, 2);
        }
        else
        {
            uint8_t buf2[1];
            buf2[0] = 'I';
            write_bytes(outfile, buf2, 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes])
{
    Stack *s = stack_create(ALPHABET);
    bool is_last_L = false;

    // go through the tree array
    for(uint16_t i=0; i<nbytes; i++)
    {
        // if last symbal is 'L' , put the node into stack
        if(is_last_L)
        {
            stack_push(s, node_create(tree[i], 0));
            is_last_L = false;
        }
        else
        {
            // if is L, set is_last_L true, else pop two, join, pushback, first is right, secode is left
            if(tree[i] == 'L')
                is_last_L = true;
            if(tree[i] == 'I')
            {
                Node *right;
                Node *left;
                stack_pop(s, &right);
                stack_pop(s, &left);
                stack_push(s, node_join(left, right));
            }
        }
    }

    Node *root;
    stack_pop(s, &root);
    stack_delete(&s);

    return root;
}

void delete_tree(Node **root)
{
    if(*root != NULL)
    {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        //node_print(*root);
        node_delete(&(*root));
    }
}
