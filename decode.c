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
					fprintf(stderr, "  A Huffman decoder.\n");
					fprintf(stderr, "  Decompresses a file using the Huffman coding algorithm.\n\n");
					fprintf(stderr, "USAGE\n");
					fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n\n");
					fprintf(stderr, "OPTIONS\n");
					fprintf(stderr, "  -h             Program usage and help.\n");
					fprintf(stderr, "  -v             Print compression statistics.\n");
					fprintf(stderr, "  -i infile      Input file to decompress.\n");
					fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
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
					fprintf(stderr, "  A Huffman decoder.\n");
					fprintf(stderr, "  Decompresses a file using the Huffman coding algorithm.\n\n");
					fprintf(stderr, "USAGE\n");
					fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n\n");
					fprintf(stderr, "OPTIONS\n");
					fprintf(stderr, "  -h             Program usage and help.\n");
					fprintf(stderr, "  -v             Print compression statistics.\n");
					fprintf(stderr, "  -i infile      Input file to decompress.\n");
					fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
					return EXIT_FAILURE;
		}
	}

	// Read Header
	Header h;
	uint32_t header_size = read_bytes(infile, (uint8_t *)&h, sizeof(Header));
	if(header_size < sizeof(Header))
	{
		fprintf(stderr, "Error: unable to read header.\n");
		return EXIT_FAILURE;
	}
	if(h.magic != MAGIC)
	{
		fprintf(stderr, "Invalid magic number.\n");
		return EXIT_FAILURE;
	}

	// set outfile permissions
	fchmod(outfile, h.permissions);

	// rebuild tree
	uint8_t buf[h.tree_size];
	read_bytes(infile, buf, h.tree_size);
	Node *huff_tree = rebuild_tree(h.tree_size, buf);

	// decompress file
	uint8_t bit;
	uint8_t buf1[1];
	Node *walker = huff_tree;
	while(bytes_written < h.file_size)
	{
		if(!(walker->left) && !(walker->right))
		{
			buf1[0] = walker->symbol;
			write_bytes(outfile, buf1, 1);
			walker = huff_tree;
		}
		else
		{
			read_bit(infile, &bit);
			if(bit == 0)
				walker = walker->left;
			else
				walker = walker->right;
		}
	}

	// print stats
	if(print_stats)
	{
		uint64_t cf_size = bytes_read;
		uint64_t df_size = bytes_written;
		double space_saving = 100 * (1 - (double)cf_size / (double)df_size);

		fprintf(stderr, "Compressed file size: %lu bytes\n", cf_size);
		fprintf(stderr, "Decompressed file size: %lu bytes\n", df_size);
		fprintf(stderr, "Space saving: %.2f%%\n", space_saving);
	}

	close(infile);
	close(outfile);

	delete_tree(&huff_tree);

	return 0;
}
