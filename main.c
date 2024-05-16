#include "cpu.h"

int main(int argc, char **argv)
{
    if(argc == 2)
        run(argv[1], 0);
    else if(argc == 3 && !strcmp(argv[2], "-debug"))
        run(argv[1], 1);
    else
        printf("Usage: ./chip8 path-to-program\n");

    return 0;
}
