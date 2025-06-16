# SimpleCPU - 4-bit CPU Simulator

A minimalist 4-bit CPU simulator written in C++ for educational purposes.  
It demonstrates core CPU design principles such as instruction fetching, decoding, register-based execution, and memory
access.

Emphasis is placed on simplicity and clarity over hardware realism. The goal is to illustrate how a CPU interprets and
executes a program.

Due to the limited memory (16 words), the complexity of programs is constrained. However, adding instructions for I/O or
interaction with external devices would expand its capabilities significantly.
For example, it could simulate a network of embedded devices making routing decisions or responding to sensor input.
This feature is not implemented in this version, as the focus is shifting toward building a more capable 8-bit CPU.
Still, the design leaves room for future expansion.

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
./cpu4bitsim   # Runs built-in tests automatically
```

## Architecture Overview

- **Registers**:
    - 2 General-purpose registers: `RegisterA`, `RegisterB`
    - Simulated as 8-bit integers in code for convenience

- **Memory**:
    - 16 addressable words (0x0–0xF), each 4 bits

- **ALU**:
    - Supports `ADD` and `SUB`, result always stored in `RegisterA`

- **Flags**:
    - `Zero`, `Negative`, `Overflow`, implemented as separate boolean values

- **Internal Registers**:
    - Instruction Store (IS)
    - Program Counter (PC)
    - ALU Result Buffer (for realism, though only one-cycle ops)

- **Architecture Style**:
    - Von Neumann architecture (shared instruction/data memory)

## Operation

On startup the CPU runs the instruction at memory address 0x0. It then increments the Program Counter (PC) until a
`Halt` instruction (OpCode:0) is reached.

## Instruction Set

Each instruction consists of a 4-bit opcode. Some require additional 4-bit arguments on the following memory line.

| Name   | #  | Code   | Args (4-bit) | Description                    |
|--------|----|--------|--------------|--------------------------------|
| Halt   | 00 | `0000` | -            | Stop execution                 |
| LoadA  | 01 | `0001` | Src Address  | Load value from memory into A  |
| LoadIA | 02 | `0010` | Value        | Load intermediate value into A |
| LoadB  | 03 | `0011` | Src Address  | Load value from memory into B  |
| StoreA | 04 | `0100` | Dest Address | Store A into memory            |
| Mov    | 05 | `0101` | Reg0, Reg1   | Copy Reg0 → Reg1               |
| Add    | 06 | `0110` | Reg0, Reg1   | A = Reg0 + Reg1                |
| Sub    | 07 | `0111` | Reg0, Reg1   | A = Reg0 - Reg1                |
| Jump   | 08 | `1000` | Ins Address  | Set PC to address              |
| JumpZ  | 09 | `1001` | Ins Address  | Jump if ALU result == 0        |
| JumpNZ | 10 | `1010` | Ins Address  | Jump if ALU result != 0        |

> **Reg0/Reg1 Encoding**:  
> `00` = A, `01` = B (e.g., `0001` = RegA, RegB)

## Sample Program

The following program loads 5 into register A (A) and 2 into register B (B). It then adds the registers for a sum of 7
and places this sum into memory location 0xE. The value of A is saved into 0xD to preserve it before addition.

```assembly
0x0 0010   ; LoadIA
0x1 0010   ; Arg. A ← 2
0x2 0101   ; Mov
0x3 0001   ; Arg. B ← A
0x4 0010   ; LoadIA
0x5 0101   ; Arg. A ← 5
0x6 0100   ; Store A
0x7 1101   ; Arg. Mem[0xD] ← A
0x8 0110   ; Add
0x9 0001   ; Arg. A + B = 7 → A
0xA 0100   ; Store A
0xB 1110   ; Arg. Mem[0xE] ← A
0xC 0000   ; Halt

; Data section after execution.
; 0xD 0101  ; Value of A saved before addition. 
; 0xE 0111  ; Result location
```
