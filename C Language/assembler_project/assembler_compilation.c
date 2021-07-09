/* Name: assembler_compilation.c
 * Author: Gil Or
 * Date: TBD
 * Description: Include functions to process and analyze the files, transform the received assembly language
 *              to computer's language, and create the output files as defined in the project description
*/

#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "data_structures.h"
#include "general_supporting_functions.h"
#include "assembler_compilation.h"
#include "functions_to_support_output_files.h"


/*Creates the structure that contains the predefined information associated with each of the commands*/
CommandInformation command_information_table[] =
        {
                {"add", R_ARITHMETIC, 0, 1, 3, {REGISTER, REGISTER, REGISTER}},
                {"sub", R_ARITHMETIC, 0, 2, 3, {REGISTER, REGISTER, REGISTER}},
                {"and", R_ARITHMETIC, 0, 3, 3, {REGISTER, REGISTER, REGISTER}},
                {"or", R_ARITHMETIC, 0, 4, 3, {REGISTER, REGISTER, REGISTER}},
                {"nor", R_ARITHMETIC, 0, 5, 3, {REGISTER, REGISTER, REGISTER}},
                {"move", R_COPY, 1, 1, 2, {REGISTER, REGISTER}},
                {"mvhi", R_COPY, 1, 2, 2, {REGISTER, REGISTER}},
                {"mvlo", R_COPY, 1, 3, 2, {REGISTER, REGISTER}},
                {"addi", I_ARITHMETIC, 10, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"subi", I_ARITHMETIC, 11, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"andi", I_ARITHMETIC, 12, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"ori", I_ARITHMETIC, 13, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"nori", I_ARITHMETIC, 14, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"bne", I_BRANCH, 15, 0, 3, {REGISTER, REGISTER, LABEL}},
                {"beq", I_BRANCH, 16, 0, 3, {REGISTER, REGISTER, LABEL}},
                {"blt", I_BRANCH, 17, 0, 3, {REGISTER, REGISTER, LABEL}},
                {"bgt", I_BRANCH, 18, 0, 3, {REGISTER, REGISTER, LABEL}},
                {"lb", I_MEMORY, 19, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"sb", I_MEMORY, 20, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"lw", I_MEMORY, 21, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"sw", I_MEMORY, 22, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"lh", I_MEMORY, 23, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"sh", I_MEMORY, 24, 0, 3, {REGISTER, IMMEDIATE, REGISTER}},
                {"jmp", J, 30, 0, 1, {REGISTER_OR_LABEL}},
                {"la", J, 31, 0, 1, {LABEL}},
                {"call", J, 32, 0, 1, {LABEL}},
                {"stop", J, 63, 0, 0},
        };

/*Function that calls internal function to process file after file in a loop*/
void process_files(int number_of_files, char *names_of_files[])
{
    int i;
    /*Initialized i to 1 as the [0] argument is always the name of the file*/
    for (i = 1; i < number_of_files; i++)
    {
        process_a_file(names_of_files[i]);
    }
}

/*Function that process and analyze a file*/
void process_a_file(char *name_of_file)
{
    /*Variable for the name of the files*/
    char file_name_as[MAXIMUM_LENGTH_FILE_NAME];
    char file_name_ob[MAXIMUM_LENGTH_FILE_NAME];
    char file_name_ext[MAXIMUM_LENGTH_FILE_NAME];
    char file_name_ent[MAXIMUM_LENGTH_FILE_NAME];
    /*Variable for the read line and the processed line version*/
    char received_line[MAXIMUM_LENGTH_LINE], processed_line[MAXIMUM_LENGTH_LINE];
    /*Pointers for each of the inputs / outputs files*/
    FILE* file_as, *file_ob, *file_ext, *file_ent;
    /*The linked lists (reflecting the tables) of the code, the data, and the symbols*/
    LIST_HEAD data = NULL, symbols = NULL, code = NULL;
    /*Variables for the number of the line in the file, the address of commands, the IC and the DC*/
    int line_number = 0, DC = 0, IC = 100, valid_line=0;
    /*A flag to identify whether the file has errors or not*/
    int propriety_flag = 1;
    /*Temp code record and node structs used for reading the assembly code lines*/
    code_record *temp_code_record;
    ELEMENT_PTR temp_node;

    /*Store the name of the assembler file and opens it*/
    sprintf(file_name_as, "%s.as", name_of_file);
    file_as = fopen(file_name_as, "r");
    /*Check if fails to open the file*/
    if (file_as == NULL)
    {
        printf("Assembler file %s not found\n", file_name_as);
        return;
    }
    printf("Start processing assembler file %s\n", file_name_as);
    /*Create the linked lists of the code, data, and symbols (reflecting the tables)*/
    code = createList(create_code_record, free_code_record);
    data = createList(create_data_record, free_data_record);
    symbols = createList(create_symbol_record, free_symbol_record);
    /*Loop to process each of the line from the assembler file*/
    while (fgets(received_line, sizeof(received_line), file_as) != NULL)
    {
        /*Add 1 to the line number's counter*/
        line_number++;
        /*Removing the spaces and tabs from the beginning and the end of the received line*/
        remove_spaces_and_tabs(received_line, processed_line);
        /*Check if the line is empty or a command*/
        if ((strlen(processed_line) == 0) || (processed_line[0] == ';'))
            continue;
        /*Call the function to analyze a line, each line which includes an error will turn the propriety_flag to 0
         * which results not creating the output files*/
         valid_line = analyze_line(line_number, processed_line, code, data, symbols, &IC, &DC);
        propriety_flag = propriety_flag && valid_line ;
    }
    fclose(file_as);
    /*Update the values of symbols and data (add IC to each of the records values*/
    update_symbols_values(symbols,IC);
    update_data_values(data,IC);
    /*Validate that the commands (code records) and their operands are legal*/
    temp_code_record = getHead(code, &temp_node);
    if (temp_code_record != NULL)
    {
        /*Validate the first command (code record), the function also add the proper numerical values of the labels*/
        propriety_flag = (propriety_flag && validate_command(temp_code_record, symbols));
        while (checkIfLastNode(&temp_node))
        /*Validate the rest of the commands (code records)*/
        {
            temp_code_record = getNext(&temp_node);
            valid_line = validate_command(temp_code_record, symbols);
            propriety_flag = (propriety_flag && valid_line);
        }
    }
    /*If propriety flag is equal to 1 (true) there was no errors and the output files should be created*/
    if (propriety_flag)
    {
        /*Create the object files*/
        sprintf(file_name_ob, "%s.ob", name_of_file);
        file_ob = fopen(file_name_ob, "w");
        /*If opening the files was succeed*/
        if (file_ob != NULL)
        {
            create_object_file(file_ob, code, data, IC, DC);
            fclose(file_ob);
        }
        else
        {
            printf("Fail to create the object file\n");
        }
        /*Create the external file*/
        sprintf(file_name_ext, "%s.ext", name_of_file);
        file_ext = fopen(file_name_ext, "w");
        if (file_ext != NULL)
        {
            create_external_file(file_ext, code, symbols, file_name_ext);
            fclose(file_ext);
        }
        else
        {
            printf("Fail to create the external file\n");
        }
        /*Create the entry file */
        sprintf(file_name_ent, "%s.ent", name_of_file);
        file_ent = fopen(file_name_ent, "w");
        if (file_ent != NULL)
        {
            create_entry_file(file_ent, symbols, file_name_ent);
            fclose(file_ent);
        }
        else
        {
            printf("Fail to create the external file\n");
        }
        printf("The process of assembler file named: %s is finished successfully\n", file_name_as);
    }
    /*If propriety flag is 0 the output files should not be created*/
    else
    {
        printf("The process of the assembler file is over, during the process errors was found\n");
    }
    /* free the lists */
    freeTheList(code);
    freeTheList(data);
    freeTheList(symbols);
}

/*Call functions to check if there is a label, and analyze specific data/command line*/
int analyze_line(int number_of_line, char* line, LIST_HEAD code, LIST_HEAD data, LIST_HEAD symbols, int *IC, int *DC)
{
    char label[MAXIMUM_LENGTH_SYMBOL];
    char name_of_command[MAXIMUM_LENGTH_COMMAND_NAME];
    int i;
    label[0] = '\0';
    /*If check_label return 1 that means there is a label*/
    if (check_label(line, label))
    {
        /*Update the line pointer to after the ':' char of the label*/
        line += strlen(label) + 1;
        validate_label_name(number_of_line,label);
    }
    /*Ignore the spaces and tabs*/
    while ((*line) && (*line == ' ' || *line == '\t'))
    {
        line++;
    }
    i = 0;
    /*Store the name of the command*/
    while ((*line) && (*line != ' ') && (*line != '\t'))
    {
        name_of_command[i] = *line;
        i++;
        line++;
    }
    /*Synthetic end for the name of the command*/
    name_of_command[i] = '\0';
    /*Check if the command is a guidance line of data*/
    if (name_of_command[0] == '.')
    {
        return analyze_guidance(number_of_line, name_of_command, label, line, data, symbols, DC);
    }
    return analyze_instruction(number_of_line, name_of_command, label, line, code, symbols, IC);
}

/*Identify the guidance type and call the correct function to analyze the command accordingly*/
int analyze_guidance(int number_of_line, char* name_of_comand, char* label, char* line, LIST_HEAD data, LIST_HEAD symbols,int* DC)
{
    /*Check the guidance type and calls the correct function to analyze*/
    if ((strcmp(name_of_comand, DB_GUIDANCE) == 0) ||
        (strcmp(name_of_comand, DH_GUIDANCE) == 0) ||
        (strcmp(name_of_comand, DW_GUIDANCE) == 0))
    {
        return analyze_data_guidance(name_of_comand, number_of_line, data, symbols, label, line, DC);
    }
    if (strcmp(name_of_comand, ASCIZ_GUIDANCE) == 0)
    {
        return analyze_asciz_guidance(number_of_line, data, symbols, label, line, DC);
    }
    if (strcmp(name_of_comand, ENTRY_GUIDANCE) == 0)
    {
        return analyze_entry_guidance(number_of_line, symbols, line);
    }
    if (strcmp(name_of_comand, EXTERN_GUIDANCE) == 0)
    {
        return analyze_extern_guidance(number_of_line, symbols, line);
    }
    error_printing(number_of_line, "The syntax of the following data command is not valid:", name_of_comand);
    return 0;
}


/*Analyze guidance command in the form of data type, also call function to update the symbol*/
int analyze_data_guidance(char *name_of_command, int number_of_line, LIST_HEAD data, LIST_HEAD symbols, char *label, char *line, int *DC)
{
    symbol_record * temp_symbol = NULL;
    data_record * temp_data_record = NULL;
    int factor;
    /*Processing the label if it's existing*/
    if (strlen(label) > 0)
    {
        temp_symbol = search_symbol_in_table(label, symbols);
        /*If temp_symbol is NULL the label does not exists in the symbols table, and it will be added*/
        if (temp_symbol == NULL)
        {
            temp_symbol = addNodeToList(symbols);
            if (temp_symbol == NULL)
            {
                error_printing(number_of_line, "Memory allocation was failed", "");
                return 0;
            }
            strcpy(temp_symbol->symbol_name, label);
            temp_symbol->symbol_value = *DC;
            temp_symbol->attributes |= DATA_ATTRIBUTETYPE;
        }
        /*If temp_record is not NULL the label already exists in the symbols table*/
        else
        {
            /*If the existing symbol has only entry attribute that's valid, otherwise not*/
            if ((temp_symbol->attributes | ENTRY_ATTRIBUTETYPE) == ENTRY_ATTRIBUTETYPE)
            {
                temp_symbol->symbol_value = *DC;
                temp_symbol->attributes |= DATA_ATTRIBUTETYPE;
            }
            else
            {
                error_printing(number_of_line, "Duplicated label was found: ", label);
                return 0;
            }
        }
    }
    /*Add the data record to the data table*/
    temp_data_record = addNodeToList(data);
    if (temp_data_record == NULL)
    {
        error_printing(number_of_line, "Memory allocation was failed","");
        return 0;
    }
    /*Add the data type*/
    if (strcmp(name_of_command, DB_GUIDANCE) == 0)
    {
        temp_data_record->data_type = DB_DATATYPE;
        factor=1;
    }
    if (strcmp(name_of_command, DH_GUIDANCE) == 0)
    {
        temp_data_record->data_type = DH_DATATYPE;
        factor=2;
    }
    if (strcmp(name_of_command, DW_GUIDANCE) == 0)
    {
        temp_data_record->data_type = DW_DATATYPE;
        factor=4;
    }
    if (!fill_values_in_record(temp_data_record, line, number_of_line))
    {
        return 0;
    }
    if (temp_data_record->number_of_values == 0)
    {
        error_printing(number_of_line, "There is no numbers after the data command", "");
        return 0;
    }
    temp_data_record->address = *DC;

    (*DC) += (factor*(temp_data_record->number_of_values));
    return 1;
}

/*Analyze guidance command in the form of asciz type, also call function to update the symbol*/
int analyze_asciz_guidance(int number_of_line, LIST_HEAD data, LIST_HEAD symbols, char *label, char *line, int *DC)
{
    int quotation_mark_beg, quotation_mark_end, i = 0, j = 0;
    symbol_record *temp_symbol = NULL;
    data_record *temp_data_record = NULL;
    /*Processing the label if it's existing*/
    if (strlen(label) > 0)
    {
        temp_symbol = search_symbol_in_table(label, symbols);
        /*If temp_symbol is NULL the label does not exists in the symbols table, and it will be added*/
        if (temp_symbol == NULL)
        {
            temp_symbol = addNodeToList(symbols);
            if (temp_symbol == NULL)
            {
                error_printing(number_of_line, "Memory allocation was failed", "");
                return 0;
            }
            strcpy(temp_symbol->symbol_name, label);
            temp_symbol->symbol_value = *DC;
            temp_symbol->attributes |= DATA_ATTRIBUTETYPE;
        }
        /*If temp_record is not NULL the label already exists in the symbols table*/
        else
        {
            /*If the existing symbol has only entry attribute that's valid, otherwise not*/
            if ((temp_symbol->attributes | ENTRY_ATTRIBUTETYPE) == ENTRY_ATTRIBUTETYPE)
            {
                temp_symbol->symbol_value = *DC;
                temp_symbol->attributes |= DATA_ATTRIBUTETYPE;
            }
            else
            {
                error_printing(number_of_line, "Duplicated label was found: ", label);
                return 0;
            }
        }
    }
    /*Ignore spaces and tabs before the string*/
    while ((line[i] != '\0') && (line[i] == '\t' || line[i] == ' '))
    {
        i++;
    }
    if (line[i] != '"')
    {
        error_printing(number_of_line, "Missing quotation mark before the string","");
        return 0;
    }
    quotation_mark_beg = i;
    i++;
    /*Jump to the end of the string*/
    while ((line[i] != '\0') && (line[i] != '"'))
        i++;
    if (line[i] != '"')
    {
        error_printing(number_of_line, "The asciz value should ends with a quotation mark","");
        return 0;
    }
    quotation_mark_end = i;
    i++;
    while ((line[i] != '\0') && (line[i] == '\t' || line[i] == ' '))
        i++;
    /*Check for redundant chars at the end of the line*/
    if (line[i] != '\0')
    {
        error_printing(number_of_line, "Redundant chars at the end of the line","");
        return 0;
    }
    /*Add a data record*/
    temp_data_record = addNodeToList(data);
    if (temp_data_record == NULL)
    {
        error_printing(number_of_line, "Memory allocation was failed","");
        return 0;
    }
    /*Make it point the char after the first quotation mark*/
    quotation_mark_beg++;
    /*Copies the string to the data record*/
    while (quotation_mark_beg < quotation_mark_end)
    {
        temp_data_record->string_value[j] = line[quotation_mark_beg];
        j++;
        quotation_mark_beg++;
    }
    /*Synthetic ending for the string and updating the data record*/
    temp_data_record->string_value[j] = '\0';
    temp_data_record->data_type = STRING_DATATYPE;
    temp_data_record->address = *DC;
    /*Promote the address*/
    (*DC) += strlen(temp_data_record->string_value) + 1;
    return 1;
}

/*Analyze guidance command in the form of entry type, also call function to update the symbol*/
int analyze_entry_guidance(int number_of_line, LIST_HEAD symbols, char *line)
{
    /*the symbol_record_processing function will either create the record
     * or return an existing one with the same symbol name*/
    symbol_record *temp_symbol = symbol_record_processing(number_of_line, line, symbols);
    if (temp_symbol == NULL)
        return 0;
    /*Check if the label within the table already has extern attribute, if so prints an error*/
    if ((temp_symbol->attributes & EXTERNAL_ATTRIBUTETYPE) == EXTERNAL_ATTRIBUTETYPE)
    {
        error_printing(number_of_line, "Label can not has both external and entry attributes","");
        return 0;
    }
    temp_symbol->attributes |= ENTRY_ATTRIBUTETYPE;
    return 1;
}

/*Analyze guidance command in the form of external type, also call function to update the symbol*/
int analyze_extern_guidance(int number_of_line, LIST_HEAD symbols, char *line)
{
    symbol_record *temp_symbol = symbol_record_processing(number_of_line, line, symbols);
    if (temp_symbol == NULL)
        return 0;
    /*Check if the label within the table already has entry attribute, if so prints an error*/
    if ((temp_symbol->attributes & ENTRY_ATTRIBUTETYPE) ==ENTRY_ATTRIBUTETYPE)
    {
        error_printing(number_of_line, "Label can not has both external and entry attributes","");
        return 0;
    }
    temp_symbol->attributes |= EXTERNAL_ATTRIBUTETYPE;
    /*Default value of 0*/
    temp_symbol->symbol_value = 0;
    return 1;
}

/*Analyze the instruction command and fill the correct values in the code record in the table*/
int analyze_instruction(int number_of_line, char *name_of_command, char *label, char* line, LIST_HEAD code, LIST_HEAD symbols,int *IC)
{
    CommandInformation *temp_command_information;
    code_record *temp_code_record = NULL;
    symbol_record * temp_symbol = NULL;
    temp_command_information = get_information_of_command(name_of_command);
    /*Processing the label if it's existing*/
    if (strlen(label) > 0)
    {
        temp_symbol = search_symbol_in_table(label, symbols);
        /*If temp_symbol is NULL the label does not exists in the symbols table, and it will be added*/
        if (temp_symbol == NULL)
        {
            temp_symbol = addNodeToList(symbols);
            if (temp_symbol == NULL)
            {
                error_printing(number_of_line, "Memory allocation was failed", "");
                return 0;
            }
            strcpy(temp_symbol->symbol_name, label);
            temp_symbol->symbol_value = *IC;
            temp_symbol->attributes |= CODE_ATTRIBUTETYPE;
        }
            /*If temp_record is not NULL the label already exists in the symbols table*/
        else
        {
            /*If the existing symbol has only entry attribute that's valid, otherwise not*/
            if ((temp_symbol->attributes | ENTRY_ATTRIBUTETYPE) == ENTRY_ATTRIBUTETYPE)
            {
                temp_symbol->symbol_value = *IC;
                temp_symbol->attributes |= CODE_ATTRIBUTETYPE;
            }
            else
            {
                error_printing(number_of_line, "Duplicated label was found: ", label);
                return 0;
            }
        }
    }

    if (temp_command_information == NULL)
    {
        error_printing(number_of_line,"The following name of command is unrecognized:", name_of_command);
        return 0;
    }
    temp_code_record = addNodeToList(code);
    if (temp_code_record == NULL)
    {
        error_printing(number_of_line, "Memory allocation was failed","");
        return 0;
    }
    temp_code_record->line_number = number_of_line;
    temp_code_record->command_information = temp_command_information;
    /*If there's a label, save it's name*/
    if (strlen(label) > 0)
    {
        strcpy(temp_code_record->label, label);
    }
    else
    {
        temp_code_record->label[0] = '\0';
    }
    /*If fill_information_in_record return 0 there was an error, otherwise it will fill the operands and return 1*/
    if (!fill_information_in_record(number_of_line, line, temp_code_record))
        return 0;
    temp_code_record->address = *IC;
    (*IC) += 4;
    return 1;
}

/*Search the name of the command in the commandInformation table and return the information if it the name was found*/
CommandInformation* get_information_of_command(char* name_of_command)
{
    int i;
    for (i = 0; i < MAXIMUM_NUMBER_OF_COMMAND; i++)
    {
        if (strcmp((const char *) command_information_table[i].command_name, name_of_command) == 0)
            return &command_information_table[i];
    }
    return NULL;
}
