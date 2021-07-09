/* Name: data_structures.h
 * Author: Gil Or
 * Date: TBD
 * Description: Define relevant data structures for the different commands, operands, and records in the project
*/

#ifndef ASSEMBLER_DATA_STRUCTURES_H
#define ASSEMBLER_DATA_STRUCTURES_H

#include "enums_constants.h"
#include "linked_list.h"

/*Structure that contains the predefined information associated with each of the commands*/
typedef struct
{
    /*The name of the command*/
    char* command_name;
    /*The type of the command (R,I,J)*/
    CommandType command_type;
    /*The opcode of the command*/
    int opcode;
    /*The funct of the command*/
    int funct;
    /*The number of operands should be received*/
    int number_of_operands;
    /*An array save store the type of operands received in this command*/
    OperandType operands_types [MAXIMUM_OPERANDS_AMOUNT];

} CommandInformation;

/*Structure that holds the symbols (the symbols table)*/
typedef struct symbol_record {
    /*The name of the symbol (aka label)*/
    char symbol_name[MAXIMUM_LENGTH_SYMBOL];
    /*The value of the symbol*/
    int symbol_value;
    /*The bits array to store attributes of symbol*/
    int attributes;
} symbol_record;

/*Structure that contains the type of an operand in a command,  a string format of it's the value, and an int value*/
typedef struct OperandStruct {
    OperandType operand_type;
    char operand_string[MAXIMUM_LENGTH_OPERAND];
    int operand_value;
} OperandStruct;

/*Structure that hold the information of the commands that was read (the code table)*/
typedef struct code_record {
    /*The label*/
    char label[MAXIMUM_LENGTH_SYMBOL];
    /*The proper command's information associated with this command*/
    CommandInformation *command_information;
    /*The number of operands received in this command*/
    int number_of_operands;
    /*The address of the command*/
    int address;
    /*The number of line in the file in which this command was written in*/
    int line_number;
    /*The array contains the operands received in this command*/
    struct OperandStruct operand_array[MAXIMUM_OPERANDS_AMOUNT];
} code_record;

/*Structure that hold the information of the data that was read (the data table)*/
typedef struct data_record {
    /*The data type*/
    DataType data_type;
    /*The string value in case the data command is .asciz*/
    char string_value[MAXIMUM_LENGTH_LINE];
    /*The number of values received (for .db,.dh,.dw commands it could be more than 1)*/
    int number_of_values;
    /*The array of the values in case the data command is .db or .dh or .dw*/
    int data_array [MAXIMUM_OPERANDS_AMOUNT_IN_DATA];
    /*The address of the data*/
    int address;
} data_record;

/*Create a code record and return the pointer to this code record*/
ELEMENT_PTR create_code_record();

/*Free a code record*/
void free_code_record(ELEMENT_PTR code_rec);

/*Create a symbol record and return the pointer to this symbol record*/
ELEMENT_PTR create_symbol_record();

/*Free a symbol record*/
void free_symbol_record(ELEMENT_PTR symbol_rec);

/*Create a data record and return the pointer to this data record*/
ELEMENT_PTR create_data_record();

/*Free a data record*/
void free_data_record(ELEMENT_PTR data_rec);

#endif
