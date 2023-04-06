#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "io.h"

uint64_t bytes_read;
uint64_t bytes_written;

// read_bits
static uint8_t buffer[BLOCK];
static uint32_t offset = BLOCK * 8;
static uint32_t size;

// write_code
static uint8_t out_buffer[BLOCK];
static uint32_t out_offset = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes)
{
    int total_read = 0;
    int byte_read = 0;

    // if not read enough bytes, keep reading until read enough or reach eof
    do
    {
        byte_read = read(infile, buf+total_read, nbytes-total_read);
        total_read += byte_read;
    }while (total_read != nbytes && byte_read > 0);
    
    bytes_read += total_read;

    return total_read;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes)
{
    int total_write = 0;
    int byte_write = 0;

    // if not write enough bytes, keep writing until write enough or reach eof
    do
    {
        byte_write = write(outfile, buf+total_write, nbytes-total_write);
        total_write += byte_write;   
    } while (total_write != nbytes && byte_write > 0);
    
    bytes_written += total_write;

    return total_write;
}

bool read_bit(int infile, uint8_t *bit)
{
    // if reach the end of buffer, read next buffer, reset offset
    if(offset == BLOCK*8)
    {
        size = read_bytes(infile, buffer, BLOCK);
        offset = 0;
    }

    // if reach the size before reach the buffer end, there is no more
    if(offset == size*8)
        return false;

    uint32_t nbytes = offset/8;
    uint32_t nbits = offset%8;

    *bit = (buffer[nbytes] >> (nbits)) & 1;

    offset++;

    return true;
}

void write_code(int outfile, Code *c)
{
    uint32_t nbytes;
    uint32_t nbits;

    // go through the code array, put bits into buffer
    for(uint32_t i=0; i<code_size(c); i++)
    {
        
        nbytes = out_offset/8;
        nbits = out_offset%8;

        if(code_get_bit(c, i) == 1)
            out_buffer[nbytes] = (1 << nbits) | (out_buffer[nbytes]);
        else    
            out_buffer[nbytes] = (~(1 << nbits)) & (out_buffer[nbytes]);
        
        out_offset++;

        // if one block is full, write the block to outfile, reset offset
        if(out_offset == BLOCK*8)
        {
            write_bytes(outfile, out_buffer, BLOCK);
            out_offset = 0;
        }
    }
}

void flush_codes(int outfile)
{
    uint32_t size = out_offset/8;

    uint32_t extra_bits = out_offset%8;

    // if there is extra bits
    if(extra_bits != 0)
    {
        // zero rest of bits
        for(int i = extra_bits; i<8; i++)
        {
            out_buffer[size] = (~(1 << i)) & (out_buffer[size]);
        }

        //increase size
        size++;
    }
    
    write_bytes(outfile, out_buffer, size);
}
