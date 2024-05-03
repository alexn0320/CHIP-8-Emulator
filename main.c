#include "cpu.h"

int main(int argc, char **argv)
{
    if(argc == 2)
        run(argv[1]);
    else
        printf("Usage: ./chip8 path-to-program\n");

    return 0;
}
