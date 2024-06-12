#include "cpu.h"
#include "display.h"
#include <stdio.h>
#include <time.h>

static void sleep(int milliseconds)
{
    struct timespec time;

    time.tv_sec = milliseconds / 1000;
    time.tv_nsec = milliseconds * 1000000;

    nanosleep(&time, &time);
}

void push(cpu *c, WORD val)
{
    c->stack[c->SP++] = val;
}

void pop(cpu *c)
{
    if(c->SP > 0)
    {
        c->stack[c->SP] = 0x0000;
        c->SP--;
    }
}

void print_stack(cpu c)
{
    printf("------\n");
    printf("STACK:\n");
    printf("------\n");

    for(BYTE i = 0; i < c.SP; i++)
        printf("%02x\n", c.stack[i]);

    printf("------\n");
}

BYTE loader(cpu *c, const char* path)
{
    FILE *rom = fopen(path, "rb");
    char b;
    uint32_t size = 0;

    if(rom == NULL)
        return 0;

    //get the size in bytes of the file
    fseek(rom, 0, SEEK_END);
    size = ftell(rom);
    rewind(rom);

    //read the ROM file byte by byte
    for(uint32_t i = 0; i < size; i++)
    {
        fread(&b, 1, 1, rom);
        c->memory[PROG_START_ADDR + i] = b;
    }

    fclose(rom);
    return 1;
}

void init_cpu(cpu *c, BYTE debug)
{
    //initialising memory
    memset(c->memory, 0, MEM_SIZE);
    memset(c->stack, 0, STACK_SIZE);

    //register initialisation
    c->SP = 0;
    c->PC = PROG_START_ADDR;
    c->I = 0;
    c->DELAY_TIMER = 0;
    c->SOUND_TIMER = 0;

    for(uint8_t i = 0; i < 16; i++)
        c->V[i] = 0;

    //TODO: font init

    //if debug mode, do not open screen
    if(!debug)
        init_disp(&c->d);

    c->running = 1;
}


void cycle(cpu *c)
{
    //fetch
    WORD opcode = READ_OPCODE(c->memory[c->PC], c->memory[c->PC + 1]);
    //decode (macros are also used)
    WORD flag = opcode & 0xF000;
    c->PC += 2;

    //debug section
    #ifdef DEBUG
        printf("%04x\n", opcode);
    #endif

    //execute
    switch (flag)
    {
        //1NNN (jump)
        case 0x1000:
            c->PC = NNN(opcode);

            #ifdef DEBUG
                printf("1NNN: %03x\n", NNN(opcode));
            #endif
            break;

        //6XNN (set register VX)
        case 0x6000:
            c->V[X(opcode)] = NN(opcode);

            #ifdef DEBUG
                printf("6XNN: %02x %02x\n", X(opcode), NN(opcode));
            #endif
            break;

        //7XNN (Add the value NN to VX)
        case 0x7000:
            c->V[X(opcode)] += NN(opcode);

            #ifdef DEBUG
                printf("7XNN: %02x %02x\n", X(opcode), NN(opcode));
            #endif
            break;

        //ANNN (set index register I)
        case 0xA000:
            c->I = NNN(opcode);

            #ifdef DEBUG
                printf("ANNN: %03x\n", NNN(opcode));
            #endif
            break;

        //DXYN (draws an N pixels tall sprite at position (VX, VY))
        case 0xD000:
        {
            //fetch values from registers and opcode
            BYTE x = c->V[X(opcode)];
            BYTE y = c->V[Y(opcode)];
            BYTE n = N(opcode);

            //reset flags
            c->V[0xF] = 0;

            //get each row from the sprite
            for(BYTE i = 0; i < n; i++)
            {
                BYTE row = c->memory[c->I + i];
                BYTE mask = 1 << 7;

                //render each pixel individually
                for (BYTE j = 0; j < 8; j++)
                {
                    //if the pixel was already on, set flags and turn in off
                    if(row & mask && c->d.gfx[y * DISP_WIDTH + x])
                    {
                        c->d.gfx[y * DISP_WIDTH + x] = 0;
                        c->V[0xF] = 1;
                    }
                    //turn the pixel on
                    else if(row & mask && !c->d.gfx[y * DISP_WIDTH + x])
                        c->d.gfx[y * DISP_WIDTH + x] = 1;

                    mask >>= 1;
                    x++;
                }

                x = c->V[X(opcode)];;
                y++;
            }

            #ifdef DEBUG
                printf("DXYN: %01x %01x %01x\n", X(opcode), Y(opcode), N(opcode));
            #endif

            c->draw_flag = 1;

            break;
        }

        //call subroutine
        case 0x2000:
            push(c, c->PC);
            c->PC = NNN(opcode);

            #ifdef DEBUG
                printf("2NNN: %03x\n", NNN(opcode));
            #endif

            break;

        //return from subroutine
        case 0x0000:

            //Clear screen
            if((Y(opcode)) == 0xE && (N(opcode)) == 0x0)
            {
                for(uint32_t i = 0; i < DISP_WIDTH * DISP_HEIGHT; i++)
                    c->d.gfx[i] = 0;

                return;
            }
            else if((Y(opcode)) == 0xE && (N(opcode)) == 0xE)
                return;

            c->PC = c->stack[c->SP - 1];
            pop(c);

            #ifdef DEBUG
                printf("00EE %03x\n", c->PC);
            #endif

            break;

        case 0x3000:
            if(c->V[X(opcode)] == (NN(opcode)))
                c->PC += 2;

            #ifdef DEBUG
                printf("3XNN %03x %03x\n", X(opcode), NN(opcode));
            #endif

            break;

        case 0x4000:
            if(c->V[X(opcode)] != (NN(opcode)))
                c->PC += 2;

            #ifdef DEBUG
                printf("4XNN %03x %03x\n", X(opcode), NN(opcode));
            #endif

            break;

        case 0x5000:
            if(c->V[X(opcode)] == c->V[Y(opcode)])
                c->PC += 2;

            #ifdef DEBUG
                printf("5XY0 %03x %03x\n", X(opcode), Y(opcode));
            #endif

            break;

        case 0x9000:
            if(c->V[X(opcode)] != c->V[Y(opcode)])
                c->PC += 2;

            #ifdef DEBUG
                printf("9XY0 %03x %03x\n", X(opcode), Y(opcode));
            #endif

            break;
    }
}

void run(const char* prog, BYTE debug)
{
    //TODO timers

    cpu chip8;
    init_cpu(&chip8, debug);

    //load the a ROM file in memory
    if(!loader(&chip8, prog))
    {
        printf("Failed to load program\n");
        return;
    }

    //debug
    if(debug == 1)
    {
        char comm[128];

        printf("DEBUG MODE\n");

        while(1)
        {
            scanf("%s", comm);
            printf("\n");

            if(!strcmp(comm, "cycle"))
                cycle(&chip8);
            else if(!strcmp(comm, "cpu"))
                print_cpu(chip8);
            else if(!strcmp(comm, "memory"))
                hexdump(chip8);
            else if(!strcmp(comm, "exit"))
                break;
        }

        return;
    }

    BYTE instructions = 0;

    //main loop
    while(chip8.running)
    {
        //fetch/decode/execute instruction
        cycle(&chip8);

        //get input
        disp_events(&chip8.running);

        //render screen
        if(chip8.draw_flag)
        {
            render(&chip8.d);
            chip8.draw_flag = 0;
        }

        //540Hz / 9 = 60Hz frequency for the timers
        if(instructions == 9)
        {
            if(chip8.DELAY_TIMER > 0)
                chip8.DELAY_TIMER--;

            if(chip8.SOUND_TIMER > 0)
                chip8.SOUND_TIMER--;

            instructions = 0;
        }
        else
            instructions++;

        //1850 milliseconds ~ 540Hz frequency for the CPU
        sleep(1850);
    }

    disp_close(&chip8.d);
}

void print_cpu(const cpu c)
{
    printf("----------\n");
    printf("REGISTERS:\n");
    printf("----------\n");

    for(BYTE i = 0; i < 16; i++)
        printf("V%d: %02x\n", i, c.V[i]);

    printf("PC: %02x\nI: %02x\nSP: %02x\nD_TIMER: %02x\nS_TIMER: %02x\n", c.PC, c.I, c.SP, c.DELAY_TIMER, c.SOUND_TIMER);
    printf("----------\n");
}

void hexdump(cpu c)
{
    printf("-------\n");
    printf("MEMORY:\n");
    printf("-------\n");

    //printing 10 bytes per row
    for(uint32_t i = 0; i < MEM_SIZE; i++)
    {
        if(i > 0 && i % 10 == 0)
            printf("\n");

        if(i == 0 || i % 10 == 0)
            printf("0x%03x: ", i);

        printf("%02x ", c.memory[i]);
    }

    printf("-------\n");
}
