/* Name: supporting_functions.c
 * Author: Gil Or
 * Date: TBD
 * Description: Including various functions to support the compilation and analysis processes.
*/

#include "general_supporting_functions.h"

/*If there is a symbol the function will return 1, and the label input would contain the label, otherwise return 0*/
int check_label(char* line, char* label)
{
    int i;
    for (i = 0; i < strlen(line); i++)
    {
        if (line[i] == ':')
        {
            label[i] = '\0';
            return 1;
        }
        label[i] = line[i];
    }
    label[0] = '\0';
    return 0;
}

/*The function removes spaces and tabs from the beginning and the end of the received line*/
void remove_spaces_and_tabs (char *original_line, char *processed_line)
{
    int i = 0, j = 0;
    /*Remove from the beginning of the line*/
    while (((original_line[i] == ' ') || (original_line[i] == '\t')) && (original_line[i] != '\0'))
    {
        i++;
    }
    /*Copy the rest of the line until '\n' or '\0'*/
    while ((original_line[i] != '\n') && (original_line[i] != '\0'))
    {
        processed_line[j] = original_line[i];
        j++;
        i++;
    }
    i--;
    j--;
    /*Revert to the last char which was not space or tab*/
    while ((original_line[i] == ' ') || (original_line[i] == '\t'))
    {
        i--;
        j--;
    }
    j++;
    /*Define the end of the line*/
    processed_line[j] = '\0';
}

/*The function prints the error according to the predefined requirements*/
void error_printing(int line_number, char* error_message, char* argument)
{
    printf("An Error was found in line %d: %s %s \n",line_number, error_message, argument);
}

/*Search if the input symbol (label) is already exists in the symbols table, if not return NULL*/
symbol_record* search_symbol_in_table(char* name_of_symbol, LIST_HEAD symbol_records)
{
    ELEMENT_PTR node_in_list;
    symbol_record *temp_symbol_record = getHead(symbol_records, &node_in_list);
    if (temp_symbol_record == NULL)
        return NULL;
    if (strcmp(name_of_symbol, temp_symbol_record->symbol_name) == 0)
        return temp_symbol_record;
    while (checkIfLastNode(&node_in_list))
    {
        temp_symbol_record = getNext(&node_in_list);
        if (strcmp(name_of_symbol, temp_symbol_record->symbol_name) == 0)
            return temp_symbol_record;
    }
    return NULL;
}

/*Fill the parameters in the data record, check for optional errors*/
int fill_values_in_record(data_record *data_rec, char* line, int number_of_line)
{
    char number_string[MAXIMUM_LENGTH_LINE];
    int i, j, number;
    i=0;

    /*Check if there is ',' before the first parameter*/
    while (line[i] && ((line[i] == ' ') || (line[i] == '\t')))
    {
        i++;
    }
    if(line[i] == ',')
    {
        error_printing(number_of_line,"Illegal comma before first parameter","");
        return 0;
    }
    /*Loop to fill the numbers*/
    while (line[i])
    {
        /*Ignore spaces and tabs before the number*/
        while (line[i] && ((line[i] == ' ') || (line[i] == '\t')))
        {
            i++;
        }
        j = 0;
        /*Store the string representing the number*/
        while (line[i] && (line[i] != ',') && !((line[i] == ' ') || (line[i] == '\t')))
        {
            number_string[j] = line[i];
            i++;
            j++;
        }
        /*Insert manually the end of the string*/
        number_string[j] = '\0';
        /*Ignore spaces and tabs after the number*/
        while (line[i] && ((line[i] == ' ') || (line[i] == '\t')))
        {
            i++;
        }
        /*Check the number represented by the string*/
        if ((strlen(number_string) == 1) && number_string[0] == '0')
        {
            number=0;
        }
        else {
            number=atoi(number_string);
            if(number == 0)
            {
                error_printing(number_of_line,"Unrecognized integer: ",number_string);
                return 0;
            }
            if(!(check_if_number_in_range(data_rec,number)))
            {
                error_printing(number_of_line,"An integer is not in the range of the data type","");
                return 0;
            }
        }
        /*Store the number in the record*/
        data_rec->data_array[data_rec->number_of_values] = number;
        data_rec->number_of_values++;
        /*Check if the last number, and the end of the line*/
        if ((line[i] == '\n') || line[i] == '\0')
        {
            break;
        }
        /*Should be another parameter, so ',' should be the next char*/
        if (line[i] != ',')
        {
            error_printing(number_of_line,"Missing comma between the parameters","");
            return 0;
        }
        i++;
    }
    return 1;
}

/*Check if the data number is the the range of the data type*/
int check_if_number_in_range (data_record *data_rec, int number)
{
    if (((data_rec->data_type) & DB_DATATYPE) == DB_DATATYPE)
    {
        if ( number>(BYTE_MIN_VAL) && number<BYTE_MAX_VAL)
        {
            return 1;
        }
    }
    if (((data_rec->data_type) & DH_DATATYPE) == DH_DATATYPE)
    {
        if ( number>(HALF_MIN_VAL) && number<HALF_MAX_VAL)
        {
            return 1;
        }
    }
    if (((data_rec->data_type) & DW_DATATYPE) == DW_DATATYPE)
    {
        if ( number>(WORD_MIN_VAL) && number<WORD_MAX_VAL)
        {
            return 1;
        }
    }
        return 0;
}

/*Check if symbol with the same name is already exists in the symbol's table, if so, return it, if not
 * create a symbol record, if fails - return NULL*/
symbol_record *symbol_record_processing(int number_of_line, char *line, LIST_HEAD symbols)
{
    char name_of_symbol[MAXIMUM_LENGTH_SYMBOL];
    symbol_record *temp_symbol;
    if (!find_name_of_symbol(number_of_line, line, name_of_symbol))
    {
        return NULL;
    }
    temp_symbol = search_symbol_in_table(name_of_symbol, symbols);
    /*If temp_symbol is NULL, the label does not exists in the symbols tables*/
    if (temp_symbol == NULL)
    {
        temp_symbol = addNodeToList(symbols);
        if (temp_symbol == NULL)
        {
            error_printing(number_of_line, "Memory allocation was failed","");
            return NULL;
        }
        strcpy(temp_symbol->symbol_name, name_of_symbol);
    }
    return temp_symbol;
}


/*The function reads the symbol's name from the line and copies it to the name_of_symbol.
 * Than checks if the name id valid*/
int find_name_of_symbol(int number_of_line, char *line, char *name_of_symbol)
{
    int i = 0, j = 0;
    /*Ignore spaces and tabs before the name of the symbol*/
    while ((line[i] != '\0') && (line[i] == ' ' || line[i] == '\t'))
    {
        i++;
    }

    while ((line[i] != '\0') && !(line[i] == ' ' || line[i] == '\t'))
    {
        name_of_symbol[j] = line[i];
        i++;
        j++;
    }
    name_of_symbol[j] = '\0';
    while (line[i] != '\0')
    {
        /*Searching for redundant chars*/
        if (!(line[i] == ' ' || line[i] == '\t'))
        {
            error_printing(number_of_line, "Redundant chars after the symbol name","");
            return 0;
        }
        i++;
    }
    return validate_label_name(number_of_line,name_of_symbol);
}


/*Checks if the name of the label is valid, if it does return 1 otherwise return 0*/
int validate_label_name(int number_of_line ,char* name_of_label)
{
    int length_of_symbol = (int)strlen(name_of_label), i;
    if (length_of_symbol == 0)
        return 0;
    if (!isalpha(name_of_label[0]))
        return 0;
    for (i = 1; i < length_of_symbol; i++)
    {
        if (!isalpha(name_of_label[i]) && !isdigit(name_of_label[i]))
        {
            error_printing(number_of_line,"The syntax of the following symbol name is not valid:",name_of_label);
            return 0;
        }
    }
    if (!is_saved_word(name_of_label))
    {
        error_printing(number_of_line,"Predefined words of the assembler can not be used as labels:",name_of_label);
        return 0;
    }
    return 1;
}

/*Check if the received word is one of the predefined words of the assembler*/
int is_saved_word (char *name_of_label)
{
    if (strcmp(name_of_label,"add") == 0 ||
        strcmp(name_of_label,"sub") == 0 ||
        strcmp(name_of_label,"and") == 0 ||
        strcmp(name_of_label,"or") == 0 ||
        strcmp(name_of_label,"nor") == 0 ||
        strcmp(name_of_label,"move") == 0 ||
        strcmp(name_of_label,"mvhi") == 0 ||
        strcmp(name_of_label,"mvlo") == 0 ||
        strcmp(name_of_label,"subi") == 0 ||
        strcmp(name_of_label,"andi") == 0 ||
        strcmp(name_of_label,"ori") == 0 ||
        strcmp(name_of_label,"nori") == 0 ||
        strcmp(name_of_label,"bne") == 0 ||
        strcmp(name_of_label,"beq") == 0 ||
        strcmp(name_of_label,"blt") == 0 ||
        strcmp(name_of_label,"bgt") == 0 ||
        strcmp(name_of_label,"lb") == 0 ||
        strcmp(name_of_label,"sb") == 0 ||
        strcmp(name_of_label,"lw") == 0 ||
        strcmp(name_of_label,"sw") == 0 ||
        strcmp(name_of_label,"lh") == 0 ||
        strcmp(name_of_label,"sh") == 0 ||
        strcmp(name_of_label,"jmp") == 0 ||
        strcmp(name_of_label,"la") == 0 ||
        strcmp(name_of_label,"call") == 0 ||
        strcmp(name_of_label,"stop") == 0 ||
        strcmp(name_of_label,"asciz") == 0 ||
        strcmp(name_of_label,"db") == 0 ||
        strcmp(name_of_label,"dh") == 0 ||
        strcmp(name_of_label,"dw") == 0 ||
        strcmp(name_of_label,"entry") == 0 ||
        strcmp(name_of_label,"extern") == 0 )
    {
        return 0;
    }
    return 1;
}

/*Fill the proper information in the code record which is received as an input*/
int fill_information_in_record(int number_of_line, char* line, code_record *code_rec)
{
    int i = 0, j, operand_counter = 0;
    while (line[i])
    {
        /*Ignore spaces and tabs before the comma*/
        while ((line[i] != '\0') && (line[i] == ' ' || line[i] == '\t'))
        {
            i++;
        }
        /*Identify the end of the line*/
        if (line[i] == '\0')
            break;
        if (line[i] == ',')
        {
            if (operand_counter == 0)
            {
                error_printing(number_of_line, "Illegal double comma before the first operand","");
                return 0;
            }
            else
            {
                i++;
            }
            /*Ignore spaces and tabs after the comma*/
            while ((line[i] != '\0') && (line[i] == ' ' || line[i] == '\t'))
            {
                i++;
            }
        }
        j= 0;
        /*Copy the operand string to the operand array*/
        while ((line[i] != '\0') && !(line[i] == ' ' || line[i] == '\t') && (line[i] != ','))
        {
            code_rec->operand_array[operand_counter].operand_string[j] = line[i];
            j++;
            i++;
        }
        code_rec->operand_array[operand_counter].operand_string[j] = '\0';
        if (strlen(code_rec->operand_array[operand_counter].operand_string) == 0)
        {
            error_printing(number_of_line, "Illegal empty operand","");
            return 0;
        }
        if((insert_operand_type(&code_rec->operand_array[operand_counter],number_of_line)) == 0)
        {
            return 0;
        }
        operand_counter++;
    }
    /*Update the number of operands in the record*/
    if (operand_counter > 0)
    {
        code_rec->number_of_operands = operand_counter;
    }
    return 1;
}

/*Identify the operands type and insert it to the record*/
int insert_operand_type(OperandStruct *operand, int number_of_line)
{
    if (fill_immediate_operand_type(operand))
    {
        return 1;
    }
    if (is_register_operand_type(operand, number_of_line))
    {
        return 1;
    }
    if (is_label_operand_type(operand, number_of_line))
    {
        return 1;
    }
    return 0;
}

/*Check if the operand is an immediate, return 1 if it does and update the operand type and value, otherwise return 0*/
int fill_immediate_operand_type(OperandStruct *operand)
{
    int number;
    /*First char of the immediate must be either '-' or a digit*/
    if (!isdigit(operand->operand_string[0]) && (operand->operand_string[0] != '-'))
    {
        return 0;
    }
    /*Check if the string represented a '0' number*/
    if ((operand->operand_string[0] == 0) && (operand->operand_string[1] == '\0'))
    {
        operand->operand_type = IMMEDIATE;
        operand->operand_value = 0;
        return 1;
    }
    number = atoi(operand->operand_string);
    if (number != 0)
    {
        operand->operand_type = IMMEDIATE;
        operand->operand_value = number;
        return 1;
    }
    return 0;
}

/*Check if the operand is a register, return 1 if it does and update the operand type and value,otherwise return 0*/
int is_register_operand_type(OperandStruct *operand, int number_of_line)
{
    int number, i;
    int length = (int)strlen(operand->operand_string);
    char *number_ptr = operand->operand_string+1;
    /*Check for length error - should be $X, or $XX format*/
    if ((length != 2) && (length != 3))
    {
        return 0;
    }
    /*Search the $ before the number of the register*/
    if (operand->operand_string[0] != '$')
        return 0;
    /*Check if the string represented a '0' number*/
    if ((operand->operand_string[1] == '0') && (operand->operand_string[2] == '\0'))
    {
        operand->operand_string[0] = 0;
        operand->operand_string[1] = '\0';
        operand->operand_type = REGISTER;
        operand->operand_value = 0;
        return 1;
    }
    /*The +1 is to start from the char after the $*/
    number = atoi(number_ptr);
    /*Check for invalid integer*/
    if (number == 0)
    {
        return 0;
    }
    /*Check the number of the register is in valid range*/
    if (number < 0 || number > 31)
    {
        error_printing((number_of_line),"The register number is not in range","");
        return 0;
    }
    /*Remove the "$" sign from the string representing the number of register*/
    for (i=0; i < (length-1); i++)
    {
        operand->operand_string[i] = operand->operand_string[i+1];
    }
    operand->operand_string[i] = '\0';
    operand->operand_type = REGISTER;
    operand->operand_value = number;
    return 1;
}

/*Check if the operand is a label, return 1 if it does and update the operand type,otherwise return 0*/
int is_label_operand_type(OperandStruct *operand, int number_of_line)
{
    if (!validate_label_name(number_of_line,operand->operand_string))
    {
        return 0;
    }
    operand->operand_type = LABEL;
    return 1;
}

/*Add the value of IC to each of the symbols values that has data attribute*/
void update_symbols_values(symbol_record *symbols,int IC)
{
    ELEMENT_PTR node_in_list;
    symbol_record *temp_symbol_record = getHead(symbols, &node_in_list);
    if (temp_symbol_record == NULL)
        return;
    /*If the symbol has data attribute, add the IC value to the symbol value*/
    if ((temp_symbol_record->attributes & DATA_ATTRIBUTETYPE) == DATA_ATTRIBUTETYPE)
    {
        temp_symbol_record->symbol_value += IC;
    }
    while (checkIfLastNode(&node_in_list))
    {
        temp_symbol_record = getNext(&node_in_list);
        if ((temp_symbol_record->attributes & DATA_ATTRIBUTETYPE) == DATA_ATTRIBUTETYPE)
        {
            temp_symbol_record->symbol_value += IC;
        }
    }
}

/*Add the value of IC to each of the data records*/
void update_data_values(data_record *data,int IC)
{
    ELEMENT_PTR node_in_list;
    data_record *temp_data_record = getHead(data, &node_in_list);
    if (temp_data_record == NULL)
        return;
    /*Add the IC value to the data address value*/
    temp_data_record->address += IC;
    while (checkIfLastNode(&node_in_list))
    {
        temp_data_record = getNext(&node_in_list);
        temp_data_record->address += IC;
    }
}

/*Validate the command, search errors in the formats, number of operands and types of operands.
 * Also check if the labels operand are exists in the symbol table convert them to numbers*/
int validate_command(code_record *code_rec, LIST_HEAD symbols)
{
    int i;
    symbol_record *temp_symbol_record;
    /*Check for NULL command information error*/
    if (code_rec->command_information == NULL)
    {
        return 0;
    }
    /*Check for incorrect number of operands relatively to what should be for the specific command*/
    if (code_rec->number_of_operands != code_rec->command_information->number_of_operands)
    {
        error_printing((code_rec->line_number),"Incorrect number of operands","");
        return 0;
    }
    /*Loop to validate that each of the operands is the correct operand type*/
    for (i = 0; i < code_rec->number_of_operands; i++)
    {
        if (code_rec->operand_array[i].operand_type != code_rec->command_information->operands_types[i])
        {
            if (strcmp(code_rec->command_information->command_name,"jmp") != 0)
            {
                error_printing((code_rec->line_number),"Invalid operand type","");
                return 0;
            }
            /*Check if "jmp" command, as the operand could be either a register or a label*/
            else
            {
                if (code_rec->operand_array[i].operand_type != LABEL &&
                    code_rec->operand_array[i].operand_type != REGISTER)
                {
                    error_printing((code_rec->line_number),"Invalid operand type","");
                    return 0;
                }
            }
        }
        /*If the operand is a label, check if the label exists in the symbols table*/
        if (code_rec->operand_array[i].operand_type == LABEL)
        {
            temp_symbol_record = search_symbol_in_table(code_rec->operand_array[i].operand_string, symbols);
            if (temp_symbol_record == NULL)
            {
                error_printing(code_rec->line_number, "The following label is unrecognized:", code_rec->operand_array[i].operand_string);
                return 0;
            }
            /*The symbol is exists in the table, hence update the attribute*/
            temp_symbol_record->attributes |= CODE_ATTRIBUTETYPE;
            /*Analyze the label and save the numerical value it is representing in the operand struct*/
            covert_label_to_numerical_value (code_rec, &(code_rec->operand_array[i]), temp_symbol_record);
        }
    }
    return 1;
}

/*Identifies and update the operand numerical value*/
int covert_label_to_numerical_value (code_record *code_rec_of_operand,  OperandStruct *operand, symbol_record *symbol_rec)
{
    int difference;
    /*If J type command*/
    if (code_rec_of_operand->command_information->command_type == J)
    {
        /*If the symbol has external attribute type the values will be 0*/
        if ((symbol_rec->attributes & EXTERNAL_ATTRIBUTETYPE) == EXTERNAL_ATTRIBUTETYPE)
        {
            symbol_rec->symbol_value = 0;
            (*operand).operand_value = 0;
            return 1;
        }
        (*operand).operand_value = symbol_rec->symbol_value;
        return 1;
    }
    /*If BRANCH type command*/
    else
    {
        /*If the symbol has external attribute type there is an error*/
        if ((symbol_rec->attributes & EXTERNAL_ATTRIBUTETYPE) == EXTERNAL_ATTRIBUTETYPE)
        {
            error_printing(code_rec_of_operand->line_number, "Branch commands can not access external labels","");
            return 0;
        }
        /*Calculates the difference between the destination address and the source
         * and updates the operand numerical value*/
        difference = (symbol_rec->symbol_value - code_rec_of_operand->address);
        if (difference < HALF_MIN_VAL || difference >HALF_MAX_VAL)
        {
            error_printing(code_rec_of_operand->line_number, "Branch commands immediate value not in the valid range","");
            return 0;
        }
        (*operand).operand_value = difference;
        return 1;
    }
}
