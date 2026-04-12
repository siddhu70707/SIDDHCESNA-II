# 🚀 SIDDHCESNA - II Architecture

**Version:** 3.0 (Unified Bus Edition)  
**Type:** 16-bit Single-cycle ISA  
**Status:** 100% Hardware Complete  

---

## 📖 Quick Links
- ⚙️ Assembler Source [Scesna doc](https://github.com/siddhu70707/SIDDHCESNA-II/blob/main/Assembler.md)
- 📜 Language Documentation [Language documentation](https://github.com/siddhu70707/SIDDHCESNA-II/blob/main/LanguageDocumentation.md)
- 🏗️ Architecture Specification [Architecture](https://github.com/siddhu70707/SIDDHCESNA-II/blob/main/Architecture.md)
- 🔨 hardware [16-bit-CPU](https://github.com/siddhu70707/SIDDHCESNA-II/blob/main/Hardware/16bitcpu.circ)
- 👩‍💻 Code [Assembler](https://github.com/siddhu70707/SIDDHCESNA-II/blob/main/Scesna(assembler)/main.cpp)
---

## 1. System Overview

| Component        | Specification                          |
|------------------|----------------------------------------|
| Word Size        | 16-bit                                 |
| Address Space    | 12-bit (4,096 Words)                  |
| Register File    | 4 General Purpose (R0–R3)             |
| ALU Feeders      | 2 Dedicated (RA, RB)                  |
| Data Memory      | RAM (12-bit Addr / 16-bit Data)       |
| Program Memory   | ROM (12-bit Addr / 16-bit Inst)       |
| Output Register  | 7-segment display for output          |

---

## 2. Component Mapping (3-bit IDs)

These IDs drive the Source (Read) and Destination (Write) buses.  
By mapping ALU results as Sources, math operations are handled via standard moves.

### Source IDs (Read From)

| ID  | Mnemonic | Component / Result |
|-----|----------|------------------|
| 000 | R0       | Register 0       |
| 001 | R1       | Register 1       |
| 010 | R2       | Register 2       |
| 011 | R3       | Register 3       |
| 100 | ALU_OUT  | ALU result       |

---

## ALU Opcode Table (Bits 13–10)

The OPCODE field defines the operation performed by the ALU on operands RA and RB.

| Opcode | Mnemonic | Operation | Description                |
|--------|----------|----------|----------------------------|
| 0000   | ADD      | RA + RB  | 16-bit Addition            |
| 0001   | SUB      | RA − RB  | 16-bit Subtraction         |
| 0010   | MUL      | RA × RB  | 16-bit Multiplication      |
| 0011   | DIV      | RA ÷ RB  | 16-bit Division            |
| 0100   | NOT      | ∼RA      | Bitwise NOT (Operand A)    |
| 0101   | AND      | RA & RB  | Bitwise AND                |
| 0110   | OR       | RA ∣ RB  | Bitwise OR                 |
| 0111   | XOR      | RA ⊕ RB  | Bitwise XOR                |
| 1000   | PASSA    | RA       | Pass RA to Bus (Bypass)    |
| 1001   | PASSB    | RB       | Pass RB to Bus (Bypass)    |
| 1111   | HALT     | STOP     | Terminates CPU Execution   |

---

### Destination IDs (Write To)

| ID  | Mnemonic | Component              |
|-----|----------|------------------------|
| 000 | R0       | Register 0             |
| 001 | R1       | Register 1             |
| 010 | R2       | Register 2             |
| 011 | R3       | Register 3             |
| 100 | RA       | ALU Operand Feeder A   |
| 101 | RB       | ALU Operand Feeder B   |
| 110 | CPU_OUT  | External Output        |

---

## 3. Instruction Formats

### Type 00: Unified Bus Move (Logic & Math)

**Operation:**  
Executes the arithmetic or logic operation defined by the OPCODE on the values currently held in RA and RB, then moves data from the Source ID to the Destination ID across the 16-bit Unified Bus.

**Example:**  
`MOV ALU_ADD, R0` → Adds RA and RB, then saves the result to R0.

---

### Type 01: Direct Store (STR)

**Operation:**  
`RAM[ADDR] = Reg[ID]`

**Action:**  
Stores a 16-bit register value into a specific 12-bit RAM address.

---

### Type 10: Control Flow (JUMP)

**Operation:**  
Jumps to a 12-bit ROM address if the condition is met.

---

### Type 11: Direct Load (LDM)

**Operation:**  
`Reg[ID] = RAM[ADDR]`

**Action:**  
Loads a 16-bit value from a 12-bit RAM address into a register.

---

## 4. Jump Flags (Type 10)

| Flag ID | Mnemonic   | Condition                |
|--------|------------|--------------------------|
| 00     | JZ         | Jump if Zero (RA = RB)   |
| 01     | JGREATERA  | Jump if RA > RB          |
| 10     | JSMALLERA  | Jump if RA < RB          |
| 11     | JC         | Jump if Carry Flag is Set|

---

## ⚠️ Hardware Constraints & Rules

- **ALU Operations:**  
  You must use `ALU_OUT` as the source for any ALU operation, including `PASSA` and `PASSB`.

- **Combinational Logic:**  
  All flags depend upon the RA and RB inputs; they update combinationally.

- **Registers:**  
  No register is combinational.

- **Constants:**  
  There are no immediate values in instructions.  
  You must preload constants into memory and use the `LDM` (Load Direct) operation.

---

## 🎯 Implementation Goals

- **Efficiency:**  
  Combined ALU and MOVE logic reduces instruction complexity and cycles.

- **Memory Optimization:**  
  Dedicated Load/Store types provide clean 12-bit addressing without complex pointer arithmetic.

- **Purpose:**  
  Optimized for high-speed iterative addition and complex conditional branching.