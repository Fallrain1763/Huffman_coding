CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: encode decode

encode: encode.o code.o huffman.o io.o node.o pq.o stack.o

decode: decode.o code.o huffman.o io.o node.o pq.o stack.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

spotless:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[c]
