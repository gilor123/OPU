/* Name: constants.h
 * Author: Gil Or
 * Date: TBD
 * Description: Define relevant constants and enum for the program.
*/

#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

/* The maximum length of a single line */
#define MAXIMUM_LENGTH_LINE 80
/* The maximum length of symbol's name */
#define MAXIMUM_LENGTH_SYMBOL 31
/* The maximum length of file's name */
#define MAXIMUM_LENGTH_FILE_NAME 200
/* The maximum length of an operand */
#define MAXIMUM_LENGTH_OPERAND 80
/* The maximum length of a command */
#define MAXIMUM_LENGTH_COMMAND_NAME 10
/* The maximum amount of operands in .data command */
#define MAXIMUM_OPERANDS_AMOUNT_IN_DATA 80
/* The maximum amount of operands in a command */
#define MAXIMUM_OPERANDS_AMOUNT 3
/*The maximum number of commands*/
#define MAXIMUM_NUMBER_OF_COMMAND 27
/*Byte min int value*/
#define BYTE_MIN_VAL (-128)
/*Byte max int value*/
#define BYTE_MAX_VAL 127
/*Half min int value*/
#define HALF_MIN_VAL (-32768)
/*Half max int value*/
#define HALF_MAX_VAL 32767
/*Word min int value*/
#define WORD_MIN_VAL (-2147483647)
/*Word max int value*/
#define WORD_MAX_VAL 2147483647



/*Define the names of all optional guidance commands*/
#define ASCIZ_GUIDANCE ".asciz"
#define DB_GUIDANCE ".db"
#define DH_GUIDANCE ".dh"
#define DW_GUIDANCE ".dw"
#define ENTRY_GUIDANCE ".entry"
#define EXTERN_GUIDANCE ".extern"

/*Enum of numbers representing the optional guidance commands*/
typedef enum DataType
{
    STRING_DATATYPE = 1,
    DB_DATATYPE = 2,
    DH_DATATYPE = 4,
    DW_DATATYPE = 8
} DataType;

/*Enum of the optional operand types*/
typedef enum OperandType
{
    REGISTER,
    IMMEDIATE,
    LABEL,
    REGISTER_OR_LABEL
} OperandType;

/*Enum of the optional commands types*/
typedef enum CommandType
{
    R_ARITHMETIC,
    R_COPY,
    I_ARITHMETIC,
    I_BRANCH,
    I_MEMORY,
    J
} CommandType;

/*Enum of all the optional attributes types for a symbol*/
typedef enum AttributeType
{
    CODE_ATTRIBUTETYPE = 1,
    DATA_ATTRIBUTETYPE = 2,
    ENTRY_ATTRIBUTETYPE = 4,
    EXTERNAL_ATTRIBUTETYPE = 8
} AttributeType;

#endif
