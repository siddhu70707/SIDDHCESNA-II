# SIDDHCESNA-II - Hardware Implementation Notes
**Architecture Version:** 3.0 (Unified Bus)  
**Last Verified State:** Fibonacci Loop (Working)

---

### 1. ALU Routing Requirement
Every instruction that uses an ALU Opcode (Bits 13-10) **must** use the correct Source ID to drive the Unified Bus.
* **Source ID `100`:** `ALU_OUT`
* **Rule:** If the opcode is anything other than a simple register-to-register bypass, the Source field (Bits 09-07) must be `100`.
* **Example:** `MOV ADD, ALU_OUT, R3` -> `00 0000 100 011 0000` (`0230`)

---

### 2. Combinational Jump Logic
Jump flags are not stored in a register; they are wires directly connected to the **RA** and **RB** feeders.
* **The "Stale Flag" Problem:** If you calculate `R3 = RA + RB`, the flags now reflect the result in `R3`, not the loop condition.
* **The "Refresh" Solution:** You must reload **RA** and **RB** with the comparison values immediately before the Jump.
* **Refresh Sequence:**
    1. `MOV PASSA, R0, RA` (Set Current Value)
    2. `MOV PASSB, R2, RB` (Set Limit/Target Value)
    3. `JUMP <ADDR>` (Execute Jump immediately)

---

### 3. Component ID Map (3-bit)
| ID | Source (Read) | Destination (Write) |
| :--- | :--- | :--- |
| `000` | R0 | R0 |
| `001` | R1 | R1 |
| `010` | R2 | R2 |
| `011` | R3 | R3 |
| `100` | **ALU_OUT** | **RA** (Feeder A) |
| `101` | *N/A* | **RB** (Feeder B) |
| `110` | *N/A* | **CPU_OUT** (Display) |

---

### 4. Verified Fibonacci Hex String
**Constants:** `RAM[0]=0`, `RAM[1]=1`, `RAM[2]=20` (0x14)  
**Hex:** `C000 D001 E002 2060 2040 24D0 0230 2480 2210 2040 2550 A003 3C00`

---

### 5. Troubleshooting Checklist
* [ ] Does the `MOV` use Source `100` if the ALU is involved?
* [ ] Is the `LDM` pointing to the correct 12-bit RAM address?
* [ ] Are **RA** and **RB** refreshed in the two cycles before the `Type 10` Jump?
* [ ] Is the Jump Address correct for the current ROM alignment?