/* Name: printMe
 * Author: Gil Or
 * Description: the program prints it's own text (source code).
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_MAX_LENGTH 200

int main(int argc, char **argv) {

    FILE *fptr;
    int ch, charsToCopy;
    char *fileNameRecived;
    char fileNameUsed[FILE_NAME_MAX_LENGTH] = {0};
    char *windownsEnd;

    setbuf(stdout, NULL);

    /* Save first argument in fileName - "./FILENAME.c*/
    fileNameRecived = argv[0];

    /*Ignore the "./" in the argument*/
    fileNameRecived = (fileNameRecived+2);
    charsToCopy = strlen(fileNameRecived);

    /*search windows executable ending, and remove it*/
    windownsEnd = strstr(fileNameRecived,".exe");
    if (windownsEnd != NULL) {
        charsToCopy -= strlen((".exe"));
    }
    strncpy(fileNameUsed,fileNameRecived,charsToCopy);

    /*add the .c ending*/
    fileNameUsed[charsToCopy]='.';
    charsToCopy++;
    fileNameUsed[charsToCopy]='c';


    /*open the source code file and fd will represent it*/
    fptr = fopen(fileNameUsed,"r");

    /*If the file could not be opened*/
    if (!(fptr)) {
        fprintf(stdout,"Something is wrong, the file can not be opened\n");
        exit (0);
    }

    while (!feof(fptr)) {
        ch = fgetc(fptr);
        putchar(ch);
    }

    fclose(fptr);

    return 0;
}

