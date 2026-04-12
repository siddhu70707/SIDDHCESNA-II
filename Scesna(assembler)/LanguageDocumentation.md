## 📖 SCESNA-II Language Documentation

This section defines the syntax and programming model for the **SIddhcesna-II** Assembler.

### 1. Syntax Rules
* **Case Sensitivity:** Mnemonics (e.g., `MOV`, `ADD`) are case-insensitive. Register names (e.g., `R0`, `r0`) are also case-insensitive.
* **Comments:** Use `//` for single-line comments.
    * Example: `MOV R0, RA // This is a comment`
* **Labels:** Defined using the `@` prefix. Labels store the address of the instruction immediately following them.
    * Example: `@START`
* **Directives:** Use `#define` to map a name to a 12-bit RAM address or constant value.
    * Example: `#define TEMP 0x01F`

---

### 2. Instruction Set Reference

#### **Data Movement**
| Instruction | Syntax | Description |
| :--- | :--- | :--- |
| **MOV** | `MOV <src>, <dest>` | Moves data from Source to Destination via the Unified Bus. Defaults to `PASSA` opcode. |
| **LOAD** | `LOAD <reg>, <addr>` | Loads a 16-bit value from RAM address `<addr>` into `<reg>` (R0-R3). |
| **STORE** | `STORE <reg>, <addr>` | Stores a 16-bit value from `<reg>` into RAM address `<addr>`. |
| **OUT** | `OUT <reg>` | Shorthand for `MOV PASSA, <reg>, CPU_OUT`. Sends value to the 7-segment display. |

#### **Arithmetic & Logic**
*Note: All Math/Logic operations use values currently in **RA** and **RB** and must store the result from **ALU_OUT**.*

| Instruction | Syntax | Hardware Action |
| :--- | :--- | :--- |
| **ADD** | `ADD <dest>` | `dest = RA + RB` |
| **SUB** | `SUB <dest>` | `dest = RA - RB` |
| **MUL** | `MUL <dest>` | `dest = RA * RB` |
| **DIV** | `DIV <dest>` | `dest = RA / RB` |
| **NOT** | `NOT <dest>` | `dest = ~RA` |
| **HALT** | `HALT` | Stops the CPU clock and terminates program execution. |

#### **Control Flow (Jumps)**
Jumps are **absolute** to a 12-bit ROM address.
| Instruction | Syntax | Condition |
| :--- | :--- | :--- |
| **JZ / JZERO** | `JZ @label` | Jump if `RA == RB` |
| **JGREATER** | `JGREATER @label` | Jump if `RA > RB` |
| **JSMALLER** | `JSMALLER @label` | Jump if `RA < RB` |
| **JC** | `JC @label` | Jump if the Carry flag is set |

---

### 3. Programming Logic (The "Feeder" Pattern)
Because the ALU is strictly combinational based on **RA** and **RB**, you must follow this pattern for any calculation or comparison:

1.  **Setup:** Load your first operand into `RA`.
2.  **Setup:** Load your second operand into `RB`.
3.  **Execute:** Call the ALU instruction (e.g., `ADD R0`) or a Jump.

**Example: Adding two numbers stored in RAM**
```assembly
#define VAR1 0x001
#define VAR2 0x002

LOAD R0, VAR1
MOV R0, RA    // Prepare RA
LOAD R1, VAR2
MOV R1, RB    // Prepare RB
ADD R2        // R2 now contains VAR1 + VAR2