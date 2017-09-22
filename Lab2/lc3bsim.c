
/*
     REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS
 
        Name 1: Hongchang Liang
        Name 2: Yuwei Liu
        UTEID 1: hl23673
        UTEID 2: UT EID of the second partner
 */

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
 *   MEMORY[A][1] stores the most significant byte of word at word address A
 *   */

#define WORDS_IN_MEM    0x08000
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;    /* run bit */


typedef struct System_Latches_Struct{

 int PC,        /* program counter */
   N,        /* n condition bit */
   Z,        /* z condition bit */
   P;        /* p condition bit */
 int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {
 printf("----------------LC-3b ISIM Help-----------------------\n");
 printf("go               -  run program to completion         \n");
 printf("run n            -  execute program for n instructions\n");
 printf("mdump low high   -  dump memory from low to high      \n");
 printf("rdump            -  dump the register & bus values    \n");
 printf("?                -  display this help menu            \n");
 printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {

 process_instruction();
 CURRENT_LATCHES = NEXT_LATCHES;
 INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {
 int i;

 if (RUN_BIT == FALSE) {
   printf("Can't simulate, Simulator is halted\n\n");
   return;
 }

 printf("Simulating for %d cycles...\n\n", num_cycles);
 for (i = 0; i < num_cycles; i++) {
   if (CURRENT_LATCHES.PC == 0x0000) {
       RUN_BIT = FALSE;
       printf("Simulator halted\n\n");
       break;
   }
   cycle();
 }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {
 if (RUN_BIT == FALSE) {
   printf("Can't simulate, Simulator is halted\n\n");
   return;
 }

 printf("Simulating...\n\n");
 while (CURRENT_LATCHES.PC != 0x0000)
   cycle();
 RUN_BIT = FALSE;
 printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {
 int address; /* this is a byte address */

 printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
 printf("-------------------------------------\n");
 for (address = (start >> 1); address <= (stop >> 1); address++)
   printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
 printf("\n");

 /* dump the memory contents into the dumpsim file */
 fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
 fprintf(dumpsim_file, "-------------------------------------\n");
 for (address = (start >> 1); address <= (stop >> 1); address++)
   fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
 fprintf(dumpsim_file, "\n");
 fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {
 int k;

 printf("\nCurrent register/bus values :\n");
 printf("-------------------------------------\n");
 printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
 printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
 printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
 printf("Registers:\n");
 for (k = 0; k < LC_3b_REGS; k++)
   printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
 printf("\n");

 /* dump the state information into the dumpsim file */
 fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
 fprintf(dumpsim_file, "-------------------------------------\n");
 fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
 fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
 fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
 fprintf(dumpsim_file, "Registers:\n");
 for (k = 0; k < LC_3b_REGS; k++)
   fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
 fprintf(dumpsim_file, "\n");
 fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {
 char buffer[20];
 int start, stop, cycles;

 printf("LC-3b-SIM> ");

 scanf("%s", buffer);
 printf("\n");

 switch(buffer[0]) {
 case 'G':
 case 'g':
   go();
   break;

 case 'M':
 case 'm':
   scanf("%i %i", &start, &stop);
   mdump(dumpsim_file, start, stop);
   break;

 case '?':
   help();
   break;
 case 'Q':
 case 'q':
   printf("Bye.\n");
   exit(0);

 case 'R':
 case 'r':
   if (buffer[1] == 'd' || buffer[1] == 'D')
       rdump(dumpsim_file);
   else {
       scanf("%d", &cycles);
       run(cycles);
   }
   break;

 default:
   printf("Invalid Command\n");
   break;
 }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {
 int i;

 for (i=0; i < WORDS_IN_MEM; i++) {
   MEMORY[i][0] = 0;
   MEMORY[i][1] = 0;
 }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {
 FILE * prog;
 int ii, word, program_base;

 /* Open program file. */
 prog = fopen(program_filename, "r");
 if (prog == NULL) {
   printf("Error: Can't open program file %s\n", program_filename);
   exit(-1);
 }

 /* Read in the program. */
 if (fscanf(prog, "%x\n", &word) != EOF)
   program_base = word >> 1;
 else {
   printf("Error: Program file is empty\n");
   exit(-1);
 }

 ii = 0;
 while (fscanf(prog, "%x\n", &word) != EOF) {
   /* Make sure it fits. */
   if (program_base + ii >= WORDS_IN_MEM) {
       printf("Error: Program file %s is too long to fit in memory. %x\n",
            program_filename, ii);
       exit(-1);
   }

   /* Write the word to memory array. */
   MEMORY[program_base + ii][0] = word & 0x00FF;
   MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
   ii++;
 }

 if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

 printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
 int i;

 init_memory();
 for ( i = 0; i < num_prog_files; i++ ) {
   load_program(program_filename);
   while(*program_filename++ != '\0');
 }
 CURRENT_LATCHES.Z = 1;
 NEXT_LATCHES = CURRENT_LATCHES;

 RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {
 FILE * dumpsim_file;

 /* Error Checking */
 if (argc < 2) {
   printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
          argv[0]);
   exit(1);
 }

 printf("LC-3b Simulator\n\n");

 initialize(argv[1], argc - 1);

 if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
   printf("Error: Can't open dumpsim file\n");
   exit(-1);
 }

 while (1)
   get_command(dumpsim_file);

}

/***************************************************************/
/* Do not modify the above code.
  You are allowed to use the following global variables in your
  code. These are defined above.
 
   MEMORY
 
   CURRENT_LATCHES
   NEXT_LATCHES
 
  You may define your own local/global variables and functions.
  You may use the functions to get at the control bits defined
  above.
 
  Begin your code here                        */

#include "math.h"

#define mask01 0x01
#define mask05 0x01F
#define mask06 0x03F
#define mask09 0x01FF
#define mask11 0x7FF
#define BR  0
#define ADD 1
#define LDB 2
#define STB 3
#define JSR 4
#define AND 5
#define LDW 6
#define STW 7
#define RTI 8
#define XOR 9
#define UNDEF10 10
#define UNDEF11 11
#define JMP 12
#define SHF 13
#define LEA 14
#define TRAP 15


/*Get bit N's value of ir */
int getBit(int n,int ir) {
  int bitN;
  return (ir>>n)&mask01 ;
}

/*get integer represented by [to:from] bits*/
int getBitRange(int from,int to, int ir) {
	int mask, tFrom, tTo;
	tFrom = from;
	tTo = to ;
	if(from > to) {
		tFrom = to;
		tTo = from ;
	}
	mask = pow(2,tTo-tFrom+1)-1 ;
	return (ir>>tFrom)&mask ;
}

int getOpcode(int ir) {
   return getBitRange(12,15,ir);
}
int sext(int n, signed int Num) {
	/*n is the total number of bits of int Num */
	signed int tmp ;
	int ns = 32 - n ;
	tmp = (Num<<ns)>>ns ;
	return tmp ;
}
void checkOpcode(int ir,int opcode){
 int tOpcode ;
 tOpcode = getOpcode(ir) ;
 if(tOpcode != opcode) {
 	printf("Error: In func: checkOpcode: illegal opcode %d, should be %d", tOpcode,opcode);
 	exit(1);
 }
}
void setCC(int dr) {
	if (NEXT_LATCHES.REGS[dr]==0)  {
        NEXT_LATCHES.N=0;
        NEXT_LATCHES.Z=1;
        NEXT_LATCHES.P=0;
        printf("Info: In func: setCC : set Z =1 based dr 0x%x\n",NEXT_LATCHES.REGS[dr]);
	} else if (getBitRange(15,15,NEXT_LATCHES.REGS[dr])==0)  {
        NEXT_LATCHES.N=0;
        NEXT_LATCHES.Z=0;
        NEXT_LATCHES.P=1;
        printf("Info: In func: setCC : set P =1 based on dr 0x%x\n",NEXT_LATCHES.REGS[dr]);
	} else if(getBitRange(15,15,NEXT_LATCHES.REGS[dr])==1){
        NEXT_LATCHES.N=1;
        NEXT_LATCHES.Z=0;
        NEXT_LATCHES.P=0;
        printf("Info: In func: setCC: set N = 1 based on dr 0x%x\n",NEXT_LATCHES.REGS[dr]);
	}
}

/*opcode= 0*/
void br(int ir){

    int pcOffset9,n,z,p,N,Z,P,opcode;

    printf("Info： In func： br  : ir==%x\n",ir);

    checkOpcode(ir,BR);

    pcOffset9 = getBitRange(0,8,ir);

    n = getBit(11,ir);
    z = getBit(10,ir);
    p = getBit(9,ir);

    N = CURRENT_LATCHES.N;
    Z = CURRENT_LATCHES.Z;
    P = CURRENT_LATCHES.P;

    pcOffset9  = sext(9,pcOffset9) << 1; 
    if((n&&N) || (z&&Z) || (p&&P))
    	NEXT_LATCHES.PC=Low16bits(NEXT_LATCHES.PC+pcOffset9);
}

/*opcode == 1*/
void  add(int ir){

    int dr,sr1,sr2,imm5,opcode;
    signed int temp;

    printf("Info： In func：add : ir==%x\n",ir);
    /*check opcode*/
    checkOpcode(ir,ADD);

    dr   = getBitRange(9,11,ir);
    sr1  = getBitRange(6,8,ir);
    sr2  = getBitRange(0,2,ir);
    imm5 = getBitRange(0,4,ir);

    if(getBit(5,ir)) {
      NEXT_LATCHES.REGS[dr] = Low16bits(CURRENT_LATCHES.REGS[sr1] + sext(5,imm5));
    } else {
      NEXT_LATCHES.REGS[dr] = Low16bits(CURRENT_LATCHES.REGS[sr1] + CURRENT_LATCHES.REGS[sr2]);
    }

    setCC(dr) ;

    printf("Info: In func: add : reg DR sign =  %x\n",getBitRange(15,15,NEXT_LATCHES.REGS[dr]));

}

/*opcode = 2*/

void ldb(int ir) {

    int dr, baseR,bOffset6,addr, memIndex;

    printf("Info： In func： ldb  : ir==%x\n",ir);
    /*check opcode*/
    checkOpcode(ir,LDB);

    dr       = getBitRange(9,11,ir);
    baseR    = getBitRange(6,8,ir);
    bOffset6 = getBitRange(0,5,ir);
    addr     = Low16bits(CURRENT_LATCHES.REGS[baseR] + sext(6,bOffset6));
    memIndex = (addr >> 1);

    if(addr%2==0) {
    	NEXT_LATCHES.REGS[dr] = MEMORY[memIndex][0];
    } else {
    	NEXT_LATCHES.REGS[dr] = MEMORY[memIndex][1];
    }
    printf("Info： In func： ldb : addr: 0x%x, offset:  0x%x ,baseR:0x%x\n",addr,sext(6,bOffset6),CURRENT_LATCHES.REGS[baseR]);
    printf("Info： In func： ldb : memIndex: 0x%x,mem0:0x%x,mem1 0x%x\n",memIndex,MEMORY[memIndex][0],MEMORY[memIndex][1]);

    setCC(dr);
}

/*opcode = 3*/
void stb(int ir){

	    int sr, baseR,bOffset6,addr, memIndex,regVal;

	    printf("Info： In func： stb: ir==%x\n",ir);
	    /*check opcode*/
	    checkOpcode(ir,STB);

	    sr       = getBitRange(9,11,ir);
	    baseR    = getBitRange(6,8,ir);
	    bOffset6 = getBitRange(0,5,ir);
	    addr     = Low16bits(CURRENT_LATCHES.REGS[baseR] + sext(6,bOffset6));
	    memIndex = (addr >> 1);

	    regVal   = getBitRange(0,7,CURRENT_LATCHES.REGS[sr]);
	    if(addr%2==0) {
	    	MEMORY[memIndex][0] = regVal ;
	    } else {
	    	MEMORY[memIndex][1] = regVal ;
	    }
}

/*opcode = 4*/
void 	jsr(int ir){

	int currentPC, pcOffset11, baseR;


	printf("Info： In func： jsr : ir==%x\n",ir);
    /*check opcode*/
	checkOpcode(ir,JSR);

	pcOffset11 = getBitRange(0,10,ir);
	baseR      = getBitRange(6,8,ir);

	NEXT_LATCHES.REGS[7]=NEXT_LATCHES.PC;

	if(getBit(11,ir)) {
		/*JSR*/
		NEXT_LATCHES.PC = Low16bits(NEXT_LATCHES.PC + (sext(11,pcOffset11)<<1));
	} else {
		/*JSRR*/
		NEXT_LATCHES.PC = Low16bits(CURRENT_LATCHES.REGS[baseR]) ;
	}

}


/*opcode = 5*/
void	and(int ir){
    int dr,sr1,sr2,imm5,opcode;

    printf("Info： In func：and : ir==%x\n",ir);
    /*check opcode*/
    checkOpcode(ir,AND);

    dr   = getBitRange(9,11,ir);
    sr1  = getBitRange(6,8,ir);
    sr2  = getBitRange(0,2,ir);
    imm5 = getBitRange(0,4,ir);

    if(getBit(5,ir)) {
      NEXT_LATCHES.REGS[dr] = Low16bits(CURRENT_LATCHES.REGS[sr1] & sext(5,imm5));
    } else {
      NEXT_LATCHES.REGS[dr] = Low16bits(CURRENT_LATCHES.REGS[sr1] & CURRENT_LATCHES.REGS[sr2]);
    }

    setCC(dr) ;

}
/*opcode = 6*/
void ldw(int ir){

    int dr, baseR,bOffset6,addr, memIndex;

    printf("Info： In func： ldw  : ir==%x\n",ir);
    /*check opcode*/
    checkOpcode(ir,LDW);

    dr       = getBitRange(9,11,ir);
    baseR    = getBitRange(6,8,ir);
    bOffset6 = getBitRange(0,5,ir);
    bOffset6 = sext(6,bOffset6) << 1;
    addr     = Low16bits(CURRENT_LATCHES.REGS[baseR] + bOffset6);
    memIndex = addr >> 1;

    if(addr%2==0) {
    	NEXT_LATCHES.REGS[dr] = MEMORY[memIndex][1];
    	NEXT_LATCHES.REGS[dr] = Low16bits((NEXT_LATCHES.REGS[dr]<<8) | getBitRange(0,7,MEMORY[memIndex][0]));
    } else {
    	printf("Error： In func： ldw : address 0x%x is odd for a word align memory!\n",addr);
    }
    setCC(dr);
    	printf("Info： In func： ldw : addr: 0x%x, offset:  0x%x ,baseR:0x%x\n",addr,(sext(6,bOffset6))<<1,CURRENT_LATCHES.REGS[baseR]);
    	printf("Info： In func： ldw : memIndex: 0x%x,mem0:0x%x,mem1 0x%x\n",memIndex,MEMORY[memIndex][0],MEMORY[memIndex][1]);
}

/*opcode = 7*/
void stw(int ir){

	    int sr, baseR,bOffset6,addr, memIndex,regVal;

	    printf("Info： In func： stw : ir==%x\n",ir);
	    /*check opcode*/
	    checkOpcode(ir,STW);

	    sr       = getBitRange(9,11,ir);
	    baseR    = getBitRange(6,8,ir);
	    bOffset6 = getBitRange(0,5,ir);
            bOffset6 = sext(6,bOffset6) << 1;
            addr     = Low16bits(CURRENT_LATCHES.REGS[baseR] + bOffset6);
	    memIndex = (addr >> 1);

	    regVal   = CURRENT_LATCHES.REGS[sr];
	    if(addr%2==0) {
	    	MEMORY[memIndex][0] = getBitRange(0,7,regVal) ;
	    	MEMORY[memIndex][1] = getBitRange(8,15,regVal);
	    } else {
	    	printf("Error： In func： stw : address 0x%x is odd for a word align memory!\n",addr);
	    }
}

/* opcode = 8 */
/*void rti(int ir); -- will not implement*/

/*opcode = 9*/
void	xor(int ir){
    int dr,sr1,sr2,imm5,opcode;

    printf("Info： In func： xor : ir==%x\n",ir);
    /*check opcode*/
    checkOpcode(ir,XOR);

    dr   = getBitRange(9,11,ir);
    sr1  = getBitRange(6,8,ir);
    sr2  = getBitRange(0,2,ir);
    imm5 = getBitRange(0,4,ir);

    if(getBit(5,ir)) {
      NEXT_LATCHES.REGS[dr] = Low16bits(CURRENT_LATCHES.REGS[sr1] ^ sext(5,imm5));
    } else {
      NEXT_LATCHES.REGS[dr] = Low16bits(CURRENT_LATCHES.REGS[sr1] ^ CURRENT_LATCHES.REGS[sr2]);
    }

    setCC(dr) ;

}

/*opcode = 10  -- not used */
/*opcode = 11  -- not used */

/* opcode = 12 */
void jmp(int ir){

	int baseR, bOffset6 ;
	printf("Info： In func： jmp : ir==%x\n",ir);
    /*check opcode*/
	checkOpcode(ir,JMP);

    baseR    = getBitRange(6,8,ir);
    bOffset6 = getBitRange(0,5,ir);

    NEXT_LATCHES.PC=Low16bits(CURRENT_LATCHES.REGS[baseR]);
}

/* opcode = 13 */
void  shf(int ir){

    int dr,sr,amount4,type;

    printf("Info： In func： shf : ir==%x\n",ir);
    /*check opcode*/
    checkOpcode(ir,SHF);

    dr      = getBitRange(9,11,ir);
    sr      = getBitRange(6,8,ir);
    amount4 = getBitRange(0,3,ir);
    type    = getBitRange(4,5,ir);


    if(type==0){
        NEXT_LATCHES.REGS[dr]=Low16bits(CURRENT_LATCHES.REGS[sr]<<amount4);
    } else if(type==1) {
        NEXT_LATCHES.REGS[dr]=Low16bits(CURRENT_LATCHES.REGS[sr]>>amount4);
    } else if(type==3) {
        NEXT_LATCHES.REGS[dr]=Low16bits((CURRENT_LATCHES.REGS[sr]<<16)>>(amount4+16));
    }
    setCC(dr) ;
}

/*opcode = 14*/
void 	lea(int ir){
    int dr,pcOffset9;
    printf("Info： In func： lea : ir==%x\n",ir);
    /*check opcode*/
    checkOpcode(ir,LEA);

    dr        = getBitRange(9,11,ir);
    pcOffset9 = getBitRange(0,8,ir);
    NEXT_LATCHES.REGS[dr] = Low16bits(NEXT_LATCHES.PC + (pcOffset9<<1));

}

/*opcode = 15*/
void trap(int ir){
/*R7 = PC+;
  PC = MEM[LSHF(ZEXT(trapvect8), 1)];
*/
    int trapvect8,memIndex ;
    printf("Info： In func： trap : ir==%x\n",ir);
    /*check opcode*/
    checkOpcode(ir,TRAP);
    trapvect8 = getBitRange(0,7,ir);
    memIndex  = trapvect8 << 1 ;

    NEXT_LATCHES.REGS[7]=Low16bits(NEXT_LATCHES.PC);
    NEXT_LATCHES.PC = MEMORY[memIndex][1];
    NEXT_LATCHES.PC = Low16bits((NEXT_LATCHES.PC <<8) | getBitRange(0,7,MEMORY[memIndex][0]));
}

/****************************************/

void process_instruction(){
/*  function: process_instruction
   
    Process one instruction at a time
       -Fetch one instruction
       -Decode
       -Execute
       -Update NEXT_LATCHES
 */
/****************************************/

    int state,memIndex;
    int irLow, ir;

    /* Fetch */
    memIndex = CURRENT_LATCHES.PC >> 1 ;
    ir= MEMORY[memIndex][1];
    irLow=MEMORY[memIndex][0];
    ir=Low16bits((ir<<8)|irLow);
    NEXT_LATCHES.PC = CURRENT_LATCHES.PC + 2 ;

    /* Decode */
    state=ir>>12;
    printf("Info: In func :main: opcode: %d\n",state);

    /*Execute & Update NEXT_LATCHES*/
    switch (state){
       case BR:		br(ir);   break;
       case ADD:	add(ir);  break;
       case LDB:	ldb(ir);  break;
       case STB:	stb(ir);  break;
       case JSR:	jsr(ir);  break;
       case AND:	and(ir);  break;
       case LDW:	ldw(ir);  break;
       case STW:	stw(ir);  break;
       case XOR:	xor(ir);  break;
       case JMP: 	jmp(ir);  break;
       case SHF:	shf(ir);  break;
       case LEA:	lea(ir);  break;
       case TRAP:	trap(ir); break;
    }

    return;
}
