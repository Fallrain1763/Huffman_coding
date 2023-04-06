#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
 #include <sys/stat.h>
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "pq.h"
#include "stack.h"

#define OPTIONS "hi:o:v"

int main(int argc, char **argv)
{
	// default
	int infile = 0;
	int outfile = 1;
	bool print_stats = false;

	int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) 
	{
		switch (opt) 
		{
			case 'h':
					fprintf(stderr, "SYNOPSIS\n");
					fprintf(stderr, "  A Huffman encoder.\n");
					fprintf(stderr, "  Compresses a file using the Huffman coding algorithm.\n\n");
					fprintf(stderr, "USAGE\n");
					fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n\n");
					fprintf(stderr, "OPTIONS\n");
					fprintf(stderr, "  -h             Program usage and help.\n");
					fprintf(stderr, "  -v             Print compression statistics.\n");
					fprintf(stderr, "  -i infile      Input file to compress.\n");
					fprintf(stderr, "  -o outfile     Output of compressed data.\n");
					return EXIT_SUCCESS;
				break;
			case 'i':
					infile = open(optarg, O_RDONLY);
					if(infile == -1)
						return EXIT_FAILURE;
				break;
			case 'o':
					outfile = open(optarg, O_WRONLY|O_CREAT|O_TRUNC);
					if(outfile == -1)
						return EXIT_FAILURE;
				break;
			case 'v':
					print_stats = true;
				break;
			default:
					fprintf(stderr, "SYNOPSIS\n");
					fprintf(stderr, "  A Huffman encoder.\n");
					fprintf(stderr, "  Compresses a file using the Huffman coding algorithm.\n\n");
					fprintf(stderr, "USAGE\n");
					fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n\n");
					fprintf(stderr, "OPTIONS\n");
					fprintf(stderr, "  -h             Program usage and help.\n");
					fprintf(stderr, "  -v             Print compression statistics.\n");
					fprintf(stderr, "  -i infile      Input file to compress.\n");
					fprintf(stderr, "  -o outfile     Output of compressed data.\n");
					return EXIT_FAILURE;
		}
	}

	uint8_t buf[1];
	uint64_t hist[ALPHABET];
	// temp file for back to beginning of infile
	int temp_file = open("temp.txt", O_RDWR|O_CREAT);

	for(int i=0; i<ALPHABET; i++)
	{
		hist[i] = 0;
	}

	// build hist
	while(read_bytes(infile, buf, 1) != 0)
	{
		hist[buf[0]] += 1;
		// make a temp file
		write_bytes(temp_file, buf, 1);
	}

	// at least two symbols
	if(hist[0] == 0)
		hist[0] = 1;

	if(hist[1] == 0)
		hist[1] = 1;

	// build tree
	Node *huff_tree = build_tree(hist);

	// build code table
	Code table[ALPHABET];
	for(int i=0; i<ALPHABET; i++)
	{
		table[i] = code_init();
	}
	build_codes(huff_tree, table);

	// build header
	Header h;

	// Magic
	h.magic = MAGIC;

	// get permission of infile then set outfile
	struct stat status;
	fstat(infile, &status);
	h.permissions = status.st_mode;
	fchmod(outfile, h.permissions);

	// tree_size
	uint32_t count = 0;
	for(int i=0; i<ALPHABET; i++)
	{
		if(hist[i] != 0)
			count++;
	}
	h.tree_size = 3 * count - 1;

	// file_size
	h.file_size = bytes_read;

	// write Header
	write_bytes(outfile, (uint8_t *)&h, sizeof(Header));
	
	// write tree
	dump_tree(outfile, huff_tree);

	// write codes
	lseek(temp_file, 0, SEEK_SET);
	while(read_bytes(temp_file, buf, 1) != 0)
	{
		write_code(outfile, &table[buf[0]]);
	}
	flush_codes(outfile);

	// print stats
	if(print_stats)
	{	
		uint64_t uf_size = h.file_size;
		uint64_t cf_size = bytes_written-uf_size;
		double space_saving = 100 * (1 - (double)cf_size / (double)uf_size);

		fprintf(stderr, "Uncompressed file size: %lu bytes\n", uf_size);
		fprintf(stderr, "Compressed file size: %lu bytes\n", cf_size);
		fprintf(stderr, "Space saving: %.2f%%\n", space_saving);
	}
	
	close(temp_file);
	close(infile);
	close(outfile);

	delete_tree(&huff_tree);
	remove("temp.txt");

	return 0;
}
