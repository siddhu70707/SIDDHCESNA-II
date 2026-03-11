# SIDDHCESNA - II Architecture

**SIDDHCESNA - II** is a custom 16-bit Instruction Set Architecture (ISA) designed for modular data flow and streamlined memory addressing. It utilizes a dual-format instruction system to balance internal register operations with external memory control.

---

## 1. Technical Specifications

| Feature | Specification |
| :--- | :--- |
| **Word Size** | 16-bit |
| **Address Space** | 12-bit (Up to 4,096 addresses) |
| **Register File** | 4 General Purpose (R0-R3) |
| **ALU Feeders** | 2 Dedicated (RA, RB) |
| **Arithmetic** | Unsigned Integer Only |

---

## 2. Instruction Formats

The CPU identifies the instruction type using the most significant bits (**Type**).

### Type 00: Register & ALU Operations
Used for arithmetic, logic, and moving data between registers and the ALU.

| Bit Range | Field | Description |
| :--- | :--- | :--- |
| **15 - 14** | `00` | **Type**: Universal ALU/Reg mode |
| **13 - 10** | `OPCODE` | 4-bit Operation Identifier |
| **09 - 07** | `SRC` | 3-bit Source Identifier |
| **06 - 04** | `DEST` | 3-bit Destination Identifier |
| **03 - 00** | `UNUSED` | Reserved for future expansion |

### Type 10: Control Flow & Memory
Used for conditional jumps and memory interfacing.

| Bit Range | Field | Description |
| :--- | :--- | :--- |
| **15 - 14** | `10` | **Type**: Control/Memory mode |
| **13 - 12** | `FLAG` | 2-bit Condition Flag |
| **11 - 00** | `ADDR` | 12-bit Target Memory Address |

---

## 3. Register & Component Mapping

### Source (3-bit)
| Value | Component |
| :--- | :--- |
| `000` | **R0** |
| `001` | **R1** |
| `010` | **R2** |
| `011` | **R3** |
| `100` | **ALU** (Result Output) |
| `101` | **MEMORY** (Load from RAM) |

### Destination (3-bit)
| Value | Component |
| :--- | :--- |
| `000` | **R0** |
| `001` | **R1** |
| `010` | **R2** |
| `011` | **R3** |
| `100` | **RA** (ALU Operand A) |
| `101` | **RB** (ALU Operand B) |

---

## 4. Instruction Set

### Opcodes (4-bit)
| Mnemonic | Code | Operation |
| :--- | :--- | :--- |
| **ADD** | `0000` | $RA + RB$ |
| **SUB** | `0001` | $RA - RB$ |
| **MUL** | `0010` | $RA \times RB$ |
| **DIV** | `0011` | $RA / RB$ |
| **NOT** | `0100` | Bitwise Invert RA |
| **AND** | `0101` | Bitwise AND |
| **OR** | `0110` | Bitwise OR |
| **XOR** | `0111` | Bitwise XOR |
| **STR** | `1000` | Store to memory |
| **HAL** | `1001` | Halt execution |

### Flags (2-bit)
Conditional jumps trigger based on these states:
* `00` (**Z**): Zero
* `01` (**C**): Carry
* `10` (**G**): Greater Than
* `11` (**S**): Small/Sign

---

## 5. Memory Logic

Memory operations are defined by the bit pattern at the first place:
* **Load (10):** Moves a value from RAM into the selected register.
* **Store (11):** Saves a register value to RAM (*Planned for v3.0*).

> **Note:** SIDDHCESNA - II does not support negative numbers. Operations resulting in values less than zero will result in an underflow or be capped at zero depending on implementation.
