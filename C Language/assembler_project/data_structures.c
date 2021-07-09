/* Name: data_structures.c
 * Author: Gil Or
 * Date: TBD
 * Description: Include functions to create and free the data structures of the project.
*/

#include "data_structures.h"
#include <malloc.h>

/*Create a code record and return the pointer to this code record*/
ELEMENT_PTR create_code_record()
{
    code_record *temp_code_record = (code_record*)malloc(sizeof(code_record));
    temp_code_record->address = 0;
    temp_code_record->label[0] = '\0';
    temp_code_record->address = 0;
    temp_code_record->line_number = 0;
    temp_code_record->number_of_operands = 0;
    return temp_code_record;
}

/*Free a code record*/
void free_code_record(ELEMENT_PTR code_rec)
{
    free((code_record*)code_rec);
}

/*Create a symbol record and return the pointer to this symbol record*/
ELEMENT_PTR create_symbol_record()
{
    symbol_record *temp_symbol_record = (symbol_record*)malloc(sizeof(symbol_record));
    temp_symbol_record->attributes = 0;
    temp_symbol_record->symbol_value = 0;
    temp_symbol_record->symbol_name[0] = '\0';
    return temp_symbol_record;
}

/*Free a symbol record*/
void free_symbol_record(ELEMENT_PTR symbol_rec)
{
    free((symbol_record*)symbol_rec);
}

/*Create a data record and return the pointer to this data record*/
ELEMENT_PTR create_data_record()
{
    data_record *temp_data_record = (data_record*)malloc(sizeof(data_record));
    temp_data_record->data_type = 0;
    temp_data_record->string_value[0] = '\0';
    temp_data_record->number_of_values = 0;
    temp_data_record->address = 0;
    return temp_data_record;
}

/*Free a data record*/
void free_data_record(ELEMENT_PTR data_rec)
{
    free((data_record*)data_rec);
}