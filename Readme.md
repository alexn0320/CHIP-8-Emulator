# CHIP-8 Emulator
- The classic CHIP-8 emulator, made to learn how emulators and CPUs work.
- Written in C and SDL for graphics (made using Linux).
- Can load programs in memory from ROM files. 

![image info](./assets/chip8.png)
### Still work in progress (can only run the IBM logo)

# Installation and running

- You need `gcc`, `make` and `SDL2`.
- Simply clone the repo, compile it and run the emulator using a ROM file from the `ROM` folder.

```
git clone https://github.com/SHOREQU69/CHIP-8-Emulator
cd CHIP-8-Emulator
make
./chip8 ./ROM/IBM_Logo.ch8
```

- To run using the debug mode (in which you can see CPU info such as register values or memory), run the
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
