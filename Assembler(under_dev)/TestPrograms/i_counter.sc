
#define 0, 0x000
#define 1, 0x001
#define 1000, 0x002

@INITIALISE
    LOAD r0, 0x000
    LOAD r1, 0x001
    LOAD r2, 0x002

@LOOP
    LOAD rA, r0
    LOAD rB, r1
    ADD r0
    OUT(r0)
    LOAD rA, r0
    LOAD rB, r2
    JSMALLER @LOOP

HALT
