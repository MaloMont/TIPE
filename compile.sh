gcc -c linreg.c -o linreg.o  -lm
gcc -c image_io.c -o image_io.o -lm
gcc -c range_blocks.c -o range_blocks.o -lm
gcc -c domain_blocks.c -o domain_blocks.o -lm
gcc -c main.c -o main.o -lm

gcc linreg.o image_io.o range_blocks.o domain_blocks.o main.o -o exe -lm
