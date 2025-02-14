#ifndef CPU_H
#define CPU_H

#include "display.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <time.h>

/*
 *   The header and source files that implement the characteristics of the
 * CHIP-8 CPU.
 */

#define MEM_DEBUG 1
// RAM size (4KB)
#define MEM_SIZE 4096
// stack size (should be abode 16)
#define STACK_SIZE 32
// starting address of the CHIP-8 program to be executed
#define PROG_START_ADDR 0x200
// starting address of the font to be used
#define FONT_START_ADDR 0x050

// sets a specified bit in a byte
#define SET_BIT(byte, pos) (byte) = (byte) | (0x01 << (pos))

// create opcode from two bytes from memory
#define READ_OPCODE(byte1, byte2) (((0x0000) + (byte1)) << 8) + (byte2)

// second nibble
#define X(opcode) ((opcode) >> 8) & 0x000F

// third nibble
#define Y(opcode) ((opcode) >> 4) & 0x000F

// forth nibble
#define N(opcode) (opcode) & 0x000F

// second byte
#define NN(opcode) (opcode) & 0x00FF

// second, thid, forth nibble
#define NNN(opcode) (opcode) & 0x0FFF

//CPU elements
typedef struct
{
  BYTE memory[MEM_SIZE];    // RAM
  WORD stack[STACK_SIZE];   // stack (16 entries or higher)
  BYTE SP;                  // registers
  WORD PC;
  WORD I;
  BYTE DELAY_TIMER;
  BYTE SOUND_TIMER;
  BYTE V[16];

  disp d;                   //display and auxiliary flags
  BYTE running;
  BYTE draw_flag;
} cpu;

//stack related functions
void push(cpu *c, WORD val);
void pop(cpu *c);
void print_stack(cpu c);
//load a program in memory
BYTE loader(cpu *c, const char* path);
// init CPU values
void init_cpu(cpu *c, BYTE debug);
// fetch/decode/execute
void cycle(cpu *c);
//main loop
void run(const char* prog, BYTE debug);
// debug registers
void print_cpu(const cpu c);
void hexdump(const cpu c);

#endif
