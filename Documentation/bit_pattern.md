                                        SIDDHCESNA - II

[type - 2 bits][OPCODE 4 bits][source/reg/alu/memory - 3 bits ][destination - 3 bits][unused - 4 bits ]

Type is universal if 00 then the bt pattern at top will be followd of 10 then

[type - 2 bits][FLAG - 2 bits][memory address - 12 bits]

the bit at 1 place in the memory operation defines load or store if 10 load then the value from ram will load in register if 11 then store in memory and it will not be used thats a problem i faced out so it wil empty

source - 3 bits
R0 - 000
R1 - 001
R2 - 010
R3 - 011
ALU - 100
MEMORy - 101

destination - 3 bits

R0 - 000
R1 - 001
R2 - 010
R3 - 011
RA - 100 alu feeder feeds operand A
RB - 101 alu feeder feeds operand B
RAM - 110

OP CODES - 4 bits

ADD - 0000
SUB = 0001
MUL - 0010
DIV - 0011
NOT - 0100 inverts a
AND - 0101
OR  - 0110
XOR - 0111
STORE - 1000 - i have a question this stores the value in ram suppose i store alu's value in ram now i want to acces it how can i i dont have the memory addr or this is the assembler's job
HALT - 1001

FLAGS - 2 bits

Z - 00
c - 01
G - 10
S - 11

NOTE -
THE CPU DOES NOT SUPPORT NEGATIVE
JUMPS CAN ONLY HAPPEN IN 0 to 2^12 or 4096 MEMORY ADDRESS
SIZE OF WORD IS 16 BIT

