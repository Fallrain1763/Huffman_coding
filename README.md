## Makefile.c
- make & make all : build the encoder and the decoder
- make encode : build just the encoder
- make decode : build just the decoder
- make clean : remove all files that are compiler generated
- make spotless : remove all files that are compiler generated as well as the executable.
- make format : format all source code

## DESIGN.pdf
- Design document which include the purpose and the pseudocode of all functions

## encode.c
- USAGE
-   ./encode [-h ] [-i infile] [-o outfile]
- OPTIONS
-   -h             Program usage and help.
-   -v             Print compression statistics.
-   -i infile      Input file to compress.
-   -o outfile     Output of compressed data.

## decode.c
- USAGE
-   ./decode [-h ] [-i infile] [-o outfile]
- OPTIONS
-   -h             Program usage and help.
-   -v             Print compression statistics.
-   -i infile      Input file to decompress.
-   -o outfile     Output of decompressed data.

## defines.h
- Contain the macro definitions used

## header.h
- Contain the struct definition for a file header

## node.h 
- Contain the node ADT interface

## node.c
- Contain implementation of the node ADT

## pq.h 
- Contain the priority queue ADT interface

## pq.c
- Contain implementation of the priority queue ADT

## code.h 
- Contain the code ADT interface

## code.c
- Contain implementation of the code ADT

## io.h
- contain the I/O module interface

## io.c
- contain implementation of the I/O module

## stack.h 
- Contain the stack ADT interface

## code.c
- Contain implementation of the stack ADT

## huffman.h
- contain the Huffman coding module interface

## huffman.c
- contain implementation of the Huffman coding module interface
