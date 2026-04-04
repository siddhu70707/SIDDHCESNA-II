#define 0, 0x000

@RESET
    LOAD r0, 0x000
    LOAD r1, 0x000
    LOAD r2, 0x000
    LOAD r3, 0x000
    LOAD rA, 0x000
    LOAD rB, 0x000
    JZERO , 0x007

// 0x007 is the starting of the program you want to run 7 thala for a reason