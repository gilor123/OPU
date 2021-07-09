/* Name: main.c
 * Author: Gil Or
 * Date: TBD
 * Description: the main program, gets the input files and call the function that compile them.
*/

#include "assembler_compilation.h"

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    if(argc == 1)
    {
        printf("No file's name was received. Please enter the name of the file(s)\n");
        return 0;
    }
    process_files(argc, argv);
    return 0;
}
