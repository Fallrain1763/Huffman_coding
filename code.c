#include <stdio.h>
#include "code.h"

Code code_init(void)
{
    Code c;
    
    c.top = 0;
    // init bit array with 0
    for(int i=0; i<MAX_CODE_SIZE; i++)
        c.bits[i] = 0;
    
    return c;
}

uint32_t code_size(Code *c)
{
    return c->top;
}

bool code_empty(Code *c)
{
    return code_size(c) == 0;
}

bool code_full(Code *c)
{
    return code_size(c) == ALPHABET;
}

bool code_set_bit(Code *c, uint32_t i)
{
    if(i > (ALPHABET-1))
        return false;
    else
    {
        // Left shift 1 (7-nbits) to find the ith bit from left, Then | num
        uint32_t nbytes = i/8;
        uint32_t nbits = i%8;
        c->bits[nbytes] = (1 << (7-nbits)) | (c->bits[nbytes]);
        
        return true;
    }
}

bool code_clr_bit(Code *c, uint32_t i)
{
    if(i > (ALPHABET-1))
        return false;
    else
    {
        // Left shift 1 (7-nbits) to find the ith bit from left, Flip all bits then & num
        uint32_t nbytes = i/8;
        uint32_t nbits = i%8;
        c->bits[nbytes] = (~(1 << (7-nbits))) & (c->bits[nbytes]);

        return true;
    }
}

bool code_get_bit(Code *c, uint32_t i)
{
    if(i > (ALPHABET-1))
        return false;
    else
    {
        uint32_t nbytes = i/8;
        uint32_t nbits = i%8;
        
        // Right shift num(7-nbit) to move the ith bits from left to right most , Then & 1
        return (c->bits[nbytes] >> (7-nbits)) & 1;    
    }
}

bool code_push_bit(Code *c, uint8_t bit)
{
    if(code_full(c))
        return false;
    else
    {
        // 0 1 1 1
        //         ^
        // 0 1 1 1 b
        //         ^
        // 0 1 1 1 b
        //           ^
        if(bit == 1)
            code_set_bit(c, c->top);
        else if(bit == 0)
            code_clr_bit(c, c->top);

        c->top++;

        return true;
    }
}

bool code_pop_bit(Code *c, uint8_t *bit)
{
    if(code_empty(c))
        return true;
    else
    {
        // 0 1 0 1 0 1 
        //             ^
        // 0 1 0 1 0 1
        //           ^
        // 0 1 0 1 0 
        //           ^
        c->top--;
        *bit = code_get_bit(c, c->top);

        return true;
    }
}

void code_print(Code *c)
{
    // go through and print
    for(uint32_t i=0; i<code_size(c); i++)
    {
        printf("%d ", code_get_bit(c, i));
        
        if(i%8 == 7 || i == code_size(c)-1)
            printf("\n");
    }
}
