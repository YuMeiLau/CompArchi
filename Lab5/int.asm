.ORIG X1200
STW R2, R6, #-1
STW R1, R6, #-2
STW R0, R6, #-3
ADD R6, R6, #-6
LEA R0, PTBR
LDW R0, R0, #0 ; R0 = PTBR;
LEA R1, COUNT
LDW R1, R1, #0 ; R1 = 128
START 	ADD R1, R1, #-1
		BRN DONE
		LDW R2, R0, #0 ; R2 = PTE
		RSHFA R2, R2, #1 ; ignore the last bit
		LSHF R2, R2, #1 ; set PTE R to 0
		STW R2, R0, #0
		ADD R0, R0, #2
		BR START

DONE LDW R0, R6, #0 ; maintain Regs: R0, R1, R2 and R6
LDW R1, R6, #1
LDW R2, R6, #2
ADD R6, R6, #6

RTI

PTBR .FILL X1000
COUNT .FILL #128 ; 128 iterations
.END