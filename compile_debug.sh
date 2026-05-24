gcc -Wall -Wextra -c linreg.c -o linreg.o  -lm -g -pg --sanitize=address
gcc -Wall -Wextra -c image_io.c -o image_io.o -lm -g -pg --sanitize=address
gcc -Wall -Wextra -c range_blocks.c -o range_blocks.o -lm -g -pg --sanitize=address
gcc -Wall -Wextra -c domain_blocks.c -o domain_blocks.o -lm -g -pg --sanitize=address
gcc -Wall -Wextra -c main.c -o main.o -lm -g -pg --sanitize=address

gcc -Wall -Wextra linreg.o image_io.o range_blocks.o domain_blocks.o main.o -o exe -lm -g -pg --sanitize=address
