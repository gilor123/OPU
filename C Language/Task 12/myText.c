/*Libraries*/

#include <stdio.h>
#include <stdlib.h>

#include "data.h"

/*Declarations*/

typedef struct node *pItem;
typedef struct node {
    char buff[ROW_LENGTH];
    pItem next;
} item;

int readText (int,void**);
void printText (int,void*);

/*Functions*/

int main() {

    int dataStructure;
    int notification;   /*notify if there is memory allocation problem*/
    char *buffer;
    pItem headOfList = NULL;

    /*Select data structure*/
    setbuf(stdout, 0);
    printf("\nPlease choose the data structure. Enter 1 For linked list, and 2 for buffer\n");

    /* Identify the required data structure*/
    dataStructure = getchar();

    /*First allocation of memory*/
    if (dataStructure == BUFFER){
        buffer = (char*) calloc(ROW_LENGTH,sizeof(char));
    }
    else {
        headOfList = (item*) calloc(1,sizeof(item));
    }

    printf("\nPlease enter your text\n");

    /*Call readText and printText with the selected data structure*/
    if (dataStructure == BUFFER){
        notification = readText(dataStructure,(void**)&buffer);
        printText(dataStructure,(void*)buffer);
    }

    else {
        notification = readText(dataStructure,(void**)&headOfList);
        printText(dataStructure,(void*)headOfList);
    }

    if (notification == MEMORY_ERROR)
        printf("\nThe memory allocation was failed, space was insufficient\n");

    return 0;
}

int readText (int dataStructure, void **pointer) {

    int i =0;   /*chars counter within each of the buffers*/
    int c;

    if (dataStructure == LINKED_LIST) {

        pItem *pHeadOfList = (pItem*)pointer;
        pItem headOfList = *pHeadOfList;
        headOfList ->next = NULL;

        while((c=getchar()) != EOF) {
            if (c != '\n') {
                /*The end of the array, need to increase memory allocation*/
                if(i == ROW_LENGTH) {
                    /*Allocating memory*/
                    (headOfList -> next) = (pItem)calloc(1,sizeof(item));
                    if(!(headOfList->next)) {
                        headOfList->buff[i] = '\0';
                        return MEMORY_ERROR;
                    }
                    /*update headOfList to point the next item*/
                    headOfList = headOfList->next;
                    i = 0;
                }
                headOfList->buff[i] = (char)c;
                i += 1;
            }
        }
    }

    else {
        char *tempBuffer = NULL;
        int bufAmount = 1;  /*reflects the amount of buffers (60 chars) already allocated*/

        while((c=getchar()) != EOF) {
            if (c != '\n') {
                /*Out of memory, need to increase memory allocation*/
                if(i == ROW_LENGTH) {
                    /*reallocating the required amount of memory*/
                    bufAmount++;
                    tempBuffer = (char*)realloc(*pointer,bufAmount*ROW_LENGTH*sizeof(char));
                    /*if memory allocation was failed*/
                    if (!tempBuffer){
                        ((char*)(*pointer))[((bufAmount-1)*ROW_LENGTH + i)] = '\0';
                        return MEMORY_ERROR;
                    }
                    /*if memory allocation succeed, update the pointer*/
                    *pointer = tempBuffer;
                    i = 0;
                }
                /*calculate the correct address to insert the char*/
                ((char*)(*pointer))[((bufAmount-1)*ROW_LENGTH + i)] = (char)c;
                i += 1;
            }
        }
        ((char*)(*pointer))[((bufAmount-1)*ROW_LENGTH + i)] = '\0';
    }
    return 0;
}


void printText (int structureType, void *pointer) {
    int i;

    if (structureType == BUFFER) {
        for (i = 0; ((char*)pointer)[i] != '\0' ; ++i) {
            if (i%ROW_LENGTH == 0 && i!=0)
                putchar('\n'); /*meet the 60 chars in a row condition*/
            putchar(((char*)pointer)[i]);
        }
        putchar('\n');
    }
    else {
        pItem headOfList = (pItem)pointer;

        for (i = 0; headOfList->buff[i] ; ++i) {
            putchar(headOfList->buff[i]);
            if (i == ROW_LENGTH-1) {
                putchar('\n');
                i=0;
                headOfList = headOfList->next;
            }
        }
        putchar('\n');
    }
}





