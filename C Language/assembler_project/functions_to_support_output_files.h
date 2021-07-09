/* Name: functions_to_support_output_files.h
 * Author: Gil Or
 * Date: TBD
 * Description: Declare the functions within functions_to_support_output_files.c file.
*/

#ifndef ASSEMBLER_FUNCTIONS_TO_SUPPORT_OUTPUT_FILES_H
#define ASSEMBLER_FUNCTIONS_TO_SUPPORT_OUTPUT_FILES_H

#include "general_supporting_functions.h"

/*Write the object output file*/
void create_object_file(FILE *object_file_ptr, LIST_HEAD code, LIST_HEAD data, int ID, int DC);
/*Identifies the command type and call the proper function to arrange the bits to printing, then print the line*/
void write_code_record_to_object_file(FILE* object_file_ptr, code_record *code_rec);
/*Arrange the bits of the r arithmetic type command in the array bits*/
void arrange_r_arithmetic_command (code_record *code_rec, unsigned int *bits_array_ptr);
/*Arrange the bits of the r copy type command in the array bits*/
void arrange_r_copy_command (code_record *code_rec, unsigned int *bits_array_ptr);
/*Arrange the bits of the i arithmetic type command in the array bits*/
void arrange_i_arithmetic_command (code_record *code_rec, unsigned int *bits_array_ptr);
/*Arrange the bits of the i branch type command in the array bits*/
void arrange_i_branch_command (code_record *code_rec, unsigned int *bits_array_ptr);
/*Arrange the bits of the i memory type command in the array bits*/
void arrange_i_memory_command (code_record *code_rec, unsigned int *bits_array_ptr);
/*Arrange the bits of the j type command in the array bits*/
void arrange_j_command (code_record *code_rec, unsigned int *bits_array_ptr);
/*Write a line to the object file*/
void write_command_line(FILE* object_file, int address, unsigned int bits_array);
/*Write a data record to the object file*/
void write_data_record_to_object_file(FILE* object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr);
/*Write db data command, also update the input address and bit arr*/
void write_db_data(FILE *object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr);
/*Write dh data command, also update the input address and bit arr*/
void write_dh_data(FILE *object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr);
/*Write dw data command, also update the input address and bit arr*/
void write_dw_data(FILE *object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr);
/*Write asciz data command, also update the input address and bit arr*/
void write_asciz_data(FILE *object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr);
/*Identify how many byte remaining to write, and write the last line of the data*/
void write_last_line_of_data(FILE *object_file_ptr, int *address, unsigned int *bits_arr);
/*Check if there are external symbols, if yes create the external file otherwise delete it*/
void create_external_file(FILE *file_ext, LIST_HEAD code, LIST_HEAD symbols, char *file_name_ext);
/*Check record after record in the code table to check if there are J type command that use this external symbol*/
int check_if_has_symbol_to_write(FILE *ext_file, symbol_record *symbol_rec, LIST_HEAD code);
/*Check if there is a J command type that use this external symbol, and if sp prints a line in the external file*/
int write_a_symbol_to_external_file(FILE *ext_file, symbol_record *symbol_rec, code_record *code_rec);
/*Check if there are entry symbols, if yes create the entry file otherwise delete it*/
void create_entry_file(FILE *file_ent, LIST_HEAD symbols, char *file_name_ent);





#endif
