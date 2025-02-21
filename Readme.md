- The classic CHIP-8 emulator, which can load programs into memory and execute them.
- Written in C and SDL for graphics (default screen 64x32 pixels).
- Has a debug mode in which you can see CPU and memory contents.
- The ROM folder contains a script that can generate a binary ROM file from an input text file.

Documentation for opcodes and general information: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

![image info](./assets/chip8.png)

# Installation and running

- You need `gcc`, `make` and `SDL2`.
- Simply clone the repo, compile it and run the emulator using a ROM file.

```
git clone https://github.com/SHOREQU69/CHIP-8-Emulator
cd CHIP-8-Emulator
make
./chip8 ./ROM/IBM_Logo.ch8
```

- To run using the debug mode run the
executable using the `-debug` flag.
- Supported operations in debug mode:
    - `cycle`: executes a single CPU cycle.  
    - `cpu`: prints CPU registers.  
    - `memory`: prints memory.  
    - `exit`: exit the emulator.  

```
git clone https://github.com/SHOREQU69/CHIP-8-Emulator
cd CHIP-8-Emulator
make
./chip8 ./ROM/IBM_Logo.ch8 -debug
```
