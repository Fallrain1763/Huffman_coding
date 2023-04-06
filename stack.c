#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct Stack
{
    uint32_t top;
    uint32_t capacity;
    Node **items;
};


Stack *stack_create(uint32_t capacity)
{
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if(s != NULL)
    {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) calloc(capacity, sizeof(Node *));
        if(!s->items)
        {
            free(s);
            s = NULL;
        }
    }

    return s;
}

void stack_delete(Stack **s)
{
    if(*s != NULL)
    {
        /*
        for(uint32_t i=0; i<(*s)->capacity; i++)
        {
            node_delete(&((*s)->items[i]));
        }
        */
        free((*s)->items);
        (*s)->items = NULL;

        free(*s);
        *s = NULL;
    }
}

bool stack_empty(Stack *s)
{
    return stack_size(s) == 0;
}

bool stack_full(Stack *s)
{
    return stack_size(s) == s->capacity;
}

uint32_t stack_size(Stack *s)
{
    return s->top;
}

bool stack_push(Stack *s, Node *n)
{
    if(stack_full(s))
        return false;
    else
    {
        // 0 1 2 3
        //         ^
        // 0 1 2 3 n
        //         ^
        // 0 1 2 3 n
        //           ^
        s->items[s->top] = n;
        s->top++;

        return true;
    }
}

bool stack_pop(Stack *s, Node **n)
{
    if(stack_empty(s))
        return false;
    else
    {
        // 0 1 2 3 4 5
        //             ^
        // 0 1 2 3 4 5
        //           ^
        // 0 1 2 3 4 
        //           ^
        s->top--;
        *n = s->items[s->top];

        return true;
    }
}

void stack_print(Stack *s)
{
    if(s == NULL)
        printf("NULL STACK\n");
    else
    {
        // go through and print
        for(uint32_t i=0; i<stack_size(s); i++)
        {
            node_print(s->items[i]);
        }
    }
}
