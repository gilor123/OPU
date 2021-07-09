/* Name: functions_to_support_output_files.c
 * Author: Gil Or
 * Date: TBD
 * Description: Including various functions to support the creation of the output files.
*/

#include "functions_to_support_output_files.h"

/*Write the object output file, call functions to write each of the code and data records*/
void create_object_file(FILE *object_file_ptr, LIST_HEAD code, LIST_HEAD data, int IC, int DC)
{
    ELEMENT_PTR code_record_node, data_record_node;
    code_record *temp_code_record = getHead(code, &code_record_node);
    data_record *temp_data_record = getHead(data, &data_record_node);
    unsigned int bits_array = 0;
    /*Used to count the byte, to identify when 4 bytes where printed to the file*/
    int address = IC;
    /*Write the first line in the file (the summary line)*/
    fprintf(object_file_ptr,"     %d %d\n", (IC-100), DC);
    if (temp_code_record != NULL)
    {
        /*Write the first code record to the output file*/
        write_code_record_to_object_file(object_file_ptr, temp_code_record);
    }
    /*Write the rest of the code records to the output file*/
    while (checkIfLastNode(&code_record_node))
    {
        temp_code_record = getNext(&code_record_node);
        write_code_record_to_object_file(object_file_ptr, temp_code_record);
    }
    if (temp_data_record != NULL)
    {
        /*Write the first data record to the output file*/
        write_data_record_to_object_file(object_file_ptr, temp_data_record,&address, &bits_array);
    }
    /*Write the rest of the data records to the output file*/
    while (checkIfLastNode(&data_record_node))
    {
        temp_data_record = getNext(&data_record_node);
        write_data_record_to_object_file(object_file_ptr, temp_data_record,&address, &bits_array);
    }
    /*Write the last line if needed*/
    write_last_line_of_data(object_file_ptr,&address, &bits_array);
}

/*Identifies the command type and call the proper function to arrange the bits to printing, then print the line*/
void write_code_record_to_object_file(FILE* object_file_ptr, code_record *code_rec)
{
    int address = (code_rec->address);
    /*The bits array will contain the bits of the code record to print to the file*/
    unsigned int bits_array = 0;
    unsigned int *bits_array_ptr = &bits_array;
    /*Organized the bits array correctly from the code record*/
    bits_array |= code_rec->command_information->opcode << 26;
    switch (code_rec->command_information->command_type) {
        case R_ARITHMETIC:
            arrange_r_arithmetic_command(code_rec, bits_array_ptr);
            break;
        case R_COPY:
            arrange_r_copy_command(code_rec, bits_array_ptr);
            break;
        case I_ARITHMETIC:
            arrange_i_arithmetic_command(code_rec, bits_array_ptr);
            break;
        case I_BRANCH:
            arrange_i_branch_command(code_rec, bits_array_ptr);
            break;
        case I_MEMORY:
            arrange_i_memory_command(code_rec, bits_array_ptr);
            break;
        case J:
            arrange_j_command(code_rec, bits_array_ptr);
            break;
    }
    write_command_line(object_file_ptr, address, bits_array);
}

/*Arrange the bits of the r arithmetic type command in the array bits*/
void arrange_r_arithmetic_command (code_record *code_rec, unsigned int *bits_array_ptr)
{
    /*Update the bits of rs register*/
    (*bits_array_ptr) |= code_rec->operand_array[0].operand_value << 21;
    /*Update the bits of rt register*/
    (*bits_array_ptr) |= code_rec->operand_array[1].operand_value << 16;
    /*Update the bits of rd register*/
    (*bits_array_ptr) |= code_rec->operand_array[2].operand_value << 11;
    /*Update the bits of the funct*/
    (*bits_array_ptr) |= code_rec->command_information->funct << 6;
}

/*Arrange the bits of the r copy type command in the array bits*/
void arrange_r_copy_command (code_record *code_rec, unsigned int *bits_array_ptr)
{
    /*Update the bits of rs register*/
    (*bits_array_ptr) |= code_rec->operand_array[0].operand_value << 21;
    /*Update the bits of rd register*/
    (*bits_array_ptr) |= code_rec->operand_array[1].operand_value << 11;
    /*Update the bits of the funct*/
    (*bits_array_ptr) |= code_rec->command_information->funct << 6;
}

/*Arrange the bits of the i arithmetic type command in the array bits*/
void arrange_i_arithmetic_command (code_record *code_rec, unsigned int *bits_array_ptr)
{
    /*Update the bits of rs register*/
    (*bits_array_ptr) |= code_rec->operand_array[0].operand_value << 21;
    /*Update the bits of rt register*/
    (*bits_array_ptr) |= code_rec->operand_array[2].operand_value << 16;
    /*Mask only the relevant bits, so if the immediate num is negative it will effect only the relevant bits*/
    code_rec->operand_array[1].operand_value &= (0xFFFF);
    /*Update the bits of the immediate*/
    (*bits_array_ptr) |= code_rec->operand_array[1].operand_value;
}

/*Arrange the bits of the i branch type command in the array bits*/
void arrange_i_branch_command (code_record *code_rec, unsigned int *bits_array_ptr)
{
    /*Update the bits of rs register*/
    (*bits_array_ptr) |= code_rec->operand_array[0].operand_value << 21;
    /*Update the bits of rt register*/
    (*bits_array_ptr) |= code_rec->operand_array[1].operand_value << 16;
    /*Mask only the relevant bits, so if the immediate num is negative it will effect only the relevant bits*/
    code_rec->operand_array[2].operand_value &= (0xFFFF);
    /*Update the bits of the immediate*/
    (*bits_array_ptr) |= code_rec->operand_array[2].operand_value;
}

/*Arrange the bits of the i memory type command in the array bits*/
void arrange_i_memory_command (code_record *code_rec, unsigned int *bits_array_ptr)
{
    /*Update the bits of rs register*/
    (*bits_array_ptr) |= code_rec->operand_array[0].operand_value << 21;
    /*Update the bits of rt register*/
    (*bits_array_ptr) |= code_rec->operand_array[2].operand_value << 16;
    /*Mask only the relevant bits, so if the immediate num is negative it will effect only the relevant bits*/
    code_rec->operand_array[1].operand_value &= (0xFFFF);
    /*Update the bits of the immediate*/
    (*bits_array_ptr) |= code_rec->operand_array[1].operand_value;
}

/*Arrange the bits of the j type command in the array bits*/
void arrange_j_command (code_record *code_rec, unsigned int *bits_array_ptr)
{
    /*If stop command, no additional adjustment is required*/
    if (strcmp(code_rec->command_information->command_name,"stop") == 0)
    {
        return;
    }
    /*If a jmp command with label operand*/
    if (code_rec->operand_array[0].operand_type == LABEL)
    {
        /*Update the bits of label*/
        (*bits_array_ptr) |= code_rec->operand_array[0].operand_value;
    }
    /*If the jump operand type is register*/
    else
    {
        /*Turn on the reg*/
        (*bits_array_ptr) |= 1 << 25;
        /*Update the bits of reg*/
        (*bits_array_ptr) |= code_rec->operand_array[0].operand_value;
    }
}

/*Write a line to the object file*/
void write_command_line(FILE *object_file, int address, unsigned int bits_array)
{
    unsigned int forth_num = ((bits_array & 0xff000000) >> 24);
    unsigned int third_num = ((bits_array & 0x00ff0000) >> 16);
    unsigned int second_num = ((bits_array & 0x0000ff00) >> 8);
    unsigned int first_num = (bits_array & 0x000000ff);
    /*Prints a line in the object file*/
    fprintf(object_file, "%04d %02X %02X %02X %02X\n", address, first_num, second_num, third_num, forth_num);
}

/*Write a data record to the object file*/
void write_data_record_to_object_file(FILE* object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr)
{
    switch (data_rec->data_type)
    {
        case DB_DATATYPE:
            write_db_data(object_file_ptr, data_rec, address, bits_arr);
            break;
        case DH_DATATYPE:
            write_dh_data(object_file_ptr, data_rec, address, bits_arr);
            break;
        case DW_DATATYPE:
            write_dw_data(object_file_ptr, data_rec, address, bits_arr);
            break;
        case STRING_DATATYPE:
            write_asciz_data(object_file_ptr, data_rec, address, bits_arr);
            break;
    }
}

/*Write db data command, also update the input address and bit arr*/
void write_db_data(FILE *object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr)
{
    int addres = (*address), i;
    unsigned bits_array = (*bits_arr);

    for (i=0; i < data_rec->number_of_values; i++)
    {
        /*Mask only the relevant bits, so if the value is negative it will effect only the relevant bits*/
        data_rec->data_array[i] &= (0xFF);
        switch (addres % 4)
        {
            case 0:
                bits_array |= (data_rec->data_array[i]);
                addres++;
                break;
            case 1:
                bits_array |= (data_rec->data_array[i] << 8);
                addres++;
                break;
            case 2:
                bits_array |= (data_rec->data_array[i] << 16);
                addres++;
                break;
            case 3:
                bits_array |= (data_rec->data_array[i] << 24);
                addres++;
                write_command_line(object_file_ptr,(addres-4),bits_array);
                bits_array = 0;
                break;
        }
    }
    *address = addres;
    *bits_arr = bits_array;
}

/*Write dh data command, also update the input address and bit arr*/
void write_dh_data(FILE *object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr)
{
    int addres = (*address), i;
    unsigned bits_array = (*bits_arr);

    for (i=0; i < data_rec->number_of_values; i++)
    {
        /*Mask only the relevant bits, so if the value is negative it will effect only the relevant bits*/
        data_rec->data_array[i] &= (0xFFFF);
        switch (addres % 4)
        {
            case 0:
                bits_array |= (data_rec->data_array[i]);
                addres += 2;
                break;
            case 1:
                bits_array |= (data_rec->data_array[i] << 8);
                addres += 2;
                break;
            case 2:
                bits_array |= (data_rec->data_array[i] << 16);
                addres += 2;
                write_command_line(object_file_ptr,(addres-4),bits_array);
                bits_array = 0;
                break;
            case 3:
                /*Copy to the bits array MSB - the LSB byte of the half word*/
                bits_array |= (data_rec->data_array[i] << 24);
                addres += 2;
                write_command_line(object_file_ptr,(addres-5),bits_array);
                bits_array = 0;
                /*Copy to the bits array LSB - the MSB byte of the half word*/
                bits_array |= data_rec->data_array[i] >> 8;
                break;
        }
    }
    *address = addres;
    *bits_arr = bits_array;
}

/*Write dw data command, also update the input address and bit arr*/
void write_dw_data(FILE *object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr)
{
    int addres = (*address), i;
    unsigned bits_array = (*bits_arr);

    for (i=0; i < data_rec->number_of_values; i++)
    {
        /*Mask only the relevant bits, so if the value is negative it will effect only the relevant bits*/
        data_rec->data_array[i] &= (0xFFFFFFFF);
        switch (addres % 4)
        {
            case 0:
                bits_array |= (data_rec->data_array[i]);
                addres += 4;
                write_command_line(object_file_ptr,(addres-4),bits_array);
                bits_array = 0;
                break;
            case 1:
                bits_array |= (data_rec->data_array[i] << 8);
                addres += 4;
                write_command_line(object_file_ptr,(addres-5),bits_array);
                bits_array = 0;
                /*Copy to the bits array LSB - the MSB byte of the word*/
                bits_array |= data_rec->data_array[i] >> 24;
                break;
            case 2:
                bits_array |= (data_rec->data_array[i] << 16);
                addres += 4;
                write_command_line(object_file_ptr,(addres-6),bits_array);
                bits_array = 0;
                /*Copy to the bits array two LSB - the two MSB bytes of the word*/
                bits_array |= data_rec->data_array[i] >> 16;
                break;
            case 3:
                bits_array |= (data_rec->data_array[i] << 24);
                addres += 4;
                write_command_line(object_file_ptr,(addres-7),bits_array);
                bits_array = 0;
                /*Copy to the bits array LSB - the MSB byte of the half word*/
                bits_array |= data_rec->data_array[i] >> 8;
                break;
        }
    }
    *address = addres;
    *bits_arr = bits_array;
}

/*Write asciz data command, also update the input address and bit arr*/
void write_asciz_data(FILE *object_file_ptr, data_record *data_rec, int *address, unsigned int *bits_arr)
{
    int addres = (*address), i;
    unsigned bits_array = (*bits_arr);
    int length = (int)strlen(data_rec->string_value);

    for (i=0; i <= length; i++)
    {
        switch (addres % 4)
        {
            case 0:
                bits_array |= (data_rec->string_value[i]);
                addres++;
                break;
            case 1:
                bits_array |= (data_rec->string_value[i] << 8);
                addres++;
                break;
            case 2:
                bits_array |= (data_rec->string_value[i] << 16);
                addres++;
                break;
            case 3:
                bits_array |= (data_rec->string_value[i] << 24);
                addres++;
                write_command_line(object_file_ptr,(addres-4),bits_array);
                bits_array = 0;
                break;
        }
    }
    *address = addres;
    *bits_arr = bits_array;
}

/*Identify how many byte remaining to write, and write the last line of the data*/
void write_last_line_of_data(FILE *object_file_ptr, int *address, unsigned int *bits_arr)
{
    unsigned bits_array = (*bits_arr);
    unsigned int third_num = ((bits_array & 0x00ff0000) >> 16);
    unsigned int second_num = ((bits_array & 0x0000ff00) >> 8);
    unsigned int first_num = (bits_array & 0x000000ff);
    int addres = (*address);

    switch (addres % 4)
    {
        case 0:
            break;
        case 1:
            /*Prints only the single remaining byte*/
            fprintf(object_file_ptr, "%04d %02X\n", (addres-1), first_num);
            break;
        case 2:
            /*Prints the two remaining bytes*/
            fprintf(object_file_ptr, "%04d %02X %02X\n", (addres-2), first_num, second_num);
            break;
        case 3:
            /*Prints the two remaining bytes*/
            fprintf(object_file_ptr, "%04d %02X %02X %02X\n", (addres-3), first_num, second_num, third_num);
            break;
    }
}

/*Check if there are external symbols, if yes create the external file otherwise delete it*/
void create_external_file(FILE *file_ext, LIST_HEAD code, LIST_HEAD symbols, char *file_name_ext)
{
    int propriety_flag = 0;
    ELEMENT_PTR temp_node;
    symbol_record *temp_symbol_record = getHead(symbols, &temp_node);
    /*If there are no symbols, the external file should be deleted*/
    if (temp_symbol_record == NULL)
    {
        if (remove(file_name_ext) == 0)
        {
            printf("There are no symbols, hence the external file was deleted");

        }
        else
        {
            printf("There was an error while deleting the external file");

        }
        return;
    }
    /*Call function to take the symbol record and check if there is a J type command use this external symbol*/
    propriety_flag += check_if_has_symbol_to_write(file_ext, temp_symbol_record, code);
    /*Call that prints the rest of the external symbols*/
    while (checkIfLastNode(&temp_node))
    {
        temp_symbol_record = getNext(&temp_node);
        propriety_flag += check_if_has_symbol_to_write(file_ext, temp_symbol_record, code);
        /*If this is the last symbol to check and there was no external symbols to print remove the file*/
        if (temp_symbol_record == NULL)
        {
            if (propriety_flag == 0)
            {
                if (remove(file_name_ext) == 0)
                {
                    printf("There are no external symbols, hence the external file was deleted");

                }
                else
                {
                    printf("There was an error while deleting the external file");
                }
                return;
            }
        }
    }
}

/*Check record after record in the code table to check if there are J type command that use this external symbol*/
int check_if_has_symbol_to_write(FILE *ext_file, symbol_record *symbol_rec, LIST_HEAD code)
{
    int propriety_flag = 0;
    ELEMENT_PTR temp_node;
    code_record *temp_code_record = getHead(code, &temp_node);
    /*Do nothing if the symbol rec has no external attribute*/
    if ((symbol_rec->attributes & EXTERNAL_ATTRIBUTETYPE) != EXTERNAL_ATTRIBUTETYPE)
        return propriety_flag;
    /*Check for in the first code record if there is a J command type that use the external symbol*/
    if (temp_code_record != NULL)
    {
        propriety_flag += write_a_symbol_to_external_file(ext_file, symbol_rec, temp_code_record);
    }
    /*Check the rest of the code records*/
    while (checkIfLastNode(&temp_node))
    {
        temp_code_record = getNext(&temp_node);
        propriety_flag += write_a_symbol_to_external_file(ext_file, symbol_rec, temp_code_record);
    }
    return propriety_flag;
}

/*Check if there is a J command type that use this external symbol, and if so prints a line in the external file*/
int write_a_symbol_to_external_file(FILE *ext_file, symbol_record *symbol_rec, code_record *code_rec)
{
    int i;
    /*Check if the code rec has a J type command*/
    if (code_rec->command_information->command_type != J)
    {
        return 0;
    }
    /*Check if there are J types commands which are using the external symbol*/
    for (i = 0; i < code_rec->number_of_operands; i++)
    {
        if (strcmp(code_rec->operand_array[i].operand_string,symbol_rec->symbol_name) == 0)
        {
            fprintf(ext_file, "%s %04d\n", symbol_rec->symbol_name, code_rec->address);
            return 1;
        }
    }
    return 0;
}

/*Check if there are entry symbols, if yes create the entry file otherwise delete it*/
void create_entry_file(FILE *file_ent, LIST_HEAD symbols, char *file_name_ent)
{
    int propriety_flag=0;
    ELEMENT_PTR temp_node;
    symbol_record *temp_symbol_record = getHead(symbols, &temp_node);
    /*If there are no symbols, the entry file should be deleted*/
    if (temp_symbol_record == NULL)
    {
        if (remove(file_name_ent) == 0)
        {
            printf("There are no symbols, hence the external file was deleted");

        }
        else
        {
            printf("There was an error while deleting the external file");
        }
        return;
    }
    /*Check if there are symbols with with entry attribute, if finds symbol, prints a line in the entry file*/
    if ((temp_symbol_record->attributes & ENTRY_ATTRIBUTETYPE) == ENTRY_ATTRIBUTETYPE)
    {
        propriety_flag = 1;
        fprintf(file_ent, "%s %04d\n", temp_symbol_record->symbol_name, temp_symbol_record->symbol_value);
    }
    while (checkIfLastNode(&temp_node))
    {
        temp_symbol_record = getNext(&temp_node);
        if ((temp_symbol_record->attributes & ENTRY_ATTRIBUTETYPE) == ENTRY_ATTRIBUTETYPE)
        {
            propriety_flag = 1;
            fprintf(file_ent, "%s %04d\n", temp_symbol_record->symbol_name, temp_symbol_record->symbol_value);
        }
    }
    /*If there was no entry symbol, delete the entry file*/
    if (propriety_flag == 0){
        if (remove(file_name_ent) == 0)
            printf("There are no entry symbols, hence the entry file was deleted");
        else
            printf("There was an error while deleting the entry file");
    }
}
