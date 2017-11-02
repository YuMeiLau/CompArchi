.ORIG X3000
LEA R0, A
LEA R1, DATA
LEA R2, COUNT
LDW R0, R0, #0 ; R0 = X4000
LDW R1, R1, #0 ; R1 = 1
LDW R2, R2, #0 ; R2 = 20
STW R1, R0, #0 ; MEM[x4000] = 1;
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
HALT


A .FILL X4000
DATA .FILL #1
COUNT .FILL #20
DADDR .FILL XC000
.END
