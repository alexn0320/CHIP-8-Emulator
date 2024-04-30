#include "cpu.h"

int main()
{
    CPU chip8;

    chip8.memory[chip8.PC] = 0x74;
    chip8.memory[chip8.PC + 1] = 0xF0;
    chip8.memory[chip8.PC + 2] = 0x74;
    chip8.memory[chip8.PC + 3] = 0x01;
    chip8.memory[chip8.PC + 4] = 0x12;
    chip8.memory[chip8.PC + 5] = 0x02;

    while(true)
    {
        int x;
        std::cin >> x;

        if(x == 1)
        {
            std::cout << "\e[1;1H\e[2J";
            chip8.cycle();
            chip8.cycle();
            chip8.print_cpu();
        }
        else
            break;
    }

    return 0;
}