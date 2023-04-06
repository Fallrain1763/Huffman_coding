#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

struct PriorityQueue
{
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

PriorityQueue *pq_create(uint32_t capacity)
{
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if(q != NULL)
    {
        q->top = 0;
        q->capacity = capacity;
        q->items = (Node **) calloc(capacity, sizeof(Node *));
        if(!q->items)
        {
            free(q);
            q = NULL;
        }
    }

    return q;
}

void pq_delete(PriorityQueue **q)
{
    if(*q != NULL)
    {
        /*
        for(uint32_t i=0; i<(*q)->capacity; i++)
        {
            node_delete(&((*q)->items[i]));
        }
        */
        free((*q)->items);
        (*q)->items = NULL;

        free(*q);
        *q = NULL;
    }
}

bool pq_empty(PriorityQueue *q)
{
    return pq_size(q) == 0;
}

bool pq_full(PriorityQueue *q)
{
    return pq_size(q) == q->capacity;
}

uint32_t pq_size(PriorityQueue *q)
{
    return q->top;
}

bool enqueue(PriorityQueue *q, Node *n)
{
    if(pq_full(q))
        return false;
    else
    {
        bool isleast = true;
        for(uint32_t i=0; i<pq_size(q); i++)
        {   
            // if n freq is greater than item
            if(node_cmp(n, q->items[i]))
            {
                // shift element at right size of i item right by one
                for(uint32_t j=pq_size(q); j>i; j--)
                    q->items[j] = q->items[j-1];

                q->items[i] = n;
                isleast = false;
                break;
            }
        }

        if(isleast)
            q->items[pq_size(q)] = n;

        q->top++;

        return true;
    }
}

bool dequeue(PriorityQueue *q, Node **n)
{
    if(pq_empty(q))
        return false;
    else
    {
        // 5 4 3 2 1 
        //           ^
        // 5 4 3 2 1
        //         ^
        // 5 4 3 2 
        //         ^
        q->top--;
        *n = q->items[q->top];

        return true;
    }
}

void pq_print(PriorityQueue *q)
{
    if(q == NULL)
        printf("NULL PQ\n");
    else
    {
        // go through and print
        for(uint32_t i=0; i<pq_size(q); i++)
        {
            node_print(q->items[i]);
        }
    }
}
