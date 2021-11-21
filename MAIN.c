/*
 * Author:         Eden Refaelov
 * ID:             311137434
 * Study Center:   710
 * Group:          81 
 *
 */
#include "Data_structer.h"

int main(int argc, char *argv[])
{
    FILE *fp;                                   /*pointer to FILE of type FILE*/
    int i;
    char singleLine[MAX_LINE_SIZE];                       /*max size of a line store*/
    static char tempChar[MAX_SIZE_OF_SYMBOL];             /*temporary value, contain Symbol*/
    char argv_helper[100] = "";
    char argv_helper2[100] = "";
    char argv_helper3[100] = "";
    char argv_helper4[100] = "";
    for(i=1; i<argc; i++)
    {
        strcpy(argv_helper, argv[i]);
        strcpy(argv_helper2, argv[i]);
        strcpy(argv_helper3, argv[i]);
        strcpy(argv_helper4, argv[i]);
        strcat(argv_helper, ".as");
        strcat(argv_helper2, ".ext");
        strcat(argv_helper3, ".ent");
        strcat(argv_helper4, ".ob");
        printf("\t\t****FILE NUMBER****: %d\n", i);
        printf("argv is: %s\n", argv[i]);
        fp = fopen(argv_helper, "r");                             /*open the file path*/
        if(!fp)                                               /*checks if the file opened successfully*/
        {
            printf("can't open the file\n");
            fprintf(stderr, "cannotopen\n");
            errorFlag++;
            exit(0);
        }
        fx = fopen(argv_helper2, "w");
        fn = fopen(argv_helper3, "w");
        fs = fopen(argv_helper4, "w");
        parsing(fp, singleLine, tempChar, saved_words);       /*calling parsing function for activate the assembler*/
        HelpStoreArrayPrint();
        isSymbolInsideTheHelperStruct();
        printf("ERROR CODE IS: %d\n", errorFlag);
        prnt1();
        prnt2();
        resetALL();
        printSymbolTable();
        printf("\n");
        initializeBuffer(argv_helper);
        printf("symbol table size is: %d\n",SYMBOL_TABLE->size);
        fclose(fp);
        /*fclose(fx);*/
        /*fclose(fn);*/
	fclose(fs);
    }

    exit(0);
}

/*###########*/
/*#Parsing###*/
/*###########*/
void parsing(FILE *fp, char *line, char *wordBuff, char*savedWords[])
{
    if(feof(fp))                                /*stopping term for the recursion*/
    {
        puts("\n******************\nEND OF THE ASSEMBLER COMPILER\n******************\n");
        printSymbolTable();                     /*will print the Symbol table*/
        debuggingDATAIMAGE();
        printf("\nINSTRUCTION COUNTER IS: %d\n", INSTRUCTIONS_COUNTER);

        return;
    }
    printf("\t\t###################\n\t\tline Number = %d\n\t\t###################\n", lineIndex);
    getLine(fp, line);                              /*gets new line and store the result in line variable*/
    printf("line number: %d, line contain: %s\n", lineIndex, line);
    lineIndex++;
    /*todo:add here stop term for ubuntu*/
    if(feof(fp))                                /*stopping term for the recursion*/
    {
        puts("\n******************\nEND OF THE ASSEMBLER COMPILER\n******************\n");
        printSymbolTable();                     /*will print the Symbol table*/
        debuggingDATAIMAGE();
        printf("\nINSTRUCTION COUNTER IS: %d\n", INSTRUCTIONS_COUNTER);

        return;
    }

    /*todo:do it untill here*/
    getWordsFromLine(line, wordBuff, savedWords);   /*get the words from the line and parsing the data*/
    /*todo: fix it:*/
    if(feof(fp))                                /*stopping term for the recursion*/
    {
        puts("\n******************\nEND OF THE ASSEMBLER COMPILER\n******************\n");
        printSymbolTable();                     /*will print the Symbol table*/
        debuggingDATAIMAGE();
        printf("\nINSTRUCTION COUNTER IS: %d\n", INSTRUCTIONS_COUNTER);

        return;
    }
    parsing(fp, line, wordBuff, savedWords);        /*will call the parsing function again in a recursion way*/
}
void getLine(FILE *fp, char *line)
{
    if(feof(fp))                                /*stopping term for the recursion if it's the FILE EOF*/
    {
        printf("end of the file, exits\n");
        printSymbolTable();
        debuggingDATAIMAGE();
        printf("\nINSTRUCTION COUNTER IS: %d\n", INSTRUCTIONS_COUNTER);
        return;
    }
    printf("\n\t\t*****LINE NUMBER %d *****\n", lineIndex);
    fgets(line, MAX_LINE_SIZE+2, fp);
    if(strlen(line) > MAX_LINE_SIZE || isEmpty(line))  /*checks non valid type of a line or ";" for a "side note"*/
    {/*todo*/
        printf("\nthe line length is over the maximum or \";\" exit\n");
        if(strlen(line) > MAX_LINE_SIZE)
            errorFlag++;
        getLine(fp, line);                            /*recursion to get the next line if fails ";" or max of line*/
        lineIndex++;
    }
}
char* getFirstWord(char* line, char *buff)                                    /*getting the First word in the line */
{
    int i = 0;
    line = deleteFirstDelimeters(line);                                         /*getting the line without delimeters*/
    while((!isEmpty(line)) && *line != '\t' && *line != ' ' && *line != ':' && *line != ',')    /*getting the first word untill ->*/
    {                                                                           /*' ' '\t' : and "isEmpty" terms */
        buff[i] = *line;                  /*getting chars to buffer 1 by 1 from the place line points*/
        line = line+1;                    /*advanced line pointer*/
        i++;                              /*advanced i for check the word length if it's over the defined symbol size*/
    }

    if(!checkWordLength(i))               /*now checking if it's over the maximum char length of a word*/
    {
        return ERROR_char();
    }
    return buff;                         /*returns the valid word*/
}
char* getWordsFromLine(char *line,char *wordBuff, char*savedWords[])
{
    int len;
    int Index_forSavedWords;
    strcpy(wordBuff, getFirstWord(line, wordBuff));                    /*first word inside wordBuff*/
    len = strlen(wordBuff);                                            /*gets the length of the word*/
    strcpy(helperBufferDATA, wordBuff);                                /*todo: delete it?*/
    strcpy(helperBufferOPCODE, wordBuff);                                /*todo: delete it?*/

    line = deletePrevWord(line, wordBuff); /*wordBuff got the prev word and line is pointing to the last cell of it*/

    /*line is deleting the previouse word and checksif it's points to ':' for example:
     * word is "LIST" we're deleting it from line so we get ":xxx xxx xxx" in line so the *line place that line
     * is pointing to must be ':' if it's label and if it's label we will place it ':' in wordBuff for the saved
     * words will notice it's a new LABEL, the first word is LABEL.*/
    if(*line == ':')
    {
        wordBuff[len] = ':';
        line = line + 1;
        if(isSymbol(wordBuff))
            strcpy(wordBuff, ":");
    }
    if(SentenceType(savedWords, wordBuff, &Index_forSavedWords)) /*get the index of the saved words and "boolean" value*/
    {

        /*(*Functions[])(*line)*/
        SentenceAlgo(Index_forSavedWords, line, Functions);/*send the word and will decide what function to activate*/




    }
    initializeBuffer(wordBuff);                       /*initilize the buffer of the word for not contain any garbage*/
    return line;
}
int checkSyntaxMacro(char *line, int indexOfSavedWord)
{
    int type = MACRO_SYMBOL;                /*what type is it*/
    char labelNAME[MAX_LINE_SIZE] = " ";    /*name of label  */
    int value;
    int LabelSyntax;
    /*checks if there is 1 space at most*/
    if(!ValidSpace(line))       /*todo:need to fix the errorFlag*/
    {

        return ERROR();
    }

/*    line = line+1;
    line = deleteFirstDelimeters(line); todo:maybe we need it*/

    /*after we deleted all the spaces we're getting to the first letter
     * and we will use the "isLetter" function*/
    LabelSyntax = checkSyntaxLabel(line, indexOfSavedWord);
    if(LabelSyntax)         /*todo:need to fix the errorFlag*/
    {
        strcpy(labelNAME, getFirstWord(line, labelNAME));
        line = line + LabelSyntax; /*gets the line to the end of the letters*/
        /*todo: fix it*/
    }
    else
    {
        return ERROR();
    }
    /*checks if there is 1 space at most*/
    if(!ValidSpace(line))       /*todo:need to fix the errorFlag*/
    {
        return ERROR();
    }
    line = deleteFirstDelimeters(line);/*deletes the delimeters " "... from the line */
    if(*line != '=')        /*todo:need to fix the errorFlag*/
    {
        return ERROR();
    }
    line = line +1; /*advance the line +1 from the '='*/
    if(!ValidSpace(line))       /*todo:need to fix the errorFlag*/
    {
        return ERROR();
    }
    line = deleteFirstDelimeters(line); /*delete the rest delimeters after the '='*/
    if(isNumber(line))  /*checks if it's a valid number if yes it will add it to the symbol tabel by the label*/
    {
        value = atoi(line);                                                 /*makes the number to int with atoi*/
        addSymbolToTheSymbolTable(labelNAME, type, lineIndex, value);       /*gets the macro inside the Symbol Table*/
        return TRUE;
    }
    /*debugg printer*/
    /*printf("\ncheckSyntaxMacro 6 Debugg 6: failed!!!!\n");*/
    return ERROR();                                                         /*will return error value and errorflag+1*/
}
int checkSyntaxLabel(char *line, int indexOfSavedWord)
{
    int start_len = strlen(line);               /*start lenght of the line*/
    char wordHelper[MAX_SIZE_OF_SYMBOL] = " ";  /*helper buffer, contains a word*/
    char wordHelper2[MAX_SIZE_OF_SYMBOL] = " ";
    int gap_len;                                /*checking the gap of the length between 2 lens*/
    int operationStore = 0;                     /*counter of operations*/
    line = deleteFirstDelimeters(line);         /*deleting the delimeters from the line*/
    gap_len = strlen(line);                     /*store the new line length after the deleted delimeters*/

    gap_len = start_len - gap_len;              /*counting the gap of the start length of the line and the length after
    * the deleted delimeters, for example: .define sz      = 7    -->  it will take the ' ' --> spaces before
    * the symbol and count it in "gap_len" untill it will get the the symbol "sz" for example and it will contain
    * it in operationStore counter in the switch case method.
    * for "LABLE:" for example it will count the gap of the delimeters to the opcode or data operation and
    * contain it inside gap_len for the length of the delimeters.*/

    /*after we deleted all the spaces we're getting to the first letter
     * and we will use the "isLetter" function*/
    switch(indexOfSavedWord)/*0 for ".define" , 1 for "LABEL:" getting it by index of the saved words type of sentence*/
    {
        case 0: isLetter(line)?(operationStore = isLetter(line) + gap_len):FALSE; /*operationStore will get the symbol +
                                                                                    * + the delimeters by the gap_len */

            if(isLabelExists(getFirstWord(line, wordHelper2)))
            {
                printf("\nERROR THE LABEL EXISTS!\n"); /*for debugging, it's really nice*/
                return ERROR();
            }
            break;

        case 1:
            strcpy(wordHelper, getFirstWord(line, wordHelper)); /*store the first word of the line in buffer*/
            line = deletePrevWord(line, wordHelper); /* deleting the word from the line*/
            start_len = strlen(line);                /* starting the line from here after the .string or .data
                                                          * for example */
            strcpy(helperBufferOPCODE, wordHelper);
            if(!isspace(*line))                      /*if there isn't space after the word error flag on++*/
            {
                return ERROR();
            }
                /*line = deleteFirstDelimeters(line);
                gap_len = strlen(line);
                gap_len = start_len - gap_len;   we have used this code for the macro, some times i will use it too
                 for label too!*/
            (SentenceType(saved_words, wordHelper, &indexOfSavedWord))? /*2 for .data 3 for .string 4 for opcode*/
            (operationStore = SentenceAlgo(indexOfSavedWord, line, Functions)?
                              (operationStore + gap_len): /*todo: maybe need to fix the gap_len but not neccesary*/
                              (operationStore = FALSE)):
            (operationStore = FALSE);


            /*todo: add more indexes??? 3 4? maybe not neccesary!*/
            if(indexOfSavedWord == 0 || indexOfSavedWord == 1 ||
            indexOfSavedWord == 4 || indexOfSavedWord == 5) /*if it's 0 or 1 it's not valid it's .def or label*/
            {
                return ERROR();
            }
            if(isLabelExists(helperBufferDATA))
            {
                if(indexOfSavedWord == 6)
                    break;
                return ERROR();
            }
            if(operationStore)
            {
                addSymbolToTheSymbolTable(helperBufferDATA, DATA_SYMBOL , lineIndex, INSTRUCTIONS_COUNTER);
                if(INSTRUCTIONS_COUNTER_HELPER == 0)
                    INSTRUCTIONS_COUNTER_HELPER = INSTRUCTIONS_COUNTER;
                INSTRUCTIONS_COUNTER += DC_TEMP;
                DC_TEMP = 0;
                break;
            }
        case 6:
                break;


        default: return ERROR(); /*todo: fix the error label*/
    }
    return operationStore; /*will return true or false, true by values changed --> need to check todo: check*/
}
int SentenceType(char *bunchOfchars[], char *destinationChar, int *charNUM)
{
    int i;
    /*check the Sentence type for ".define" ".data" ".string" ":"*/
    for(i=0; i<SYNTAX_TYPE_LENGTH; i++)
    {
        if(strcmp(destinationChar, bunchOfchars[i]) == 0)
        {
            *charNUM = i;
            return TRUE;
        }
    }
    if(isValidCommand(destinationChar))    /*if it's opcode, valid "command"*/
    {
        *charNUM = SYNTAX_TYPE_LENGTH;     /*for the opcodes, last variable*/
        return TRUE;
    }
    return FALSE;
}
int SentenceAlgo(int index, char *line, int (*Functions[])(char *line, int)) /*todo: need to complete*/
{
    int booleanValue;
    booleanValue = Functions[index](line, index);
    return booleanValue;
}
















/*#############*/
/*#Utilities###*/
/*#############*/







/*#####################################################~~~~~~~~~~~~~~~~~~################################*/

int checkIf_SYMBOL_LabelBefore()
{
    char wordGetter[MAX_SIZE_OF_SYMBOL] = "";
    strcpy(wordGetter, getFirstWord(helperBufferDATA, wordGetter));
    if(isValidCommand(helperBufferDATA))
        return FALSE;
    return TRUE;

}
int isOPCODE(char *line, int indexofCommands) /*todo:*/
{
    int opCODEnum;
    unsigned int unsigned_opCODEnum;
    int addressingType1;
    int addressingType2;
    int coded1, coded2;
    char *word1[MAX_SIZE_OF_SYMBOL] = {""};
    char *word2[MAX_SIZE_OF_SYMBOL] = {""};
    char wordONE[MAX_SIZE_OF_SYMBOL] = " ";
    char wordTWO[MAX_SIZE_OF_SYMBOL] = " ";


    /*the opcode is good, we will check if there is atleast 1 space*/
    if(!ValidSpace(line))       /*todo:need to fix the errorFlag*/
    {
        errorFlag++;
        printf("#######error##########\n");
        return ERROR();
    }

    /* i.) we will check the op_code name is in the list, opCODE will store the decimal value of it*/
    opCODEnum = getCommandNUM(helperBufferOPCODE);
    /* for example mov is 0*/



    line = deleteFirstDelimeters(line);
    /* now we will get to the directives   " mov    "<----all of this deleted,   left---> "#1, r2" for example*/
    /*back*/
    set2OPCODESin2Words(line, word1, word2); /* will return 2 words in a void function from the line*/

    if(*word1 == NULL && *word2 == NULL)
    {
        addressingType1 = OP_NONE;
        addressingType2 = OP_NONE;
    }
    else if(*word2 == NULL)
    {
        strcpy(wordONE, *word1);
        addressingType1 = whatKindOfADDRESSING(wordONE);
        addressingType2 = OP_NONE;
    }
    else
    {
        strcpy(wordONE, *word1);
        strcpy(wordTWO, *word2);
        addressingType1 = whatKindOfADDRESSING(wordONE);
        addressingType2 = whatKindOfADDRESSING(wordTWO);
    }

    /* we will check what kind of addressing modes:
     * i.) if it's 0.   ii.)if it's 1.  iii.)if it's 2.     iv.)if it's 3
     */



    unsigned_opCODEnum = (unsigned int)opCODEnum;
    switch(opCODEnum)
    {
        case OP_mov:
        case OP_cmp:
        case OP_add:
        case OP_sub:
        case OP_lea:    /* for 2 op codes*/
            if(addressingType1 == OP_NONE || addressingType2 == OP_NONE)
            {
                return ERROR();
            }
            if(!isValidADDRESSforMethod(opCODEnum, addressingType1, addressingType2))
            {
                printf("error: addressing type1: %d\n"
                       "addressing type2: %d\n", addressingType1,
                       addressingType2);
                return ERROR();
            }
            /*first word*/
            if(checkIf_SYMBOL_LabelBefore())
                addSymbolToTheSymbolTable(helperBufferDATA, CODE_SYMBOL , lineIndex, INSTRUCTIONS_COUNTER);
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.opcode = unsigned_opCODEnum;
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.source = (unsigned int)addressingType1;
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.target = (unsigned int)addressingType2;
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.ARE = 0;
            INSTRUCTIONS_COUNTER++;
            /*the next words*/
                /*second word for 2 registers and thats it*/
            if(addressingType1 == OP_REGISTER && addressingType2 == OP_REGISTER)/*for 2 registers - share same word*/
            {
                CODE_IMAGE[INSTRUCTIONS_COUNTER-100].thirdword.source_reg = (unsigned int)getRegisterNum(wordONE);
                CODE_IMAGE[INSTRUCTIONS_COUNTER-100].thirdword.target_reg = (unsigned int)getRegisterNum(wordTWO);
                CODE_IMAGE[INSTRUCTIONS_COUNTER-100].thirdword.ARE = 0;
                INSTRUCTIONS_COUNTER++;
                break; /*todo: need to return from.....*/
            }
            /*next words if 2 registers not applied above*/
                /*will check how much words by setWords and more functionality*/
            /*todo: add more words here please */
            /*todo: add more words here please */
            /*todo: add more words here please */

            coded1 = setWords_code(wordONE, addressingType1, SOURCE);
            coded2 = setWords_code(wordTWO, addressingType2, DESTINATION);
            if(coded1 && coded2)
            {
                break;
            }
            else
            {
                return ERROR();
            }










        case OP_not:
        case OP_clr:
        case OP_inc:
        case OP_dec:
        case OP_jmp:
        case OP_bne:
        case OP_red:
        case OP_prn:
        case OP_jsr:    /*for 1 opcode*/
            /*todo:################# maybe need to fix the adrresing TYPE 1*/
            /*todo:################# maybe need to fix the adrresing TYPE 1*/
            /*todo:################# maybe need to fix the adrresing TYPE 1*/

            if(addressingType1 == OP_NONE)
            {
                printf("error: printf addressing type 1 is: %d\n", addressingType1);
                return ERROR();
            }
            /*todo:################# maybe need to fix the adrresing TYPE 1*/

            if(!isValidADDRESSfor_ONE_Method(opCODEnum, addressingType1))
            {
                return ERROR();
            }
                    /*todo:################# maybe need to fix the adrresing TYPE 1*/
            if(checkIf_SYMBOL_LabelBefore())
                addSymbolToTheSymbolTable(helperBufferDATA, CODE_SYMBOL , lineIndex, INSTRUCTIONS_COUNTER);
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.opcode = unsigned_opCODEnum;
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.target = (unsigned int)addressingType1;
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.ARE = 0;
            INSTRUCTIONS_COUNTER++;

            coded1 = setWords_code(wordONE, addressingType1, DESTINATION);
            if(coded1)
                break;
            else
                return FALSE;




        case OP_rts:
        case OP_stop:      /*without opcode*/
            if(checkIf_SYMBOL_LabelBefore())
                addSymbolToTheSymbolTable(helperBufferDATA, CODE_SYMBOL , lineIndex, INSTRUCTIONS_COUNTER);
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.opcode = unsigned_opCODEnum;
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].firstword.ARE = 0;
            INSTRUCTIONS_COUNTER++;
            break;
        default:
            break;
    }
    return TRUE;
}
int isValidADDRESSfor_ONE_Method(int opCODEnum, int addressingType1)
{
    if(((commands[opCODEnum].destination | addressingType1) == (commands[opCODEnum].destination)))
        return TRUE;
    return FALSE;
}
int setWords_code(char *word, int addressingType, int Type_destinationORsource)
{
    int value;
    int boolVal = FALSE;

    int extractedSymbolOrNumber_Value;
    char extractedSymbol_NAME[MAX_SIZE_OF_SYMBOL] = "";

    char buffer1[MAX_SIZE_OF_SYMBOL] = "";

    switch(addressingType)
    {
        case OP_IMMEDIATE:    /*adressing 0*/
            if(*word != '#')
            {
                return ERROR();
            }
            word++;
            value = atoi(word);
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].secondword.number = (unsigned int)value;
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].secondword.ARE = 0;
            INSTRUCTIONS_COUNTER++;
            boolVal = TRUE;
            break;

        case OP_DIRECT:       /*addressing 1*/
            HELP_STORE_ARRAY[HELP_ARRAY_INDEX].LOCATION = INSTRUCTIONS_COUNTER-100;
            strcpy(HELP_STORE_ARRAY[HELP_ARRAY_INDEX].symbol_name, word);
            HELP_STORE_ARRAY[HELP_ARRAY_INDEX].lineNumber = lineIndex;
            HELP_ARRAY_INDEX++;
            INSTRUCTIONS_COUNTER++;
            boolVal = TRUE;
            break;



        case OP_INDEX:        /*adressing 2*/ /*eden refaelov back to here*/
            extractedSymbolOrNumber_Value = extractSymbol_VALUE_FromIndex(word); /* the index value [inside]*/
            if(extractedSymbolOrNumber_Value == -1)
            {
                printf("error of the errors in []\n");
                return ERROR();
            }
            strcpy(extractedSymbol_NAME, extractSymbol_NAME_FromIndex(word, buffer1));
            HELP_STORE_ARRAY[HELP_ARRAY_INDEX].LOCATION = INSTRUCTIONS_COUNTER-100;
            strcpy(HELP_STORE_ARRAY[HELP_ARRAY_INDEX].symbol_name, extractedSymbol_NAME);
            HELP_STORE_ARRAY[HELP_ARRAY_INDEX].lineNumber = lineIndex;
            HELP_ARRAY_INDEX++;
            INSTRUCTIONS_COUNTER++;
            /*todo maybe*/
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].secondword.number = (unsigned int)extractedSymbolOrNumber_Value;
            /*todo maybe*/
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].secondword.ARE = 0;
            INSTRUCTIONS_COUNTER++;
            boolVal = TRUE;
            break;


        case OP_REGISTER:     /*adressing 3*/
            switch(Type_destinationORsource)
            {
                case DESTINATION:
                    CODE_IMAGE[INSTRUCTIONS_COUNTER-100].thirdword.target_reg = (unsigned int)getRegisterNum(word);
                    break;
                case SOURCE:
                    CODE_IMAGE[INSTRUCTIONS_COUNTER-100].thirdword.source_reg = (unsigned int)getRegisterNum(word);
                    break;
                default: break;
            }
            CODE_IMAGE[INSTRUCTIONS_COUNTER-100].thirdword.ARE = 0;
            INSTRUCTIONS_COUNTER++;
            boolVal = TRUE;
            break;
    }
    return boolVal;
}
char * extractSymbol_NAME_FromIndex(char *word, char *buff1)
{
    int i = 0;

    while(*word != '[')
    {
        buff1[i] = *word;
        word++;
        i++;
    }
    if(isLetterForEntryExtern(buff1))
    {
        return buff1;
    }
    else
    {
        ERROR();
        return "";
    }
}
int extractSymbol_VALUE_FromIndex(char *word)
{
    char buffH[MAX_SIZE_OF_SYMBOL] = "";
    int i = 0;
    int number = 0;

    while(*word != '[')
    {
        word++;
    }
    if(*word == '[')
        word++;
    while(*word != ']')
    {
        buffH[i] = *word;
        word++;
        i++;
    }
    if(isLabelExists(buffH))
    {
        return getLabelValue(buffH);
    }
    else if((isNumberForData(buffH)))
    {
        number = getNUMBER(buffH);
        return number;
    }
    return T_NONE;
}
int isValidADDRESSforMethod(int opcodeNUM, int address1, int address2) /*todo: check if it's ok*/
{
    if(((commands[opcodeNUM].source | address1) == (commands[opcodeNUM].source)) &&
            ((commands[opcodeNUM].destination | address2) == (commands[opcodeNUM].destination)))
        return TRUE;
    return FALSE;
}
void set2OPCODESin2Words(char *line, char *word1[], char *word2[])
{
    /*int len1, len2;*/
    char word1temp[MAX_SIZE_OF_SYMBOL] = " ";
    char word2temp[MAX_SIZE_OF_SYMBOL] = " ";
    *word1 = malloc(MAX_SIZE_OF_SYMBOL);
    *word2 = malloc(MAX_SIZE_OF_SYMBOL);
    /*word 1 */
    if(*line != ' ')
    {
        strcpy(*word1, getFirstWord(line, word1temp));                    /*first word inside wordBuff*/
        /*len1 = strlen(*word1);                                     */   /*gets the length of the word*/
        line = deletePrevWord(line, *word1);

    }
    /*check between*/
    line = deleteFirstDelimeters(line);
    if(*line == ',')
    {
        line = line+1;
        line = deleteFirstDelimeters(line);
        /*word 2 */
        strcpy(*word2, getFirstWord(line, word2temp));                    /*first word inside wordBuff*/
        /*len2 = strlen(*word2); */                                       /*gets the length of the word*/
        line = deletePrevWord(line, *word2);
        line = deleteFirstDelimeters(line);
    }
    else if(*line != ',')
    {
        *word2 = NULL;
        /*
        errorFlag++;
        printf("error opcode\n");
        exit(0);
        return ERROR_VOID();
         */
    }
}
int whatKindOfADDRESSING(char *word)
{
    int reg;

    /*return register ADDRESSING --- 3 */
    reg = getRegisterNum(word);
    if(reg != -1)
    {
        return OP_REGISTER;
    }

    /*return immediate number ADDRESSING --- 3 */
    if(*word == '#')
    {
        word = word + 1;
        if(isNumber(word))
        {
            return OP_IMMEDIATE;
        }
        else
        {
            return ERROR_MINUS();
        }
    }
    /*return direct ADDRESSING --- 1 */
    if(isLetterFOR_OPCODE(word))
    {
        if(isLabelExists(word))
        {
            return OP_DIRECT;
        }
        else
        {
            /*todo*/
            /*todo*/
            /*todo*/
            *SavedLabelsForLater = word;    /*todo:*/
            return OP_DIRECT;
        }
    }
    /*return index ADDRESSING --- 2 */
    /*todo: todo todo:todo      index addressing LIST[sz] for example*/
    if(isINDEX(word))
    {
        return OP_INDEX;
    }
    return OP_NONE;
}
int isINDEX(char *word)
{
    int i=0;
    if(isalpha(*word)) /* first letter much be a letter */
    {
        word = word+1;
        i++;
        while(isalpha(*word) || isdigit(*word))
        {
            word = word + 1;
            i++;
        }
        if(*word == '[')
            return i;
    }
    return FALSE;
}
int isLetterFOR_OPCODE(char *line)
{
    int i=0;
    if(isalpha(*line)) /* first letter much be a letter */
    {
        line = line+1;
        i++;
        while(isalpha(*line) || isdigit(*line)) /*can be a letter or a number a123 asdg...*/
        {
            line = line + 1;
            i++;
        }
        if(*line == '[')
        {
            return FALSE;
        }
        return i;
    }
    return FALSE;
}

int isEntry(char *line, int indexOfSavedWord)
{
    int ValidSymbol;
    char helpBUFFING[MAX_SIZE_OF_SYMBOL] = "";
    if(!ValidSpace(line))
    {
        ERROR();
    }
    line = deleteFirstDelimeters(line);
    ValidSymbol = isLetterForEntryExtern(line);
    strcpy(helpBUFFING, getFirstWord(line, helpBUFFING));
    line = line + ValidSymbol;
    line = deleteFirstDelimeters(line);
    if(*line == '\n' || *line == '\0')
    {
        if(isLabelExists(helpBUFFING))
        {
            fprintf(fn, "name: %s line index: %d value: %d\n", helpBUFFING, lineIndex, getLabelValue(helpBUFFING));
            return TRUE;
        }
        strcpy(HELP_STORE_ARRAY[HELP_ARRAY_INDEX].symbol_name, helpBUFFING);
        HELP_STORE_ARRAY[HELP_ARRAY_INDEX].lineNumber = lineIndex; /*todo*/
        HELP_STORE_ARRAY[HELP_ARRAY_INDEX].Entry = TRUE; /*todo*/

        HELP_ARRAY_INDEX++;
        return TRUE;
    }
    else
    {
        return ERROR();
    }
}
int isExtern(char *line, int indexOfSavedWord)
{
    int ValidSymbol;
    char helpBUFFING[MAX_SIZE_OF_SYMBOL] = "";
    if(!ValidSpace(line))
    {
        ERROR();
    }
    line = deleteFirstDelimeters(line);
    ValidSymbol = isLetterForEntryExtern(line);
    strcpy(helpBUFFING, getFirstWord(line, helpBUFFING));
    line = line + ValidSymbol;
    line = deleteFirstDelimeters(line);
    if(*line == '\n' || *line == '\0')
    {
        if(isLabelExists(helpBUFFING))
        {
            fprintf(fx, "name: %s line index: %d value: %d\n", helpBUFFING, lineIndex, getLabelValue(helpBUFFING));
            return TRUE;
        }
        addSymbolToTheSymbolTable(helpBUFFING, EXTERNAL_SYMBOL, lineIndex, 0);/*todo:fix it*/
        return TRUE;
    }
    else
    {
        printf("there is overwords after the symbol, ERROR");
        return ERROR();
    }
}
int isValidAddressingMode(const char* cmd, ADDRESSING_MODE mode, int isDestination)
{
    int srcAddrMode = 0;
    int destAddrMode = 0;
    int result = FALSE;

    int i;
    for (i=0; i < (sizeof(commands)/sizeof(commands[0])); i++)
    {
        if(strcmp(cmd, commands[i].name) == 0)
        {
            srcAddrMode = commands[i].source;
            destAddrMode = commands[i].destination;
        }
    }

    if (isDestination) {/*Destination operand*/
        result = destAddrMode & mode;
    }else{              /*Source operand*/
        result = srcAddrMode & mode;
    }

    return result;
}
int getRegisterNum(char* str)
{
    int i;
    int index = sizeof(registers)/sizeof(registers[0]);
    for (i=0; i < index; i++)
    {
        if(strcmp(str, registers[i].name) == 0){
            return registers[i].number;
        }
    }
    return -1;
}
int isValidRegister(char* str)
{
    int index = sizeof(registers)/sizeof(registers[0]);
    int i;
    for (i=0; i < index; i++)
    {
        if(strcmp(str, registers[i].name) == 0)
            return TRUE;
    }
    return FALSE;
}
int getCommandNUM(const char *str)
{
    int i;
    int index = sizeof(commands)/sizeof(commands[0]);
    for (i=0; i < index; i++)
    {
        if(strcmp(str, commands[i].name) == 0)
        {
            return commands[i].opcodeNUM;
        }
    }
    return -1;
}
int isValidCommand(const char *str)
{
    int i;
    int index = sizeof(commands)/sizeof(commands[0]);
    for (i=0; i < index; i++)
    {
        if(strcmp(str, commands[i].name) == 0)
            return TRUE;
    }
    return FALSE;
}
/*#####################################################~~~~~~~~~~~~~~~~~~################################*/



























int isString(char *line, int indexOfSavedWord)
{
    if(!isspace(*line))
    {
        return ERROR();
    }
    line = deleteFirstDelimeters(line);
    if(arrayStrings(line))
    {
        return TRUE;
    }
    return FALSE;
}
int isData(char *line, int indexOfSavedWord) /*todo: needs to check if it's .data before and strtok it*/
{
    if(!isspace(*line))
    {
        return ERROR();
    }
    line = deleteFirstDelimeters(line);
    if(arrayNumbers(line))
    {
        printf("****************\narray NUMBER success!!!\n******************\n");
        return TRUE;
    }
    return FALSE;
}

int isSymbol(char *pointer)
{
    if(isalpha(*pointer))
    {
        while(*pointer != '\0' && *pointer != ':')
        {
            if(isalpha(*pointer) || isdigit(*pointer))
                pointer++;
            else
                return FALSE;
        }
        if(*pointer == ':')
        {
            return TRUE;
        }
    }
    return FALSE;
}
int isitHashTag(char *line)
{
    if(*line == '#')
        return TRUE;
    return FALSE;
}
int trueLabel(char *line)
{
    if(*line == ':')
        return TRUE;
    return FALSE;
}
void initializeBuffer(char *buffer)
{
    int i;
    for(i=0; i<MAX_SIZE_OF_SYMBOL; i++)
    {
        buffer[i] = '\0';
    }
}
int checkWordLength(int indexCount)
{
    if(indexCount > MAX_SIZE_OF_SYMBOL)
        return FALSE;
    return TRUE;
}
int isLetter(char *line)
{
    int i=0;
    if(isalpha(*line)) /* first letter much be a letter */
    {
        line = line+1;
        i++;
        while(isalpha(*line) || isdigit(*line)) /*can be a letter or a number a123 asdg...*/
        {
            line = line + 1;
            i++;
        }
        if(!isspace(*line))
        {
            return ERROR();
        }
        return i;
    }
    return FALSE;
}
int isLetterForEntryExtern(char *line)
{
    int i=0;
    if(isalpha(*line)) /* first letter much be a letter */
    {
        line = line+1;
        i++;
        while(isalpha(*line) || isdigit(*line)) /*can be a letter or a number a123 asdg...*/
        {
            line = line + 1;
            i++;
        }
        return i;
    }
    return FALSE;
}
char* deleteFirstDelimeters(char* line)
{
    while(*line == '\t' || *line == ' ')
    {
        line++;
    }

    return line;
}
int isEmpty(char* line)
{
    while((*line != '\0') && (*line != ';'))
    {
        if(!isspace(*line))
        {
            return FALSE;
        }
        line++;
    }
    return TRUE;
}
int checkOneDelimeterAndDelete(char* line)
{
    if (*line == ' ' || *line == '\t')
    {
        deleteFirstDelimeters(line);
        return TRUE;
    }
    return FALSE;
}
int isNumber(char *str)
{
    int increaseNumber = 0;
    if (*str == '-' || *str == '+')
    {
        str++;
        increaseNumber++;
    }
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
        {
            if(*str == '\n')
                return increaseNumber;
            return FALSE;
        }
        increaseNumber++;
        str++;
    }
    return increaseNumber;
}
int isNumberForData(char *str)
{
    int increaseNumber = 0;
    if (*str == '-' || *str == '+')
    {
        str++;
        increaseNumber++;
    }
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
        {
            if(*str == ',')
            {
                return increaseNumber;
            }
            else if(*str == ' ')
            {
                str++;
                return ++increaseNumber;
            }
        }
        increaseNumber++;
        str++;
    }
    return increaseNumber;
}
int arrayStrings(char *str)
{
    /*char stringData[MAX_LINE_SIZE];*/
    int i = 0;
    int len;





    if(*str != '"')
    {
        printf("String ERROR");
        return ERROR();
    }
    str++;

    len = strlen(str);
    len--;
    while(len != -1)
    {
        if(isspace(str[len]) || str[len] == ' ' || str[len] == '\t' || str[len] == '\n')
        {
            len--;
        }
        else if(str[len] == '"')
        {
            break;
        }
        else
        {
            printf("error\n");
            return FALSE;
        }
    }
    if(len == -1)
        return FALSE;
    while(i<len)
    {
        DATA_IMAGE[DATA_COUNTER].DATA = (unsigned char)str[i];
        DATA_COUNTER++;
        DC_TEMP++;
        i++;
    }
    DATA_IMAGE[DATA_COUNTER].DATA = '\0';
    DATA_COUNTER++;
    DC_TEMP++;
    return TRUE;
}
int arrayNumbers(char *str)
{
    int numberCheck;
    int commaCheck;
    int checkLEN;
    int value;
    /*char strhelper[MAX_SIZE_OF_SYMBOL] = "";*/
    char labelHelper[MAX_SIZE_OF_SYMBOL] = " ";
    int flagPrevNumber = FALSE;
    int flagNOW = FALSE;
    while(*str != '\0')
    {
        str = deleteFirstDelimeters(str);
        numberCheck = isNumberForData(str);
        value = getNUMBER(str);
        if(flagPrevNumber == FALSE && numberCheck)/*todo: need to add labels as numbers*/
        {
            if(isalpha(*str))
            {
                if(isalpha(*str))
                {
                    strcpy(labelHelper, getWORD(str, labelHelper));
                    if (isLabelExists(labelHelper))
                    {/*back*/
                        checkLEN = strlen(labelHelper);
                        str += checkLEN;
                        flagNOW = TRUE;
                        DATA_IMAGE[DATA_COUNTER].DATA = (unsigned int)getLabelValue(labelHelper);
                        DATA_COUNTER++;
                        DC_TEMP++;
                    }
                    else
                    {
                        errorFlag++;
                        printf("exits not working!\n");
                        exit(0);
                    }
                }
            }
            if(flagNOW)
            {
                flagNOW = FALSE;
            }
            else
            {
                str = str + numberCheck;
                flagPrevNumber = TRUE;
                if(value)
                {
                    DATA_IMAGE[DATA_COUNTER].DATA = (unsigned int)value;
                    DATA_COUNTER++;
                    DC_TEMP++;
                    /*back*/
                }
            }
        }
        else if((flagPrevNumber == TRUE) && (commaCheck = checkComma(str)))
        {
            str = str + commaCheck;
            flagPrevNumber = FALSE;
        }
        else if(isspace(*str))
        {
            str = deleteFirstDelimeters(str);
        }
        else
        {
            errorFlag++;
            return FALSE;
        }
    }
    return TRUE;
}
int getNUMBER(char *str)
{
    char numberBuffer[MAX_SIZE_OF_SYMBOL] = "";
    int increaseNumber = 0;
    int number;
    int i = 0;

    if (*str == '-' || *str == '+')
    {
        numberBuffer[i] = *str;
        str++;
        increaseNumber++;
        i++;
    }
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
        {
            if(*str == ',')
            {
                number = atoi(numberBuffer);
                return number;
            }
            else if(*str == ' ')
            {
                /*
                str++;
                ++increaseNumber;
                 */
                number = atoi(numberBuffer);
                return number;
            }
        }
        numberBuffer[i] = *str;
        i++;
        increaseNumber++;
        str++;
    }
    number = atoi(numberBuffer);
    return number;
}
int checkComma(char *str)
{
    str = deleteFirstDelimeters(str);
    if(*str == ',')
        return TRUE;
    printf("not a Comma\n");
    errorFlag++;
    return FALSE;
}
int ValidSpace(char *line)
{
    if(isspace(*line))
        return TRUE;
    return FALSE;
}
char* deletePrevWord(char *line, char *word)
{
    line = deleteFirstDelimeters(line) + strlen(word);
    return line;
}
char* getWORD(char* line, char *buff)                                    /*getting the First word in the line */
{
    int i = 0;
    line = deleteFirstDelimeters(line);
    if(isdigit(*line))
    {
        printf("exits error error errror\n");
        /*exit(0);*/
        ERROR_char();
    }
    while((!isEmpty(line)) && *line != '\t' && *line != ' ' && *line != ':' && *line != ',')
    {
        buff[i] = *line;
        line = line+1;
        i++;
    }

    if(!checkWordLength(i))
    {
        printf("error check me\n");
        printf("\n***************\nerror flag count: %d\n***************\n", errorFlag);
        exit(0);
    }
    return buff;
}
int ERROR()
{
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    printf("There is an error in line: %d\n", lineIndex);
    printf("will jump now to the next line number %d\n", lineIndex+1);
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    errorFlag++;
    return FALSE;
}
int ERROR_MINUS()
{
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    printf("There is an error in line: %d\n", lineIndex);
    printf("will jump now to the next line number %d\n", lineIndex+1);
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    errorFlag++;
    return T_NONE;
}
char * ERROR_char()
{
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    printf("There is an error in line: %d\n", lineIndex);
    printf("will jump now to the next line number %d\n", lineIndex+1);
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    errorFlag++;
    return NULL;
}
void ERROR_VOID()
{
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    printf("There is an error in line: %d\n", lineIndex);
    printf("will jump now to the next line number %d\n", lineIndex+1);
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    errorFlag++;
    return;
}
int ERROR_HELPER_LINENUM(int i)
{
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    printf("There is an error in line: %d\n", HELP_STORE_ARRAY[i].lineNumber);
    printf("\n\t\t**ALERT**\t**ALERT**\t**ALERT**\t\n");
    errorFlag++;
    return FALSE;
}

/*#############*/
/*#Linked List#*/
/*#############*/
void freeSymbolTable()
{
    free(SYMBOL_TABLE -> symbolTypeAndValue);
}
void addSymbolToTheSymbolTable(char *symbolName, LABEL_TYPE symbolType,
                               int lineIndex, int value)
{
    /*the first thing , we need to check if the symbol exist , lets assume that the symbol does not*/
    if (!SYMBOL_TABLE)
    {
        SYMBOL_TABLE = calloc(1, sizeof(SymbolTable));
        SYMBOL_TABLE->size = 0;
        SYMBOL_TABLE->symbolTypeAndValue = calloc(1,
                                                  sizeof(LabelAddressPair));
    }
    else
    {

        SYMBOL_TABLE -> symbolTypeAndValue = realloc(
                SYMBOL_TABLE->symbolTypeAndValue,
                ((SYMBOL_TABLE->size) + 1) * sizeof(LabelAddressPair));
    }
    strcpy(
            (SYMBOL_TABLE->symbolTypeAndValue + (SYMBOL_TABLE->size))->symbolName,
            symbolName);
    (SYMBOL_TABLE->symbolTypeAndValue + (SYMBOL_TABLE->size))->symbolType =
            symbolType;

    if (symbolType == CODE_SYMBOL)
    {
        (SYMBOL_TABLE->symbolTypeAndValue + (SYMBOL_TABLE->size))->value =
                INSTRUCTIONS_COUNTER;
    }
    else if (symbolType == DATA_SYMBOL)
    {
        (SYMBOL_TABLE->symbolTypeAndValue + (SYMBOL_TABLE->size))->value =
                INSTRUCTIONS_COUNTER; /*eden need to change*/

    }
    else if (symbolType == EXTERNAL_SYMBOL || symbolType == ENTRY_SYMBOL)
    {
        (SYMBOL_TABLE->symbolTypeAndValue + (SYMBOL_TABLE->size))->value = 0;

    }
    else if (symbolType == MACRO_SYMBOL)
    {
        (SYMBOL_TABLE->symbolTypeAndValue + (SYMBOL_TABLE->size))->value =
                value;

    }
    SYMBOL_TABLE->size++;
}

void printSymbolTable()
{
    char *savedProperties[] = {"DATA_SYMBOL", "CODE_SYMBOL", "EXTERNAL_SYMBOL", "ENTRY_SYMBOL", "MACRO_SYMBOL"};
    printf("\t\t\tSYMBOL TABLE\n\n");
    if(!SYMBOL_TABLE)
    {
        printf("Nothing to print...");
    }
    else
    {


        int i = 0;
        printf("SYMBOL\t\tProperties\t\tValue\t\tnodeNumber\n");
        while (i < SYMBOL_TABLE -> size )
        {

            printf("%s\t\t%s\t\t%d\t\t%d\n ",
                   (SYMBOL_TABLE -> symbolTypeAndValue+i)  -> symbolName,
                   savedProperties[(SYMBOL_TABLE ->symbolTypeAndValue+i)->symbolType],
                   (SYMBOL_TABLE ->symbolTypeAndValue+i)->value,i);
            i++;
        }
    }
}

int isLabelExists(char* label)
{
    int i = 0;
    while (SYMBOL_TABLE && i < SYMBOL_TABLE -> size && label) {
        if (strcmp((SYMBOL_TABLE -> symbolTypeAndValue + i) -> symbolName, label) == 0) {
            return i+1;    /*todo:fix*/
        }
        i++;
    }
    return FALSE;
}

int getLabelValue(char* label)
{
    int i = 0;
    while (i < SYMBOL_TABLE -> size && label) {
        if (strcmp((SYMBOL_TABLE -> symbolTypeAndValue + i) -> symbolName, label) == 0) {
            return (SYMBOL_TABLE -> symbolTypeAndValue + i) -> value;
        }
        i++;
    }
    return -1;
}

void debuggingDATAIMAGE()
{
    int i;
    printf("\n\n");
    for(i=0; i<DATA_COUNTER; i++)
    {
        printf("\t\tThe data in [%d] position is: %d\n", i, DATA_IMAGE[i].DATA);
    }
}
void HelpStoreArrayPrint()
{
    int i;

    for(i=0; i<HELP_ARRAY_INDEX;i++)
    {
        printf("EDEN REFAELOV HELP ARRAY LABEL IS: %s\n", HELP_STORE_ARRAY[i].symbol_name);
    }
}

int isSymbolInsideTheHelperStruct()
{
    int i = 0;
    int TRUEcounter = 0;
    int PropertiesSymbolType;

    for(i=0; i<HELP_ARRAY_INDEX;i++)
    {
        /*    printf("british is: %s\n", HELP_STORE_ARRAY[i].symbol_name);
        printf("val is: %d\n", HELP_STORE_ARRAY[i].LOCATION+100);    */

        if(isLabelExists(HELP_STORE_ARRAY[i].symbol_name))
        {
            TRUEcounter++;
            PropertiesSymbolType = isLabelExists(HELP_STORE_ARRAY[i].symbol_name)-1;
            PropertiesSymbolType = (SYMBOL_TABLE->symbolTypeAndValue+PropertiesSymbolType)->symbolType;

            if(PropertiesSymbolType == DATA_SYMBOL && HELP_STORE_ARRAY[i].Entry == FALSE)
            {
                CODE_IMAGE[HELP_STORE_ARRAY[i].LOCATION].secondword.number =
                        DATA_IMAGE[getLabelValue(HELP_STORE_ARRAY[i].symbol_name) - INSTRUCTIONS_COUNTER_HELPER].DATA;
                CODE_IMAGE[HELP_STORE_ARRAY[i].LOCATION].secondword.ARE = 1;
            }
            else if(PropertiesSymbolType == CODE_SYMBOL)
            {
                printf("miss\n");
            }
            else if(PropertiesSymbolType == EXTERNAL_SYMBOL)
            {

                CODE_IMAGE[HELP_STORE_ARRAY[i].LOCATION].secondword.number = 0;
                CODE_IMAGE[HELP_STORE_ARRAY[i].LOCATION].secondword.ARE = 1;
                fprintf(fx, "name: %s line index: %d value: %d\n", HELP_STORE_ARRAY[i].symbol_name,
                        HELP_STORE_ARRAY[i].lineNumber, HELP_STORE_ARRAY[i].LOCATION+100);
            }


            if(HELP_STORE_ARRAY[i].Entry == TRUE)
            {
                fprintf(fn, "name: %s line index: %d value: %d\n", HELP_STORE_ARRAY[i].symbol_name,
                        HELP_STORE_ARRAY[i].lineNumber, getLabelValue(HELP_STORE_ARRAY[i].symbol_name));
            }
                /*todo: else if*/

        }

        else
        {
            printf("\t\t****BUG****\t\t\nThe symbol name bugged is: %s\n", HELP_STORE_ARRAY[i].symbol_name);
            return ERROR_HELPER_LINENUM(i);
        }
    }
    fclose(fx);
    fclose(fn);
    return TRUEcounter;
}

void resetALL()
{
    int i;
    printf("RESET STARTS\n");
    INSTRUCTIONS_COUNTER = 100;
    DATA_COUNTER = 0;
    lineIndex = 0;
    HELP_ARRAY_INDEX = 0;
    errorFlag = 0;
    freeSymbolTable();
    initializeBuffer(helperBufferDATA);
    initializeBuffer(helperBufferOPCODE);
    DC_TEMP = 0;

    for(i=0; i<=1000;i++)
    {
        CODE_IMAGE[i].zero = 0;
        DATA_IMAGE[i].DATA = 0;
        HELP_STORE_ARRAY[i].lineNumber = HELP_STORE_ARRAY2[i].lineNumber;
        strcpy(HELP_STORE_ARRAY[i].symbol_name, HELP_STORE_ARRAY2[i].symbol_name);
        HELP_STORE_ARRAY[i].LOCATION = HELP_STORE_ARRAY2[i].LOCATION;
    }
}

void prnt1()
{
    int i;
    fprintf(fs,"\t%d\t%d\n",INSTRUCTIONS_COUNTER_HELPER-100,DATA_COUNTER);
    for(i=100;i<INSTRUCTIONS_COUNTER_HELPER;i++){
        unsigned short *x;
        unsigned short y;
        x=(unsigned short*)(&CODE_IMAGE[i-100]);
        y=*x;
        fprintf(fs,"0%d\t",i);
        SpecialBaseFour(y);
    }
}
void prnt2()
{
    int i;
    for(i=INSTRUCTIONS_COUNTER_HELPER;i<INSTRUCTIONS_COUNTER_HELPER+DATA_COUNTER;i++){
        unsigned short *x;
        unsigned short y;
        x=(unsigned short*)(&DATA_IMAGE[i-INSTRUCTIONS_COUNTER_HELPER]);
        y=*x;
        fprintf(fs,"0%d\t",i);
        SpecialBaseFour(y);
    }
}

void SpecialBaseFour(unsigned short x)
{
    int i;
    char Base4[8];
    for(i=6;i>=0;i--){
        if((x&3)==0)/*3 is '11' in binary ,so this way we isolated the 2 first bits*/
            Base4[i]='*';
        else if((x&3)==1)
            Base4[i]='#';
        else if((x&3)==2)
            Base4[i]='%';
        else
            Base4[i]='!';
        x=x>>2;
    }
    Base4[8-1]='\0';/*to make it a string, I put \0 at end*/
    fprintf(fs,"%s\n",Base4);

}
