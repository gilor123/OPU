/*Defenitions*/

#define UPPER_LIMIT 127
#define FALSE (-1)

#define READ_SET 1
#define PRINT_SET 2
#define UNION_SET 3
#define INTERSECT_SET 4
#define SUB_SET 5
#define SYMDIFF_SET 6
#define STOP 7
#define EMPTY 8

#define COMMAND_LINE_LENGTH 80
#define ERROR_LENGTH 60

#define ERROR_COMMAND_NAME_SIGH 10
#define ERROR_COMMAND_NAME_TEXT "Undefined command name\n"
#define ERROR_SET_NAME_SIGH 11
#define ERROR_SET_NAME_TEXT "Undefined/missing set name\n"
#define ERROR_OUT_OF_RANGE_SIGH 12
#define ERROR_OUT_OF_RANGE_TEXT "Invalid set member – value out of range\n"
#define ERROR_TERMINATED_INCORRECTLY_SIGH 13
#define ERROR_TERMINATED_INCORRECTLY_TEXT "List of set members is not terminated correctly\n"
#define ERROR_INVALID_NUMBER_SIGH 14
#define ERROR_INVALID_NUMBER_TEXT "Invalid set member – not an integer\n"
#define MISSING_PARAMETER_SIGH 15
#define MISSING_PARAMETER_TEXT "Missing parameter\n"
#define EXTRANEOUS_TEXT_SIGH 16
#define EXTRANEOUS_TEXT_TEXT "Extraneous text after end of command\n"
#define CONSECUTIVE_COMMAS_SIGH 17
#define CONSECUTIVE_COMMAS_TEXT "Multiple consecutive commas\n"
#define MISSING_COMMAS_SIGH 18
#define MISSING_COMMAS_TEXT "Missing comma\n"
#define ILLEGAL_COMMA_SIGH 19
#define ILLEGAL_COMMA_TEXT "Illegal comma\n"
#define ERROR_EOF_SIGH 20
#define ERROR_EOF_TEXT "The program should not end with EOF\n"

/*Declare a set struct, holding 128 bits*/
typedef struct sets {
    int veryHigh;
    int High;
    int low;
    int veryLow;
} set;
typedef struct sets *pset;

/*Functions*/
int commandAnalysis (char* , int [] , pset* , pset* , pset* );
int isCommand (char*);
set* isSet (char*);
char* errorMassage (int sign);
