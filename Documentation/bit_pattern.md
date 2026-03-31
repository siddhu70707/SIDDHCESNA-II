# SIDDHCESNA - II

**SIDDHCESNA - II** is a custom 16-bit RISC-style architecture designed for efficiency and iterative calculation. It features a unique **Unified Bus** design where arithmetic results and register moves are handled through a single 16-bit data path, optimized for fast conditional branching and modular expansion. 🛠️

---

## 🏗️ Architecture Specifications


| Component | Specification |
| :--- | :--- |
| **Word Size** | 16-bit |
| **Address Space** | 12-bit (4,096 Words) |
| **Register File** | 4 General Purpose (R0-R3) |
| **ALU Feeders** | 2 Dedicated Operands (RA, RB) |
| **Data Memory** | RAM (12-bit Addr / 16-bit Data) |
| **Program Memory** | ROM (12-bit Addr / 16-bit Inst) |
| **Output** | Integrated 7-Segment Display |

---

## 🚀 Key Features

*   **Unified Bus Move:** Logic and Math operations are executed during data moves, reducing the instruction count for complex algorithms.
*   **Dedicated ALU Feeders:** High-speed RA and RB registers act as direct inputs to the ALU, enabling combinational flag updates for Jumps.
*   **Hardware-Verified Logic:** Successfully executes iterative algorithms like the Fibonacci sequence using a robust comparison-based loop.

---

## 📑 Instruction Set Architecture (ISA)

The ISA uses a 2-bit prefix to determine the instruction type:


| Type | Prefix | Mnemonic | Description |
| :--- | :--- | :--- | :--- |
| **00** | `00` | **MOV** | ALU Operations & Bus Transfers |
| **01** | `01` | **STR** | Store Register value to RAM |
| **10** | `10` | **JUMP** | Conditional Branching (JZ, JGREATER, etc.) |
| **11** | `11` | **LDM** | Load value from RAM to Register |

> [!IMPORTANT]
> **Hardware Note:** To drive the 16-bit Unified Bus during any ALU operation (ADD, SUB, PASSA, etc.), the **Source ID** must be set to `100` (ALU_OUT).

---

## 📉 Hardware Verification: Fibonacci Test

The architecture has been verified using a Fibonacci sequence generator.

**Logic Flow:**
1.  **Initialize:** $RA = 0, RB = 1$.
2.  **Perform:** $RA + RB$ and output to `CPU_OUT`.
3.  **Store:** Result in `R0` (Temp).
4.  **Shift values:** $RA = RB, RB = R0$.
5.  **Compare:** Against a pre-loaded limit in `R1`.
6.  **JGREATERA:** Back to start if the limit is not reached.

**Result:** Successfully calculated and displayed `1, 2, 3, 5, 8, D` (Hex).

---

## 📂 Project Structure

*   `/Architecture_documentation`: Bit-pattern maps and component IDs.
*   `/HARDWARE`: `.circ` files for the CPU core and sub-circuits.
*   `/Documentation/Programs`: Hex code and assembly notes for verified programs.

---

## 💻 How to Run

1.  Download and install **Logisim-evolution**.
2.  Open `16bitcpu.circ`.
3.  Load the desired `.hex` file into the **ROM** component.
4.  Initialize required constants in **RAM** (e.g., addresses `0x000`–`0x002`).
5.  Enable **Ticks** and set frequency to **4Hz - 16Hz** to observe the 7-segment output.
