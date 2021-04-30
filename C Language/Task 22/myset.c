/*
Name: Maman 22
Author: Gil Or
*/

/*Liabraries*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "set.h"
#include "set.c"

/*initialized the sets to empty sets*/
set SETA = {0};
set SETB = {0};
set SETC = {0};
set SETD = {0};
set SETE = {0};
set SETF = {0};


int main() {


    /*Variables*/
    int commandName;
    int numbers [COMMAND_LINE_LENGTH] = {0}; /*for read_set command*/
    int stopFlag = 0; 	/* to idetify when stop command is recived*/
    char commandLine[COMMAND_LINE_LENGTH] = {0};
    char *pCommandLine=(char*) calloc(COMMAND_LINE_LENGTH,sizeof(char));
    pset setFirst = NULL;
    pset setSecond = NULL;
    pset setThird = NULL;


    while (stopFlag == 0) {
        printf("\nPlease enter your commend:\n");
        fgets(pCommandLine, COMMAND_LINE_LENGTH, stdin);

        /*if reach EOF*/
        if(pCommandLine == NULL) {
            printf("%s",ERROR_EOF_TEXT);
            return 0;
        }
        /*prints the line, for case when reading from file input*/
        strcpy(commandLine, pCommandLine);
        printf("%s", commandLine);

        /* Analyze the input and return an int represents the command*/
        commandName = commandAnalysis(commandLine,numbers,&setFirst,&setSecond,&setThird);

        /*If an error*/
        if ((commandName <= ERROR_EOF_SIGH) && (commandName >= ERROR_COMMAND_NAME_SIGH)) {
            char* errorText;
            errorText = errorMassage(commandName);
            printf("%s",errorText);
        }

        if (commandName == STOP) {
            stopFlag=1;
        }

        if (commandName == READ_SET) {
            read_set(setFirst,numbers);
        }

        if (commandName == PRINT_SET) {
            print_set(setFirst);
        }

        if (commandName == UNION_SET) {
            union_set (setFirst,setSecond,setThird);
        }

        if (commandName == INTERSECT_SET) {
            intersect_set (setFirst,setSecond,setThird);
        }

        if (commandName == SUB_SET) {
            sub_set (setFirst,setSecond,setThird);
        }

        if (commandName == SYMDIFF_SET) {
            symdiff_set (setFirst,setSecond,setThird);
        }
    }


    printf("Thanks for using the interactive sets calculator!");
    return 0;
}


/*Analyze the command. If valid, return an int representing the command, otherwise the sign of the specific error*/

int commandAnalysis (char* command, int numbers [] , set** firstSet, set** secondSet, set** thirdSet) {

    char ch;
    char *commandName = (char*) calloc(COMMAND_LINE_LENGTH,sizeof(char));
    char *set1 = (char*) calloc(5,sizeof(char));
    pset pSetTemp;
    int i=0;
    int j=0;
    int commandSign;

    /*ignore white spaces*/
    while (isspace(ch=command[i])!=0) { i++; }

    /*read the command*/
    while (islower(ch=command[i])!=0|| ch=='_') {
        commandName[j]=ch;
        i++;
        j++;
    }
    commandSign = isCommand(commandName);

    /*if command name is wrong return error*/
    if (commandSign == FALSE) {
        free (commandName);
        free (set1);
        return ERROR_COMMAND_NAME_SIGH;
    }

    /*if stop command*/
    if (commandSign == STOP) {
        free (commandName);
        free (set1);
        return STOP;
    }

    /*get the first set*/
    while (isspace(ch=command[i])!=0) { i++; }
    if (ch==',') {
        free (commandName);
        free (set1);
        return ILLEGAL_COMMA_SIGH;
    }
    j=0;
    while (isupper(ch=command[i])!=0) {
        set1[j]=ch;
        i++;
        j++;
    }
    pSetTemp = isSet(set1);
    if (pSetTemp == NULL) {
        free (commandName);
        free (set1);
        return ERROR_SET_NAME_SIGH;
    }
    /*Direct the set pointer to pointer the correct set*/
    *firstSet=pSetTemp;
    while (isspace(ch=command[i])!=0) { i++; }

    /*if print set command*/
    if (commandSign == PRINT_SET) {
        free (commandName);
        free (set1);
        if (ch=='\n' || ch=='\0') {
            return commandSign;
        }
        return EXTRANEOUS_TEXT_SIGH;
    }

    /*if read_set command, expecting to read numbers*/
    if (commandSign == READ_SET){
        char *numberString = (char*) calloc(COMMAND_LINE_LENGTH,sizeof(char));
        int number;
        int numbersCounter=0;

        /*check for a comma*/
        if (ch!=',') {
            free (numberString);
            free (commandName);
            free (set1);
            return MISSING_COMMAS_SIGH;
        }
        i++;
        while (isspace(ch=command[i])!=0) { i++; }

        /*read the numbers*/
        while (1) {
            j=0;
            if(ch==',') {
                free (numberString);
                free (commandName);
                free (set1);
                return CONSECUTIVE_COMMAS_SIGH;
            }
            if(ch=='\0') {
                free (numberString);
                free (commandName);
                free (set1);
                return ERROR_TERMINATED_INCORRECTLY_SIGH;
            }
            if(ch=='-') {
                numberString[j]=ch;
                i++;
                ch=command[i];
                if (ch=='1') {
                    /*identifier of the end of the array*/
                    numbers[numbersCounter]=-1;
                    break;
                }
                free (numberString);
                free (commandName);
                free (set1);
                return ERROR_OUT_OF_RANGE_SIGH;
            }
            if(ch=='0') {
                i++;
                while (isspace(ch=command[i])!=0) { i++; }
                if(ch!=',') {
                    free (numberString);
                    free (commandName);
                    free (set1);
                    return ERROR_OUT_OF_RANGE_SIGH;
                }
                numbers[numbersCounter]=0;
                numbersCounter++;
                i++;
                while (isspace(ch=command[i])!=0) { i++; }
                continue;
            }
                /*if not '-' /'0' / ',' / NULL */
            else {
                j=0;
                while (ch!=',') {
                    if (ch == '\n' || ch == '\0') {
                        free (numberString);
                        free (commandName);
                        free (set1);
                        return ERROR_TERMINATED_INCORRECTLY_SIGH;
                    }
                    if (isspace(ch)!=0) {
                        while (isspace(ch=command[i])!=0) { i++; }
                        if (ch!=',') {
                            free (numberString);
                            free (commandName);
                            free (set1);
                            return MISSING_COMMAS_SIGH;
                        }
                        break;
                    }
                    if(isdigit(ch) == 0) {
                        free (numberString);
                        free (commandName);
                        free (set1);
                        return ERROR_INVALID_NUMBER_SIGH;
                    }
                    numberString[j]=ch;
                    j++;
                    i++;
                    ch=command[i];
                }
                if (ch == '-') {
                    continue;
                }
                /*convert the string to integer*/
                number = atoi(numberString);
                memset(numberString,'\0',3);
                if (number==0) {
                    free (numberString);
                    free (commandName);
                    free (set1);
                    return ERROR_INVALID_NUMBER_SIGH;
                }
                /*check if out of range*/
                if ( number > UPPER_LIMIT ) {
                    free (numberString);
                    free (commandName);
                    free (set1);
                    return ERROR_OUT_OF_RANGE_SIGH;
                }
                numbers[numbersCounter]=number;
                numbersCounter++;
                i++;
                while (isspace(ch=command[i])!=0) { i++; }
            }
        }
        free (numberString);
        free (commandName);
        free (set1);
        return commandSign;
    }

    /*if union / intersect / sub / symdiff sets command, expecting to read additional 2 sets*/

    /*check for a comma*/
    if (ch!=',') return MISSING_COMMAS_SIGH;
    i++;
    while (isspace(ch=command[i])!=0) { i++; }

    /*read the second set*/
    if (ch==',') return CONSECUTIVE_COMMAS_SIGH;
    if (ch=='\n' || ch=='\0') return MISSING_PARAMETER_SIGH;
    j=0;
    while (isupper(ch=command[i])!=0) {
        set1[j]=ch;
        i++;
        j++;
    }
    pSetTemp = isSet(set1);
    if (pSetTemp == NULL) return ERROR_SET_NAME_SIGH;
    /*Direct the set pointer to pointer the correct set*/
    *secondSet=pSetTemp;
    while (isspace(ch=command[i])!=0) { i++; }

    /*read the third set*/
    if (ch == '\0') return MISSING_PARAMETER_SIGH;
    if (ch!=',') return MISSING_COMMAS_SIGH;
    i++;
    while (isspace(ch=command[i])!=0) { i++; }
    if (ch==',') return CONSECUTIVE_COMMAS_SIGH;
    if (ch=='\n' || ch=='\0') return MISSING_PARAMETER_SIGH;
    j=0;
    while (isupper(ch=command[i])!=0) {
        set1[j]=ch;
        i++;
        j++;
    }
    pSetTemp = isSet(set1);
    if (pSetTemp == NULL) return ERROR_SET_NAME_SIGH;
    /*Direct the set pointer to pointer the correct set*/
    *thirdSet=pSetTemp;
    while (isspace(ch=command[i])!=0) { i++; }
    if (ch!='\n' && ch!='\0') return EXTRANEOUS_TEXT_SIGH;
    return commandSign;
}


/*Supporting Functions*/

/*The function gets the name (string) of the command and return an integer reporesents the cooresponding command*/
int isCommand (char* command){

    if (strcmp(command,"read_set")      == 0) return READ_SET;
    if (strcmp(command,"print_set")     == 0) return PRINT_SET;
    if (strcmp(command,"union_set")     == 0) return UNION_SET;
    if (strcmp(command,"intersect_set") == 0) return INTERSECT_SET;
    if (strcmp(command,"sub_set")       == 0) return SUB_SET;
    if (strcmp(command,"symdiff_set")   == 0) return SYMDIFF_SET;
    if (strcmp(command,"stop")          == 0) return STOP;
    if (strcmp(command,"\n")            == 0) return EMPTY;
    return FALSE;
}

/*The function gets the sign of the error and return the corresponding error massage*/
char* errorMassage (int sign) {

    char* errorText = (char*) calloc(ERROR_LENGTH, sizeof(char ));

    if (sign == ERROR_COMMAND_NAME_SIGH)               errorText=ERROR_COMMAND_NAME_TEXT;
    if (sign == ERROR_OUT_OF_RANGE_SIGH)               errorText=ERROR_OUT_OF_RANGE_TEXT;
    if (sign == ERROR_TERMINATED_INCORRECTLY_SIGH)     errorText=ERROR_TERMINATED_INCORRECTLY_TEXT;
    if (sign == ERROR_INVALID_NUMBER_SIGH)             errorText=ERROR_INVALID_NUMBER_TEXT;
    if (sign == MISSING_PARAMETER_SIGH)                errorText=MISSING_PARAMETER_TEXT;
    if (sign == EXTRANEOUS_TEXT_SIGH)                  errorText=EXTRANEOUS_TEXT_TEXT;
    if (sign == CONSECUTIVE_COMMAS_SIGH)               errorText=CONSECUTIVE_COMMAS_TEXT;
    if (sign == MISSING_COMMAS_SIGH)                   errorText=MISSING_COMMAS_TEXT;
    if (sign == ILLEGAL_COMMA_SIGH)                    errorText=ILLEGAL_COMMA_TEXT;
    if (sign == ERROR_EOF_SIGH)                        errorText=ERROR_EOF_TEXT;
    if (sign == ERROR_SET_NAME_SIGH)                   errorText=ERROR_SET_NAME_TEXT;

    return errorText;

}


/*the function gets the name of the set as a string and returns a pointer of the corresponding set, if the name of the set is not corret returns NULL*/
pset isSet (char* set ){

    if (strcmp(set,"SETA")==0) {
        return &SETA;
    }
    if (strcmp(set,"SETB")==0) {
        return &SETB;
    }
    if (strcmp(set,"SETC")==0) {
        return &SETC;
    }
    if (strcmp(set,"SETD")==0) {
        return &SETD;
    }
    if (strcmp(set,"SETE")==0) {
        return &SETE;
    }
    if (strcmp(set,"SETF")==0) {
        return &SETF;
    }
    return NULL;
}

