;/*
;        Name 1: Hongchang Liang
;        UTEID 1: hl23673
; */

     ;sBase: label, source starting address(4000)
     ;dBase: label, dest starting address(4005)
     ;maskAddr: label, shuffle mask address(4004)  
     ;R0: count,when reach 4, exit
     ;R1: mask, will right shift by 2 every loop
     ;R2: sAddr (sBase + sOffset)
     ;R3: dAddr (dBase + count)
     ;R4: sOffset (mask[1:0])
     ;R5: tmp
;
;
     .ORIG x3000
     AND R0,R0,#0; R0=count =0

     ;load mask value
     LEA R2,maskAddr
     LDW R2,R2,#0
     LDB R2,R2,#0; R2=mask
     AND R1,R2,x03; R1=sOffset = mask[1:0]

 LOOP LEA R5,sBase
     LDW R5,R5,#0
     ADD R3,R5,R1; R3=sAddr = sBase + sOffset

     LEA R5,dBase
     LDW R5,R5,#0
     ADD R4,R5,R0 ; R4=dAddr = dBase + count

     LDB  R3,R3,#0; R3=[sAddr]
     STB  R3,R4,#0; [dAddr] = [sAddr]

     ADD R0,R0,#1 ; count++
     RSHFL R2,R2,#2 ; right shift mask by 2
     AND R1,R2,x03; R2=sOffset = mask[1:0]
     XOR R5,R0,#4 ;
     BRnp LOOP

     
;sBase      .FILL x4000    
;dBase      .FILL x4005    
;maskAddr   .FILL x4004    
sBase      .FILL x302a ;sData Address, eventually change to x400
maskAddr   .FILL x302e ;sMask address, eventually change to x4004 
dBase      .FILL x3030 ;dData address, eventually change to x4005
sData      .FILL x4241 ; ASCII A
           .FILL x4342 ; ASCII B
sMask      .FILL xE4 ; copy
dData      .FILL x00
           .FILL x00
           .FILL x00
           .FILL x00

       AND R1,R1,#0; clear counter
OUTPUT LEA R0,dData
       ADD R0,R0,R1      
       LDB R0,R0,#0
       TRAP x21 ; ASCII code in R0 is displayed 
       ADD R1,R1,#1
       XOR R2,R1,#4
       BRnp OUTPUT

       HALT    

      .END
