# 📖 SCESNA-II Language Documentation (v3.2)

This document defines the syntax and hardware-interaction model for the **SIDDHCESNA-II processor**.

---

## 1. Instruction Set Architecture (ISA)

### 🧩 Data Movement

| Instruction | Syntax              | Opcode     | Description |
|------------|--------------------|------------|-------------|
| MOV        | `MOV <src> <dest>` | 1000 / 1001 | Moves data via Unified Bus. Uses PASSA or PASSB based on source. |
| LOAD       | `LOAD <reg> <addr>`| 11         | Loads 16-bit value from RAM address into R0–R3. |
| STORE      | `STORE <reg> <addr>`| 11        | Stores 16-bit value from R0–R3 into RAM address. |
| OUT        | `OUT <src>`        | 1000       | Sends `<src>` data to the display port. |

---

### ⚙️ Arithmetic & Control

> **Note:** Operations are performed on the values currently latched in **RA** and **RB**.

| Instruction | Syntax           | Hardware Action |
|------------|------------------|-----------------|
| ADD        | `ADD <dest>`     | `dest = RA + RB` (Result from ALU_OUT) |
| SUB        | `SUB <dest>`     | `dest = RA - RB` (Result from ALU_OUT) |
| JSMALLER   | `JSMALLER @label`| Jump to ROM address if `RA < RB` |
| HALT       | `HALT`           | Opcode `1111`. Stops the CPU clock. |

---

## 2. 🔁 The "Feeder" Programming Pattern

The **SIDDHCESNA-II** uses a strict **setup-before-execution** flow.

Because the ALU is fed by dedicated registers (**RA** and **RB**), you must *feed* them before performing math or comparisons.

### Pattern:

1. **Feed RA**  
   Move the first operand into `RA`.

2. **Feed RB**  
   Move the second operand into `RB`.

3. **Execute**  
   Run the ALU or jump instruction.

---

## 3. 🧠 Logic Example: Adding Two Numbers

Add two numbers stored in RAM addresses `0x001` and `0x002`.

### Code Snippet

```asm
#define VAR1 0x001
#define VAR2 0x002

// 1. Load data from RAM to GP Registers
LOAD R0 VAR1
LOAD R1 VAR2

// 2. Feed the ALU
MOV R0 RA    // Feed RA (F(n-2) path)
MOV R1 RB    // Feed RB (F(n-1) path - triggers PASSB)

// 3. Execute and store
ADD R3       // R3 = RA + RB (Hex: 0230)
OUT R3       // Send result to display

HALT