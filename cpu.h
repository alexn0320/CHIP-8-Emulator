#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

/*
 *   The header and source files that implement the characteristics of the CHIP-8 CPU.
 */

#define DEBUG 1
// RAM size (4KB)
#define MEM_SIZE 4096
// stack size (should be avode 16)
#define STACK_SIZE 32
// starting address of the CHIP-8 program to be executed
#define PROG_START_ADDR 0x200
// starting address of the font to be used
#define FONT_START_ADDR 0x050

// sets a specified bit in a byte
#define SET_BIT(byte, pos) \
    (byte) = (byte) | (0x01 << (pos))

// create opcode from two bytes from memory
#define READ_OPCODE(byte1, byte2) \
    (((0x0000) + (byte1)) << 8) + (byte2)

// second nibble
#define X(opcode) \
    ((opcode) >> 8) & 0x000F

// third nibble
#define Y(opcode) \
    ((opcode) >> 4) & 0x000F

// forth nibble
#define N(opcode) \
    (opcode) & 0x000F

// second byte
#define NN(opcode) \
    (opcode) & 0x00FF

// second, thid, forth nibble
#define NNN(opcode) \
    (opcode) & 0x0FFF

// class that provides the functionality
class CPU
{

public:
    // RAM
    uint8_t memory[MEM_SIZE];
    // stack (16 entries or higher)
    uint16_t stack[STACK_SIZE];
    // registers
    uint8_t SP;
    uint16_t PC;
    uint16_t I;
    uint8_t DELAY_TIMER;
    uint8_t SOUND_TIMER;
    uint8_t V[16];

    CPU();
    // fetch/decode/execute
    void cycle();
    // debug registers
    void print_cpu();
};

#endif