#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */
#include <stdint.h>
#define MAX_LINE_LENGTH 255
#define MAX_LABEL_LEN 20
#define MAX_SYMBOLS 255
// function def
int readAndParse(FILE * pInfile, char * pLine, char ** pLabel, char** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4);
char* symbols[MAX_LINE_LENGTH];
int locations[MAX_LINE_LENGTH];
int symbolTableEnding = 0;
char* orig = 0x0000;
int programCounter = 0x0000;
int end = 0;
int reg[8];
enum{DONE, OK, EMPTY_LINE};

/*for the symbol table mapping*/
typedef struct {
  int address;
  char label[MAX_LABEL_LEN + 1];        /* Question for the reader: Why do we need to add 1? */
} TableEntry;

TableEntry symbolTable[MAX_SYMBOLS];


int main(int argc, char* argv[]) {

    char *prgName   = NULL;
    char *iFileName = NULL;
    char *oFileName = NULL;

    prgName   = argv[0];
    iFileName = "input.asm";
    oFileName = "output.hex";
    if(!iFileName){
        printf("Not such file input or file output.\n");
        exit(4);
    }
    /*creating the symbol table for the first pass phase*/
    firstPass(iFileName);
    /*Second pass to generate the actual instruction*/
    secondPass(iFileName);
    printf("========================================================\n");
    for(int i = 0; i < symbolTableEnding; i++)  printf("%s, %d\n", symbolTable[i].label, symbolTable[i].address);
}

void insertSymbolTable(char* lLabel, int programCounter){
    strcpy(symbolTable[symbolTableEnding].label, lLabel);
    symbolTable[symbolTableEnding].address = programCounter;
    symbolTableEnding++;
}
int isValidLabel(char* label) {
  /*may need double check the error code might contain? */
  int i = 0;
  if(!strcmp(label, "")){
    return 0;
  }
  else if (!label[0] || label[0] == 'x') {
    return 0;
  }
  else if (!strcmp(label, "in") || !strcmp(label, "out") ||
      !strcmp(label, "getc") || !strcmp(label, "puts")) {
      return 0;
  }
  while (label[i]) {
    if (!isalnum(label[i]) || i > 20) {
      return 0;
    }
    i++;
  }
  return 1;
}
void add(char* lOpcode, char** lArg1, char** lArg2, char** lArg3){
  if(!lArg1 || !lArg2 || !lArg3 || lArg1 == '\0'|| lArg2 == '\0'|| lArg3 == '\0') exit(0);
  int16_t instruction = 1 << 12;
  printf("%d\n", instruction);
  printf("r1: %s\n", lArg1);
  printf("r2: %s\n", lArg2);
  //printf("%c\n", lArg1[0]);
  //printf("%s\n", lArg1);

}
void secondPass(char* inputFileName){
  char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
  int lRet;
  FILE * lInfile;
  lInfile = fopen(inputFileName, "r");  /* open the input file */
  /*for the first pass, we only have to do establishing symbol table.*/
  do{
    lRet = readAndParse( lInfile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
     if( lRet != DONE && lRet != EMPTY_LINE ){
       if(lOpcode && lArg1){
        //still need to log the programCounter value.
         if(!strcmp(lOpcode, ".orig")){
           programCounter = toNum(lArg1);
           printf("*******The program starts at: %d*******\n", programCounter); /*0x3000 ---> 12288*/
         }
      }else
        exit(0);
      if(!strcmp(lOpcode, "add"))add(lOpcode, lArg1, lArg2, lArg3);
    }
  }while( lRet != DONE );
}

void firstPass(char* inputFileName){
  char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
  int lRet;
  FILE * lInfile;
  lInfile = fopen(inputFileName, "r");	/* open the input file */
 /* for the first pass, we only have to do establishing symbol table.*/
  do{
    lRet = readAndParse( lInfile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
     if( lRet != DONE && lRet != EMPTY_LINE ){
       if(lOpcode && lArg1){
         if(!strcmp(lOpcode, ".orig")){
           programCounter = toNum(lArg1);
           printf("*******The program starts at: %d*******\n", programCounter); /*0x3000 ---> 12288*/
         }
      }else exit(0);
      if(isValidLabel(lLabel)){
        insertSymbolTable(lLabel, programCounter);
      }
    }
    programCounter+=2;
  }while( lRet != DONE );
}
void processOpt(char* lOpcode, char** lArg1, char** lArg2, char** lArg3, char** lArg4){
  printf("Now in process Opt: %s\n", lOpcode);
}
int toNum(char *pStr)
{
  char * t_ptr;
  char * orig_pStr;
  int t_length,k;
  int lNum, lNeg = 0;
  long int lNumLong;

  orig_pStr = pStr;
  if( *pStr == '#' )                                /* decimal */
  {
    pStr++;
    if( *pStr == '-' )                                /* dec is negative */
    {
      lNeg = 1;
      pStr++;
    }
    t_ptr = pStr;
    t_length = strlen(t_ptr);
    for(k=0;k < t_length;k++)
    {
      if (!isdigit(*t_ptr))
      {
         printf("Error: invalid decimal operand, %s\n",orig_pStr);
         exit(4);
      }
      t_ptr++;
    }
    lNum = atoi(pStr);
    if (lNeg)
      lNum = -lNum;

    return lNum;
  }
  else if( *pStr == 'x' )        /* hex     */
  {
    pStr++;
    if( *pStr == '-' )                                /* hex is negative */
    {
      lNeg = 1;
      pStr++;
    }
    t_ptr = pStr;
    t_length = strlen(t_ptr);
    for(k=0;k < t_length;k++)
    {
      if (!isxdigit(*t_ptr))
      {
         printf("Error: invalid hex operand, %s\n",orig_pStr);
         exit(4);
      }
      t_ptr++;
    }
    lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
    lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
    if(lNeg)
      lNum = -lNum;
    return lNum;
    }
  else
  {
    printf( "Error: invalid operand, %s\n", orig_pStr);
    exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
  }
}

int isOpcode(char* op){
    if(!strcmp(op, "add") || !strcmp(op, "and") || !strcmp(op, "br") ||!strcmp(op, "brn") || !strcmp(op, "brz") || !strcmp(op, "brp") ||!strcmp(op, "brnz") || !strcmp(op, "brnp") || !strcmp(op, "brzp")
        ||!strcmp(op, "brnzp") || !strcmp(op, "jmp") || !strcmp(op, "jsr") ||!strcmp(op, "jsrr") || !strcmp(op, "ldb") || !strcmp(op, "ldw") ||!strcmp(op, "lea") || !strcmp(op, "not") || !strcmp(op, "ret")
        ||!strcmp(op, "rti") || !strcmp(op, "lshf") || !strcmp(op, "rshfl") ||!strcmp(op, "rshfa") || !strcmp(op, "stb") || !strcmp(op, "stw") ||!strcmp(op, "trap") || !strcmp(op, "xor")){return 1;}
    else return -1;
}

/*read the file and decide whether or not is empty line? return enum{DONE, OK, EMPTY_LINE};*/
int readAndParse(FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode,
                 char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4){
  char *lRet, *lPtr;
  int i;
  if(!fgets(pLine, MAX_LINE_LENGTH, pInfile ))
      return(DONE);

    /* convert entire line to lowercase */
  for(i = 0;i < strlen( pLine );i++ )
      pLine[i] = tolower(pLine[i]);
  /* set all args to point null terminator of line? */
  *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);
  /* ignore the comments */
  lPtr = pLine;
  while( *lPtr != ';' && *lPtr != '\0' && *lPtr != '\n' )
    lPtr++;
  *lPtr = '\0'; //replace comment or end of line with null terminator

  if(!(lPtr = strtok( pLine, "\t\n ,")))
    return(EMPTY_LINE);

  if(isOpcode( lPtr ) == -1 && lPtr[0] != '.' ){ /* found a label */
     *pLabel = lPtr;
      if(!( lPtr = strtok( NULL, "\t\n ,")))return(OK);
  }
  *pOpcode = lPtr;
  if(!(lPtr = strtok(NULL, "\t\n ,")))return(OK);
  *pArg1 = lPtr;
  if(!(lPtr = strtok(NULL, "\t\n ,")))return(OK);
  *pArg2 = lPtr;
  if(!(lPtr = strtok(NULL, "\t\n ,")))return(OK);
  *pArg3 = lPtr;
  if(!(lPtr = strtok(NULL, "\t\n ,")))return(OK);
  *pArg4 = lPtr;
  return(OK);
}
