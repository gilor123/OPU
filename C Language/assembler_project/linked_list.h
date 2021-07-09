/* Name: linked_list.h
 * Author: Gil Or
 * Date: TBD
 * Description: Define relevant constants for the linked lists, and declare the functions associated with the linked lists.
*/

#ifndef ASSEMBLER_LINKED_LIST_H
#define ASSEMBLER_LINKED_LIST_H

/*ELEMENT_PTR would be a pointer to a struct that holds the data of each node in the linked list.
 * Each node will have a ELEMENT_PTR variable parameter, and a pointer to the next node.
 * It's created to make the code more convenience to be read*/
typedef void* ELEMENT_PTR;
/*HEAD would reflect a node in a linked list. Created to make the code more convenience to be read*/
typedef void* LIST_HEAD;



/*Return pointer of a list information data struct, with the relevant functions and a NULL head
 * The inputs are pointers to functions that create and free node in this list type*/
LIST_HEAD createList(ELEMENT_PTR(*create_node)(), void(*free_node)(ELEMENT_PTR));

/* Return pointer to the data of the head of the list. Update the input node to be the head node of the list*/
ELEMENT_PTR getHead(LIST_HEAD list_info, ELEMENT_PTR *node);

/*Add node to the list. If fail, return null, otherwise return pointer to the node's data struct*/
ELEMENT_PTR addNodeToList(LIST_HEAD list_info);

/*Return pointer to the data of the next node in the list. Update the input node to be the next node in the list*/
ELEMENT_PTR getNext (ELEMENT_PTR* node);

/* Return '1' if it's not the last node in the list*/
int checkIfLastNode(ELEMENT_PTR* node);

/*Received the list information struct and free all the nodes and the node's data structs*/
void freeTheList(LIST_HEAD list_info);

#endif
