#include "cpu.h"

CPU::CPU()
{
    //initialising memory
    memset(memory, 0, MEM_SIZE);
    memset(stack, 0, STACK_SIZE);
    
    //register initialisation
    SP = 0;
    PC = PROG_START_ADDR;
    I = 0;
    DELAY_TIMER = 0;
    SOUND_TIMER = 0;

    for(uint8_t i = 0; i < 16; i++)
        V[i] = 0;

    //TODO: font init
    //TODO: display init
}


void CPU::cycle()
{
    //fetch
    uint16_t opcode = READ_OPCODE(memory[PC], memory[PC + 1]);
    PC += 2;
    //decode (macros are also used)
    uint16_t flag = opcode & 0xF000;

    //debug section
    #ifdef DEBUG
        printf("%04x %04x\n", opcode, flag);
    #endif

    //execute
    switch (flag)
    {
        //1NNN (jump)
        case 0x1000:
            PC = NNN(opcode);
            
            #ifdef DEBUG
                printf("1NNN: %03x\n", NNN(opcode));
            #endif
            break;
        
        //6XNN (set register VX)
        case 0x6000:
            V[X(opcode)] = NN(opcode);
            
            #ifdef DEBUG
                printf("6XNN: %02x %02x\n", X(opcode), NN(opcode));
            #endif
            break;

        //7XNN (Add the value NN to VX)
        case 0x7000:
            V[X(opcode)] += NN(opcode);
            
            #ifdef DEBUG
                printf("7XNN: %02x %02x\n", X(opcode), NN(opcode));
            #endif
            break;

        //ANNN (set index register I)
        case 0xA000:
            I = NNN(opcode);

            #ifdef DEBUG
                printf("ANNN: %03x\n", NNN(opcode));
            #endif
            break;
    }
}


void CPU::print_cpu()
{
    printf("----------\n");
    printf("REGISTERS:\n");
    printf("----------\n");

    for(uint8_t i = 0; i < 16; i++)
        printf("V%d: %02x\n", i, V[i]);

    printf("PC: %02x\nI: %02x\nSP: %02x\nD_TIMER: %02x\nS_TIMER: %02x\n", PC, I, SP, DELAY_TIMER, SOUND_TIMER);
    printf("----------\n");
}