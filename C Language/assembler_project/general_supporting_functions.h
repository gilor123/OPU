/* Name: supporting_functions.c
 * Author: Gil Or
 * Date: TBD
 * Description: Declare the functions within supporting_functions.c file.
*/

#ifndef ASSEMBLER_SUPPORTING_FUNCTIONS_H
#define ASSEMBLER_SUPPORTING_FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "enums_constants.h"
#include "linked_list.h"
#include "data_structures.h"

/*If there is a symbol the function will return 1, and the label input would contain the label, otherwise return 0*/
int check_label(char* line, char* label);
/*The function removes spaces and tabs from the beginning and the end of the received line*/
void remove_spaces_and_tabs (char *original_line, char *processed_line);
/*The function prints the error according to the predefined requirements*/
void error_printing(int line_number, char* error_message, char* argument);
/*Search if the input symbol (label) is already exists in the symbols table, if not return NULL*/
symbol_record* search_symbol_in_table(char* name_of_symbol, LIST_HEAD symbol_records);
/*Fill the parameters in the data record, check for optional errors*/
int fill_values_in_record(data_record *data_rec, char* line, int number_of_line);
/*Check if the data number is the the range of the data type*/
int check_if_number_in_range (data_record *data_rec, int number);
/*Check if symbol with the same name is already exists in the symbol's table, if so, return it, if not
 * create a symbol record, if fails - return NULL*/
symbol_record *symbol_record_processing(int number_of_line, char *line, LIST_HEAD symbols);
/*The function reads the symbol's name from the line and copies it to the name_of_symbol.
 * Than checks if the name id valid*/
int find_name_of_symbol(int number_of_line, char *line, char *name_of_symbol);
/*Checks if the name of the symbol is valid, if it does return 1 otherwise return 0*/
int validate_label_name(int number_of_line ,char* name_of_symbol);
/*Check if the received word is one of the predefined words of the assembler*/
int is_saved_word (char *name_of_label);
/*Fill the proper information in the code record which is received as an input*/
int fill_information_in_record(int number_of_line, char* line, code_record *code_rec);
/*Identify the operands type and insert it to the record*/
int insert_operand_type(OperandStruct *operand, int number_of_line);
/*Check if the operand is an immediate, return 1 if it does and update the operand type,otherwise return 0*/
int fill_immediate_operand_type(OperandStruct *operand);
/*Check if the operand is a register, return 1 if it does and update the operand type,otherwise return 0*/
int is_register_operand_type(OperandStruct *operand, int number_of_line);
/*Check if the operand is a label, return 1 if it does and update the operand type,otherwise return 0*/
int is_label_operand_type(OperandStruct *operand, int number_of_line);
/*Add the value of IC to each of the symbols values that has data attribute*/
void update_symbols_values(symbol_record *symbols,int IC);
/*Add the value of IC to each of the data records*/
void update_data_values(data_record *data,int IC);
/*Validate the command, search errors in the formats, number of operands and types of operands.
 * Also check if the labels operand are exists in the symbol table*/
int validate_command(code_record *code_rec, LIST_HEAD symbols);
/*Identifies and update the operand numerical value*/
int covert_label_to_numerical_value (code_record *code_rec_of_operand,  OperandStruct *operand, symbol_record *symbol_rec);



#endif
