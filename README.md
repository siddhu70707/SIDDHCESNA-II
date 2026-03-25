
# SIDDHCESNA - II Architecture Specification
**Version:** 3.0 (Unified Bus Edition)  
**Type:** 16-bit Single-cycle ISA  
**Status:** 100% Hardware Complete

---

## 1. System Overview
| Component | Specification |
| :---: | :--- |
| **Word Size** | 16-bit |
| **Address Space** | 12-bit (4,096 Words) |
| **Register File** | 4 General Purpose (R0-R3) |
| **ALU Feeders** | 2 Dedicated (RA, RB) |
| **Data Memory** | RAM (12-bit Addr / 16-bit Data) |
| **Program Memory** | ROM (12-bit Addr / 16-bit Inst) |
| **Output Register** | 7 segment display for output|

---

## 2. Component Mapping (3-bit IDs)
These IDs drive the **Source (Read)** and **Destination (Write)** buses. By mapping ALU results as Sources, math operations are handled via standard moves.

### Source IDs (Read From)
| ID | Mnemonic | Component / Result |
| :---: | :--- | :--- |
| `000` | **R0** | Register 0 |
| `001` | **R1** | Register 1 |
| `010` | **R2** | Register 2 |
| `011` | **R3** | Register 3 |
| `100` | **ALU_OUT** | ALU result |

### ALU Opcode Table (Bits 13-10)
The **OPCODE** field defines the operation performed by the ALU on operands **RA** and **RB**.

| Opcode | Mnemonic | Operation | Description |
| :---: | :--- | :--- | :--- |
| `0000` | **ADD** | $RA + RB$ | 16-bit Addition |
| `0001` | **SUB** | $RA - RB$ | 16-bit Subtraction |
| `0010` | **MUL** | $RA \times RB$ | 16-bit Multiplication |
| `0011` | **DIV** | $RA \div RB$ | 16-bit Division |
| `0100` | **NOT** | $\sim RA$ | Bitwise **NOT** (Operand A) |
| `0101` | **AND** | $RA \text{ \& } RB$ | Bitwise **AND** |
| `0110` | **OR** | $RA \mid RB$ | Bitwise **OR** |
| `0111` | **XOR** | $RA \oplus RB$ | Bitwise **XOR** |
| `1000` | **PASSA** | $RA$ | Pass **RA** to Bus (Bypass) |
| `1001` | **PASSB** | $RB$ | Pass **RB** to Bus (Bypass) |
| `1111` | **HALT** | **STOP** | Terminates CPU Execution |

*Note: Opcodes 1010 through 1110 are reserved for future expansion (e.g., Logical Shifts or Floating Point).*

### Destination IDs (Write To)
| ID | Mnemonic | Component |
| :---: | :--- | :--- |
| `000` | **R0** | Register 0 |
| `001` | **R1** | Register 1 |
| `010` | **R2** | Register 2 |
| `011` | **R3** | Register 3 |
| `100` | **RA** | ALU Operand Feeder A |
| `101` | **RB** | ALU Operand Feeder B |
| `110` | **CPU_OUT** | External Output (Display) |

*NOTE: CPU_OUT must be ENABLED if doing ALU operations*

---

## 3. Instruction Formats

### Type 00: Unified Bus Move (Logic & Math)
`[15-14: 00] [13-10: OPCODE] [09-07: SRC] [06-04: DEST] [03-00: UNUSED]`
* **Operation:** Executes the arithmetic or logic operation defined by the **OPCODE** on the values currently held in **RA** and **RB**, then moves data from the **Source ID** to the **Destination ID** across the 16-bit Unified Bus.
* **Example:** `MOV ALU_ADD, R0` adds RA and RB, then saves the result to R0.

### Type 01: Direct Store (STR)
`[15-14: 01] [13-12: REG_ID] [11-00: RAM_ADDR]`
* **Operation:** `RAM[ADDR] = Reg[ID]`
* **Action:** Stores a 16-bit register value into a specific 12-bit RAM address.

### Type 10: Control Flow (JUMP)
`[15-14: 10] [13-12: FLAG_ID] [11-00: ROM_ADDR]`
* **Operation:** Jumps to a 12-bit ROM address if the condition is met.

### Type 11: Direct Load (LDM)
`[15-14: 11] [13-12: REG_ID] [11-00: RAM_ADDR]`
* **Operation:** `Reg[ID] = RAM[ADDR]`
* **Action:** Loads a 16-bit value from a 12-bit RAM address into a register.

---

## 4. Jump Flags (Type 10)
| Flag ID | Mnemonic | Condition |
| :---: | :--- | :--- |
| `00` | **JZ** | Jump if Zero ($RA = RB$) |
| `01` | **JGREATERA** | Jump if $RA > RB$ |
| `10` | **JSMALLERA** | Jump if $RA < RB$ |
| `11` | **JC** | Jump if Carry Flag is Set |

*NOTE: 1. Every flag depends upon the RA and RB flags updates combinationaly
2. No Register is Combnational*
---

## 5. Implementation Summary
* **Efficiency:** Combined ALU and MOVE logic reduces instruction complexity.
* **Memory:** Dedicated Load/Store types provide clean 12-bit addressing without needing pointers.
* **Siddhuceros-II Goal:** Optimized for fast iterative addition and conditional branching.


U have to preload the constants in memory and use these above operation to make the in use 