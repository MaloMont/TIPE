gcc -O3 -c linreg.c -o linreg.o  -lm -g
gcc -O3 -c image_io.c -o image_io.o -lm -g
gcc -O3 -c range_blocks.c -o range_blocks.o -lm -g
gcc -O3 -c domain_blocks.c -o domain_blocks.o -lm -g
gcc -O3 -c decompression.c -o decompression.o -lm -g
gcc -O3 -c IFS.c -o IFS.o -lm -g
gcc -O3 -c main.c -o main.o -lm -g

gcc -O3 linreg.o image_io.o range_blocks.o domain_blocks.o decompression.o IFS.o main.o -o exe -lm -g
