# 📑 SIDDHCESNA-II: Architecture & Design Specification
**Project:** Siddhcesna-II  
**Architect:** Siddhant Kushwaha  
**Class:** 16-bit Custom RISC / Harvard Architecture  
**Transistor Budget:** ~8,000 - 12,000  

---

## 1. System Overview
The **SIDDHCESNA-II** is a high-efficiency, single-cycle processor designed for deterministic execution. By utilizing a **Unified Bus** and a **Non-Von Neumann (Harvard)** memory split, the architecture eliminates traditional bottlenecks found in entry-level 16-bit designs.

### Core Specifications
* **Word Size:** 16-bit
* **Address Space:** 12-bit (4,096 Words)
* **Execution Model:** Single-cycle (1 Instruction = 1 Clock)
* **Registers:** 4 General Purpose (R0-R3) + 2 Dedicated ALU Feeders (RA, RB)
* **Output:** Memory-mapped 7-segment display register (`CPU_OUT`)

---

## 2. Hardware Design Philosophy
### The "Feeder" Strategy
Unlike standard architectures that wire the Register File directly to the ALU, SIDDHCESNA-II uses **Dedicated Feeders (rA, rB)**. 
* **Benefit:** Reduces the number of read ports required on the main Register File.
* **Flow:** Data is moved to `rA`/`rB` first, and the ALU result is then captured from a virtual source (`ALU_OUT`).

### Combinational Flag Logic
The Jump flags (`JZ`, `JGREATER`, `JSMALLER`) are evaluated **combinationally** based on the current state of the ALU Feeders. 
* **Zero-Lag Branching:** There is no "Status Register" delay. The CPU decides whether to jump in the same cycle the comparison is made.

---

## 3. Engineering Trade-offs
In silicon design, every gain in one area results in a cost in another. Below are the strategic choices made for this architecture:

### A. Latency vs. Throughput (Single-Cycle vs. Pipeline)
* **Decision:** Single-cycle execution.
* **Gain:** **Absolute Determinism.** Perfect for real-time systems where instruction timing must be 100% predictable.
* **Cost:** **Lower Clock Ceiling.** The clock speed is limited by the "Longest Path" (ROM -> ALU -> Reg). However, we save ~4k transistors by omitting pipeline hazard logic.

### B. Instruction Density vs. Decoder Simplicity
* **Decision:** Fixed 16-bit instructions with no "Immediate" (embedded) values in Type 00.
* **Gain:** **Extremely Small Decoder.** The hardware logic required to "understand" an instruction is minimal, staying within our 12k transistor budget.
* **Cost:** **Constant Pre-loading.** Since values aren't in the code, they must be pre-loaded in RAM. This increases total program size but simplifies the silicon.

### C. Harvard Architecture vs. Complexity
* **Decision:** Separate Instruction (ROM) and Data (RAM) buses.
* **Gain:** **Elimination of the Von Neumann Bottleneck.** The CPU can fetch the next instruction while simultaneously reading/writing data.
* **Cost:** **Increased Pin Count.** Doubling the address and data buses increases the routing complexity on the FPGA.

---

## 4. Software Ecosystem
### SCESNA Assembler (C++17)
The architecture is supported by a custom-built, high-performance assembler.
* **Interactive CLI:** Provides a shell-like environment for developers.
* **Symbolic Resolution:** Supports `@LABEL` for branching and `#define` for constant management.
* **Multi-View Inspection:** Allows users to toggle between `HEX`, `BINARY`, and `SYMBOL TABLE` views to verify machine code integrity.

### Bootloader Logic
The system utilizes a dedicated `.sc` bootloader that:
1. Clears all general-purpose registers.
2. Flushes ALU feeders.
3. Performs a hard jump to `0x007` (The designated User Program Space).

---

## 5. Summary of System Constraints
| Feature | Implementation | Trade-off |
| :--- | :--- | :--- |
| **Constants** | RAM-based Pool | Saves Decoder gates / Uses more RAM |
| **Registers** | Synchronous/Clocked | Prevents Race Conditions / Limits frequency |
| **Addressing** | 12-bit Fixed | Simple 16-bit Opcode / Limits memory to 4K |
| **ALU Move** | Unified Bus | Fast math-to-reg moves / Requires manual feeder mgmt |

---
*Document for the SIDDHCESNA-II Research Project.*