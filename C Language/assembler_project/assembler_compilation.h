/* Name: assembler_compilation.h
 * Author: Gil Or
 * Date: TBD
 * Description: Declare the functions associated with assembler_compilation.c file.
*/

#ifndef ASSEMBLER_ASSEMBLER_COMPILATION_H
#define ASSEMBLER_ASSEMBLER_COMPILATION_H

#include "data_structures.h"
#include "linked_list.h"
#include <stdio.h>

/*Function that process file after file in a loop*/
void process_files(int number_of_files, char *names_of_files[]);
/*Function that process and analyze a file*/
void process_a_file(char *name_of_file);
/*Call functions to check if there is a label, and analyze specific data/command line*/
int analyze_line(int line_number, char* line, ELEMENT_PTR code, ELEMENT_PTR data, ELEMENT_PTR symbols, int *IC, int *DC);
/*Identify the guidance type and call the correct function to analyze the command accordingly*/
int analyze_guidance(int number_of_line, char* name_of_command, char* label, char* line, LIST_HEAD data, LIST_HEAD symbols,int* DC);
/*Analyze guidance command in from data type*/
int analyze_data_guidance(char *name_of_command, int number_of_line, LIST_HEAD data, LIST_HEAD symbols, char* label, char* line, int* DC);
/*Analyze guidance command in the form of asciz type, also call function to update the symbol*/
int analyze_asciz_guidance(int number_of_line, LIST_HEAD data, LIST_HEAD symbols, char *label, char *line , int *DC);
/*Analyze guidance command in the form of entry type, also call function to update the symbol*/
int analyze_entry_guidance(int number_of_line, LIST_HEAD symbols, char *line);
/*Analyze guidance command in the form of external type, also call function to update the symbol*/
int analyze_extern_guidance(int number_of_line, LIST_HEAD symbols, char *line);
/*Search the name of the command in the commandInformation table and return the information if it the name was found*/
CommandInformation* get_information_of_command(char* name_of_command);
/*Analyze the instruction command and fill the correct values in the code record in the table*/
int analyze_instruction(int number_of_line, char *name_of_command, char *label, char* line, LIST_HEAD code, LIST_HEAD symbols, int *IC);

#endif
