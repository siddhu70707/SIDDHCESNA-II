# SIDDHCESNA - II Architecture Specification
**Version:** 2.3  
**Type:** 16-bit Multi-cycle ISA  

---
README NOT RIGHT YET

## 1. System Overview
| Component | Specification |
| :--- | :--- |
| **Word Size** | 16-bit |
| **Address Space** | 12-bit (4,096 Words) |
| **Register File** | 4 General Purpose (R0-R3) |
| **ALU Feeders** | 2 Dedicated (RA, RB) |
| **RAM Type** | Asynchronous |

---

## 2. Component Mapping (3-bit IDs)
These IDs drive your **Read Demux** (Sources) and **Write Demux** (Destinations).

### Sources (Read Enable)
| ID | Mnemonic | Component |
| :--- | :--- | :--- |
| `000` | **R0** | General Purpose Register 0 |
| `001` | **R1** | General Purpose Register 1 |
| `010` | **R2** | General Purpose Register 2 |
| `011` | **R3** | General Purpose Register 3 |
| `100` | **ALU_OUT**| Combinational ALU Result |
| `101` | **MEMORY** | RAM Output (at Address Bus) |
| `110` | **OutputReg** | It acts like output in logisim |
| `111` | **UNUSED** | Reserved |

### Destinations (Write Enable)
| ID | Mnemonic | Component |
| :--- | :--- | :--- |
| `000` | **R0** | General Purpose Register 0 |
| `001` | **R1** | General Purpose Register 1 |
| `010` | **R2** | General Purpose Register 2 |
| `011` | **R3** | General Purpose Register 3 |
| `100` | **RA** | ALU Operand Feeder A |
| `101` | **RB** | ALU Operand Feeder B |
| `110` | **CPU_OUT**| External Output (Display/Port) |
| `111` | **MEMORY** | RAM Input (Store) |

---

## 3. Instruction Formats

### Type 00: Data Movement (MOVE)
`[15-14: 00] [13-10: UNUSED] [09-07: SRC] [06-04: DEST] [03-00: UNUSED]`

### Type 01: Arithmetic & Logic (ALU)
`[15-14: 01] [13-10: OPCODE] [09-07: DEST] [06-00: UNUSED]`
*Note: ALU operations always use RA and RB as inputs.*

### Type 10: Control Flow (JUMP)
`[15-14: 10] [13-12: FLAG] [11-00: ADDR]`

---

## 4. ALU Opcodes (Type 01)
| Mnemonic | Code | Operation |
| :--- | :--- | :--- |
| **ADD** | `0000` | $RA + RB$ |
| **SUB** | `0001` | $RA - RB$ |
| **MUL** | `0010` | $RA \times RB$ |
| **DIV** | `0011` | $RA / RB$ |
| **AND** | `0100` | $RA \text{ AND } RB$ |
| **OR** |  `0101` | $RA \text{ OR } RB$ |
| **XOR** | `0110` | $RA \oplus RB$ |
| **NOT** | `0111` | $\text{NOT } RA$ |
| **PASS A**| `1000` | Output = $RA$ |
| **PASS B**| `1001` | Output = $RB$ |
| **HALT** | `1111` | Stop Execution |

---

## 5. Micro-Operation Breakdown

* **Tick 0 (Fetch):**
    * PC $\to$ Address Bus.
    * RAM $\to$ Data Bus.
    * **IR_Write_Enable** = High.
* **Tick 1 (Execute):**
    * **Type 00:** Activate **Read Demux** (SRC) and **Write Demux** (DEST).
    * **Type 01:** Activate **ALU_OUT** Read and **Write Demux** (DEST).
    * **Type 10:** If Condition Met, **PC_Load** = High. Else, **PC_Inc** = High.