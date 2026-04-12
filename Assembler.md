# 🚀 SIDDHCESNA-II Assembler v1.4

**SIDDHCESNA-II** is a custom 16-bit Single-cycle Instruction Set Architecture (ISA). This repository contains the official Assembler written in **C++17**, designed to convert human-readable `.scesna` assembly files into machine-executable Binary and Hexadecimal code.

---

## 🏗 Architecture Specifications (v3.0)

The system operates on a **Unified Bus** principle where every operation—including arithmetic—is treated as a data move between a Source and a Destination.

### 1. Component Mapping (3-bit IDs)
| ID | Source (Read From) | Destination (Write To) |
| :--- | :--- | :--- |
| `000` | **R0** | **R0** |
| `001` | **R1** | **R1** |
| `010` | **R2** | **R2** |
| `011` | **R3** | **R3** |
| `100` | **ALU_OUT** | **RA** (ALU Feeder A) |
| `101` | **Reserved** | **RB** (ALU Feeder B) |
| `110` | **Reserved** | **CPU_OUT** (Display) |

### 2. ALU Opcodes (Bits 13-10)
| Opcode | Mnemonic | Operation |
| :--- | :--- | :--- |
| `0000` | **ADD** | $RA + RB$ |
| `0001` | **SUB** | $RA - RB$ |
| `1000` | **PASSA** | Pass **RA** to Bus |
| `1001` | **PASSB** | Pass **RB** to Bus |
| `1111` | **HALT** | Terminate Execution |

---

## 📜 Instruction Formats

### Type 00: Unified Bus Move
`[15-14: 00] [13-10: Opcode] [09-07: SRC] [06-04: DEST] [03-00: Unused]`  
* **Note:** To perform a standard move, the assembler defaults to the **PASSA** opcode.

### Type 01 & 11: Memory Operations
* **Type 11 (LOAD):** `[11] [Reg_ID] [12-bit RAM_Addr]` — Loads from RAM to Register.
* **Type 01 (STORE):** `[01] [Reg_ID] [12-bit RAM_Addr]` — Stores Register to RAM.

### Type 10: Control Flow (Jump)
`[10] [Flag_ID] [12-bit ROM_Addr]`  
* **JZ / JZERO:** Jump if $RA == RB$
* **JGREATER:** Jump if $RA > RB$
* **JSMALLER:** Jump if $RA < RB$

---

## 🛠 How to Use

### 1. Clone

clone the repo [SIDDHCESNA-II](https://github.com/siddhu70707/SIDDHCESNA-II)

### 2. Code

write your code in .scesna [syntax](https://github.com/siddhu70707/SIDDHCESNA-II/blob/main/LanguageDocumentation.md) next to the main.cpp file

### 3. Compilation
Compile the assembler using any C++17 compatible compiler:
```powershell
g++ main.cpp -o main