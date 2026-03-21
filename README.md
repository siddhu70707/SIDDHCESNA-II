# SIDDHCESNA - II Architecture Specification
**Version:** 2.5  
**Type:** 16-bit Multi-cycle ISA  
**Status:** 90% Hardware Complete

---

## 1. System Overview
| Component | Specification |
| :--- | :--- |
| **Word Size** | 16-bit |
| **Address Space** | 12-bit (4,096 Words) |
| **Register File** | 4 General Purpose (R0-R3) |
| **ALU Feeders** | 2 Dedicated (RA, RB) |
| **Data Memory** | RAM (12-bit Address) |
| **Program Memory**| ROM (12-bit Address) |

---

## 2. Component Mapping (3-bit IDs)
These IDs drive the **Read Demux** (Sources) and **Write Demux** (Destinations) in the Datapath.

### Sources (Read Enable)
| ID | Mnemonic | Component |
| :--- | :--- | :--- |
| `000` | **R0** | General Purpose Register 0 |
| `001` | **R1** | General Purpose Register 1 |
| `010` | **R2** | General Purpose Register 2 |
| `011` | **R3** | General Purpose Register 3 |
| `100` | **ALU_OUT**| Combinational ALU Result |
| `101` | **MEMORY** | RAM Output (at current Address) |
| `110` | **OutputReg**| Logisim Hex Display / Output Port |
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
| `111` | **MEMORY** | RAM Input (Store at Address) |

---

## 3. Instruction Formats

### Type 00: Data Movement (MOVE)
`[15-14: 00] [13-10: UNUSED] [09-07: SRC] [06-04: DEST] [03-00: UNUSED]`
* Handles transfers between registers and memory.

### Type 01: Arithmetic & Logic (ALU)
`[15-14: 01] [13-10: OPCODE] [09-07: DEST] [06-00: UNUSED]`
* Processes values currently held in **RA** and **RB**.

### Type 10: Control Flow (JUMP)
`[15-14: 10] [13-12: FLAG_ID] [11-00: ADDR]`
* Conditional jumps to the 12-bit Program ROM address.

### Type 11: Immediate Load (LDI)
`[15-14: 11] [13-12: REG_ID] [11-00: VALUE]`
* Loads a 12-bit constant or address directly into a register.

---

## 4. Opcodes & Flags

### ALU Opcodes (Type 01)
| Mnemonic | Code | Operation |
| :--- | :--- | :--- |
| **ADD** | `0000` | $RA + RB$ |
| **SUB** | `0001` | $RA - RB$ |
| **MUL** | `0010` | $RA \times RB$ |
| **DIV** | `0011` | $RA / RB$ |
| **AND** | `0100` | $RA \text{ AND } RB$ |
| **OR** | `0101` | $RA \text{ OR } RB$ |
| **XOR** | `0110` | $RA \oplus RB$ |
| **NOT** | `0111` | $\text{NOT } RA$ |
| **PASS A**| `1000` | $Dest = RA$ |
| **PASS B**| `1001` | $Dest = RB$ |
| **HALT** | `1111` | Stop CPU Execution |

### Jump Flags (Type 10)
| Flag ID | Mnemonic | Condition |
| :--- | :--- | :--- |
| `00` | **JZ** | Jump if Zero ($RA = RB$) |
| `01` | **JGREATERA**| Jump if $RA > RB$ |
| `10` | **JSMALLERA**| Jump if $RA < RB$ |
| `11` | **JC** | Jump if Carry Flag is Set |

---

### Logic Summary
* **Address Handling:** Since addresses are 12 bits, use **Type 11** to load a memory address into a register. Use **Type 00** with `SRC 101` to pull data from that location into a destination register.
* **Siddhuceros Method:** Optimized for iterative addition and comparison using the dedicated `JGREATERA` and `JSMALLERA` flags.
