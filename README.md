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
> **Format:** `[15-14: Type] [13-12: XX] [11-08: SRC] [07-04: DEST] [03-00: XXXX]`

### Type 01: Arithmetic & Logic (ALU)
Performs math on **RA** and **RB** and stores the result in a General Purpose Register.
> **Format:** `[15-14: Type] [13-10: OPCODE] [09-06: DEST] [05-00: XXXXXX]`

### Type 10: Control Flow (JUMP)
Updates the Program Counter (PC) based on flags.
> **Format:** `[15-14: Type] [13-12: FLAG] [11-00: ADDR]`

---

## 4. Opcodes & Flags

### ALU Opcodes (Type 01)
| Mnemonic | Code | Operation |
| :--- | :--- | :--- |
| **ADD** | `0000` | $Result = RA + RB$ |
| **SUB** | `0001` | $Result = RA - RB$ |
| **MUL** | `0010` | $Result = RA \times RB$ |
| **DIV** | `0011` | $Result = RA / RB$ |
| **NOT** | `0100` | $Result = \neg RA$ |
| **AND** | `0101` | $Result = RA \text{ AND } RB$ |
| **OR** | `0110` | $Result = RA \text{ OR } RB$ |
| **XOR** | `0111` | $Result = RA \oplus RB$ |
| **PASS A** | `1000` | $Result = RA$ |
| **PASS B** | `1001` | $Result = RB$ |
| **HALT** | `1111` | Stops CPU Execution |

### Status Flags
* **Z (Zero):** High if ALU result is 0.
* **C (Carry):** High if operation exceeds 16-bit unsigned limit.
* **G (Greater):** High if $RA > RB$.
* **S (Sign):** High if MSB of result is 1.

---

## 5. Implementation Notes
1. **Fetch/Execute:** The Control Unit uses a 2-step cycle. **T0** fetches the instruction into the IR. **T1** performs the internal move or math operation.
2. **ALU Dependency:** ALU operations require data to be pre-loaded into `RA` and `RB` using Type 00 instructions.
3. **RAM Interaction:** Since RAM is asynchronous, the Address Bus must be stable before the destination register latches the Data Bus on the next clock edge.