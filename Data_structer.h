#ifndef DATA_STRUCTER_H
#define DATA_STRUCTER_H

#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

/*#############*/
/*#defines#####*/
/*#############*/
#define MAX_LINE_SIZE 80
#define MAX_SIZE_OF_SYMBOL 31
#define IC 100
#define DC 0
#define SYNTAX_TYPE_LENGTH 6
#define REG_NAME_LEN 3
int errorFlag = 0;



typedef enum /*true and false*/
{
    FALSE, TRUE
}boolean_val;

enum
{
    DESTINATION = 1,
    SOURCE = 2
};

enum
{
    T_NONE = -1,
    T_SYMBOL = 1,
    T_NUMBER = 2
};


/*#############*/
/*#bitfields###*/
/*#############*/
typedef struct
{
    unsigned int ARE:2; /*00 01 10 11 */
    unsigned int source:2; /* 00 01 10 11*/
    unsigned int target:2; /* 00 01 10 11*/
    unsigned int opcode:4; /* 0000 0001 0010 0011 0100 0101 0110 0111 1000...*/
}word_main;

typedef struct
{
    unsigned int ARE:2;
    unsigned int number:12;
}word_number;

typedef struct
{
    unsigned int ARE:2;
    unsigned int source_reg:3;
    unsigned int target_reg:3;
}word_register;

typedef union
{
    unsigned int zero;       /*initialize the values of the word bitfield to zero*/
    word_main     firstword;   /*bitfield of the first word*/
    word_number   secondword;  /*bitfield of the first word*/
    word_register thirdword; /*bitfield of the first word*/
}cmd_encoded;

typedef struct
{
    unsigned int DATA:14;
}word_data;

typedef struct
{
    char symbol_name[MAX_SIZE_OF_SYMBOL];
    int LOCATION;
    int lineNumber;
    int Extern;
    int Entry;
}helper;



/*for the OPCODE*/
/*todo:delete it?*/
enum
{
    OP_mov,
    OP_cmp,
    OP_add,
    OP_sub,
    OP_not,
    OP_clr,
    OP_lea,
    OP_inc,
    OP_dec,
    OP_jmp,
    OP_bne,
    OP_red,
    OP_prn,
    OP_jsr,
    OP_rts,
    OP_stop
};



/*################*/
/*#Symbol Table###*/
/*################*/
typedef enum {  /*define label type: data, code, external or entry*/
    DATA_SYMBOL, CODE_SYMBOL, EXTERNAL_SYMBOL, ENTRY_SYMBOL, MACRO_SYMBOL
}LABEL_TYPE;



typedef struct{ /*DATASTRUCTURE TO STORE LABELS AND ITS ADDRESS*/
    char symbolName[MAX_SIZE_OF_SYMBOL];
    LABEL_TYPE symbolType;
    int value;
}LabelAddressPair;

typedef struct{     /*DATASTRUCTURE TO STORE LABELS ADDRESS pairs*/
    int size; /*todo: maybe not needed*/
    LabelAddressPair* symbolTypeAndValue;
}SymbolTable;

/*###################*/
/*#DATA IMAGE########*/
/*###################*/







/*###################*/
/*#ADDRESSING MODE###*/
/*###################*/
typedef enum {
    OP_NONE= -1, OP_IMMEDIATE = 1, OP_DIRECT = 2, OP_INDEX = 4, OP_REGISTER = 8
}ADDRESSING_TYPE;

typedef enum {
    NONE=0, IMMEDIATE = 1,DIRECT = 2,DISPLACEMENT = 4,REGISTER = 8
}ADDRESSING_MODE;

typedef struct{     /*struct which represents register*/
    char name[REG_NAME_LEN]; /*for the name of the register r0 r1.... r7*/
    int number;              /*number that represents the register r0 = 0...r7=7*/
}Register;

static const Register registers[] = /*array with available registers, its name and decimal value*/
{
        {"r0",  0},
        {"r1",  1},
        {"r2",  2},
        {"r3",  3},
        {"r4",  4},
        {"r5",  5},
        {"r6",  6},
        {"r7",  7}
};

typedef struct {        /*struct to contain various fields of our command*/
    char *name;         /*command name*/
    int opcodeNUM;    /*decimal code*/
    int operatorsNUM;   /*how many operators it should receive*/
    int source;    /*permitted source addressing mode*/
    int destination;   /*permitted destination addressing mode*/
} Command;

                                        /*IMMEDIATE = 1,DIRECT = 2,DISPLACEMENT = 4,REGISTER = 8*/
static const Command commands[] = {         /*array with predefined command and permitted value (parameters)*/
        {"mov",     0,       2, IMMEDIATE|DIRECT|DISPLACEMENT|REGISTER, DIRECT|DISPLACEMENT|REGISTER},
        {"cmp",     1,       2, IMMEDIATE|DIRECT|DISPLACEMENT|REGISTER, IMMEDIATE|DIRECT|DISPLACEMENT|REGISTER},
        {"add",     2,       2, IMMEDIATE|DIRECT|DISPLACEMENT|REGISTER, DIRECT|DISPLACEMENT|REGISTER},
        {"sub",     3,       2, IMMEDIATE|DIRECT|DISPLACEMENT|REGISTER, DIRECT|DISPLACEMENT|REGISTER},
        {"not",     4,       1, NONE,                                   DIRECT|DISPLACEMENT|REGISTER},
        {"clr",     5,       1, NONE,                                   DIRECT|DISPLACEMENT|REGISTER},
        {"lea",     6,       2, DIRECT|DISPLACEMENT,                    DIRECT|DISPLACEMENT|REGISTER},
        {"inc",     7,       1, NONE,                                   DIRECT|DISPLACEMENT|REGISTER},
        {"dec",     8,       1, NONE,                                   DIRECT|DISPLACEMENT|REGISTER},
        {"jmp",     9,       1, NONE,                                   DIRECT|REGISTER},
        {"bne",     10,      1, NONE,                                   DIRECT|REGISTER},
        {"red",     11,      1, NONE,                                   DIRECT|DISPLACEMENT|REGISTER},
        {"prn",     12,      1, NONE,                                   IMMEDIATE|DIRECT|DISPLACEMENT|REGISTER},
        {"jsr",     13,      1, NONE,                                   DIRECT|REGISTER},
        {"rts",     14,      0, NONE,                                   NONE},
        {"stop",    15,      0, NONE,                                   NONE}
};


























/*#########################*/
/*#functions declartions###*/
/*#########################*/
void pars(int argc, char* argv[]);
int decimalToBinary(int);
char* getFirstWord(char*, char*);
char* deleteFirstDelimeters(char*);
int isEmpty(char*);
int isSymbol(char*);
void getLine(FILE *, char*);
int checkTheSyntaxMacro(char *, char *);
int checkSyntaxMacro(char *line, int);
int isNumber(char *);
char* deletePrevWord(char *a, char *);
int SentenceType(char *[], char *, int *);
int isInteger(double val);
char* getWordsFromLine(char *,char *, char*[]); /*int (*[])(char *)*/
int checkWordLength(int);
int checkOneDelimeterAndDelete(char*);
void change(char **line);
int ValidSpace(char *line);
int SentenceAlgo(int index, char*, int (*[])(char *, int));
int isLetter(char *line);
int isLetterForEntryExtern(char *line);
void initializeBuffer(char *buffer);
int checkSyntaxLabel(char *line, int indexOfSavedWord);
void parsing(FILE *fp, char *line,char *wordBuff, char*savedWords[]);
int trueLabel(char *line);
int arrayNumbers(char *str);
int checkComma(char *str);
int isData(char *line, int);
int isNumberForData(char *str);
int isString(char *line, int);
int arrayStrings(char *str);
void addSymbolToTheSymbolTable(char *symbolName, LABEL_TYPE symbolType, int lineIndex, int value);
int ADD2TABLEstrNdata(int value, LABEL_TYPE symbolType, char *line);
void printSymbolTable();
char* getWORD(char* line, char *buff);
int isLabelExists(char* label);
int getLabelValue(char* label);
int isOPCODE(char* line, int index);
int checkIf_SYMBOL_LabelBefore();
int getRegisterNum(char* str);
int isValidRegister(char* str);
int isValidCommand(const char *str);
int isExtern(char *line, int indexOfSavedWord);
int isEntry(char *line, int indexOfSavedWord);
int getCommandNUM(const char *str);
int isitHashTag(char *line);
void set2OPCODESin2Words(char *line, char *word1[], char *word2[]);
int whatKindOfADDRESSING(char *word);
int isValidADDRESSforMethod(int opcodeNUM, int address1, int address2);
int isValidADDRESSfor_ONE_Method(int opCODEnum, int addressingType1);
int setWords_code(char *word, int addressingType, int Type_destinationORsource);
int getNUMBER(char *str);
void debuggingDATAIMAGE();
int isINDEX(char *word);
int isLetterFOR_OPCODE(char *line);
int extractSymbol_VALUE_FromIndex(char *word);
char * extractSymbol_NAME_FromIndex(char *word, char *buff1);
int ERROR();
char * ERROR_char();
int ERROR_MINUS();
void ERROR_VOID();
int isSymbolInsideTheHelperStruct();
void resetALL();
void HelpStoreArrayPrint();
void freeSymbolTable();
void prnt1();
void prnt2();
void SpecialBaseFour(unsigned short x);
/*###############*/
/*#GLOBAL VALUES#*/
/*###############*/
char *saved_words[] =
{
        ".define",
        ":",
        ".data",
        ".string",
        ".extern",
        ".entry",
        "op_code"
};
/*todo: need to complete the isData to add a variable for the Label as parameter*/
int (*Functions[])(char *line, int indexOfSavedWord) =
{
        checkSyntaxMacro,
        checkSyntaxLabel,
        isData,
        isString,
        isExtern,
        isEntry,
        isOPCODE
};







SymbolTable *SYMBOL_TABLE;
int INSTRUCTIONS_COUNTER = 100;
int INSTRUCTIONS_COUNTER_HELPER = 0;
int DATA_COUNTER = 0;
int lineIndex = 0;
int HELP_ARRAY_INDEX = 0;
char helperBufferDATA[MAX_SIZE_OF_SYMBOL] = " ";
char helperBufferOPCODE[MAX_SIZE_OF_SYMBOL] = " ";
char FILE_NAME[MAX_LINE_SIZE] = "";
FILE *fx;
FILE *fn;
FILE *fs;
int DC_TEMP = 0;

char *SavedLabelsForLater[MAX_SIZE_OF_SYMBOL] = {""};

cmd_encoded CODE_IMAGE[1000];
word_data   DATA_IMAGE[1000];

helper HELP_STORE_ARRAY[1000];
helper HELP_STORE_ARRAY2[1000];

#endif /*DATA_STRUCTER_H*/
