# SIDDHCESNA - II Architecture Specification
**Version:** 2.1  
**Type:** 16-bit Multi-cycle ISA  

---

## 1. System Overview
| Component | Specification |
| :--- | :--- |
| **Word Size** | 16-bit |
| **Address Space** | 12-bit (4,096 Words) |
| **Register File** | 4 General Purpose (R0-R3) |
| **ALU Feeders** | 2 Dedicated (RA, RB) |
| **RAM Type** | Asynchronous |

---

## 2. Register & Component Mapping (4-bit IDs)
These IDs are used in the **SRC** and **DEST** fields of instructions to route data across the main bus.

| ID | Mnemonic | Description |
| :--- | :--- | :--- |
| `0000` | **R0** | General Purpose Register 0 |
| `0001` | **R1** | General Purpose Register 1 |
| `0010` | **R2** | General Purpose Register 2 |
| `0011` | **R3** | General Purpose Register 3 |
| `0100` | **RA** | ALU Operand A (Input Only) |
| `0101` | **RB** | ALU Operand B (Input Only) |
| `0110` | **RAM** | Memory Access (via Address Bus) |
| `0111` | **ALU_OUT**| Combinational output of the ALU |

---

## 3. Instruction Formats

### Type 00: Data Movement (MOVE)
Copies data from a source component to a destination component.
> **Format:** `[15-14: Type] [11-08: SRC] [07-04: DEST]`

| Bit Range | Field | Description |
| :--- | :--- | :--- |
| **15 - 14** | `00` | Type: Data Movement |
| **13 - 12** | `XX` | Unused |
| **11 - 08** | `SRC` | 4-bit Source ID |
| **07 - 04** | `DEST` | 4-bit Destination ID |
| **03 - 00** | `XXXX` | Unused |

### Type 01: Arithmetic & Logic (ALU)
Performs math on **RA** and **RB** and stores the result in a General Purpose Register.
> **Format:** `[15-14: Type] [13-10: OPCODE] [09-06: DEST]`

| Bit Range | Field | Description |
| :--- | :--- | :--- |
| **15 - 14** | `01` | Type: ALU Operation |
| **13 - 10** | `OPCODE` | 4-bit Math Operation |
| **09 - 06** | `DEST` | 4-bit Destination ID (R0-R3) |
| **05 - 00** | `XXXXXX` | Unused |

### Type 10: Control Flow (JUMP)
Updates the Program Counter (PC) based on flags.
> **Format:** `[15-14: Type] [13-12: FLAG] [11-00: ADDR]`

| Bit Range | Field | Description |
| :--- | :--- | :--- |
| **15 - 14** | `10` | Type: Branching |
| **13 - 12** | `FLAG` | Condition: 00(Z), 01(C), 10(G), 11(Always) |
| **11 - 00** | `ADDR` | 12-bit Target Jump Address |

---

## 4. Opcodes & Flags

### ALU Opcodes (Type 01)
| Mnemonic | Code | Operation |
| :--- | :--- | :--- |
| **ADD** | `0000` | $Result = RA + RB$ |
| **SUB** | `0001` | $Result = RA - RB$ |
| **MUL** | `0010` | $Result = RA \times RB$ |
DIV
NOT
AND
OR
XOR
PASS A
PASS B
HALT
### Status Flags
* **Z (Zero):** High if ALU result is 0.
* **C (Carry):** High if operation exceeds 16-bit limit.
* **G (Greater):** High if $RA > RB$.
* **S (Sign):** High if MSB of result is 1 (Note: Hardware is unsigned).

---

## 5. Implementation Notes
1. **Fetch/Execute:** Each instruction requires a minimum of 2 clock cycles (T0: Fetch into IR, T1: Execute logic).
2. **ALU Usage:** Before calling any Type 01 instruction, the programmer must use Type 00 instructions to load values into `RA` and `RB`.
3. **RAM:** Since RAM is asynchronous, the data is available on the bus immediately when the address is set, but must be latched into a register on a clock edge.