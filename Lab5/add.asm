.ORIG X3000

LEA R2, COUNT
LDW R2, R2, #0 ; R2 = 20
LEA R0, DADDR  
LDW R0, R0, #0 ; R0 = XC000
START   ADD R2, R2, #-1 
        BRN DONE
        LDB R3, R0, #0 ; R3 = data;
        ADD R4, R4, R3 ; accumulative adding
        ADD R0, R0, #1
        BR START

; R4 stores the results
; R0 = XC014
DONE STW R4, R0, #0

JMP R4 ; Protection Exception expected ; PC = R4 = 0x0052

HALT

COUNT .FILL #20
DADDR .FILL XC000
.END
