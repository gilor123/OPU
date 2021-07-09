/* Name: linked_list.c
 * Author: Gil Or
 * Date: TBD
 * Description: Include the relevant functions and structs for the linked lists.
*/

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/*Create struct for a node in the list*/
typedef struct node
{
    /*Pointer to the data struct contains the data of this node*/
    ELEMENT_PTR node_data;
    /*Pointer to the next node*/
    struct node* next;
} node;

/*Create struct contains the information of the list*/
typedef struct list_information
{
    /*Pointer to the head of the list*/
    node* head;
    /*Function pointer to a function that add new node to the list*/
    ELEMENT_PTR(*add_node_data_func)();
    /*Function pointer to a function that frees a node in the list*/
    void(*free_node_data_func)(ELEMENT_PTR);
} list_information;

/*Return pointer of a list information data struct, with the relevant functions and a NULL head*/
LIST_HEAD createList(ELEMENT_PTR(*add_node_data_function)(), void(*free_node_data_function)(ELEMENT_PTR))
{
    struct list_information* list_info;
    list_info = (list_information*)malloc(sizeof(list_information));
    if (list_info == NULL)
        return NULL;
    list_info->add_node_data_func = add_node_data_function;
    list_info->free_node_data_func = free_node_data_function;
    list_info->head = NULL;
    return list_info;
}

/* Return pointer to the data of the head of the list. Update the input node to be the head node of the list*/
ELEMENT_PTR getHead(LIST_HEAD list_info, ELEMENT_PTR *node)
{
    list_information* temp_information = (list_information*)list_info;
    if ((temp_information == NULL) || (temp_information->head == NULL))
        return NULL;
    /*update the input node to points the head*/
    *node = temp_information->head;
    return temp_information->head->node_data;
}

/*Add node to the list. If fail, return null, otherwise return pointer to the node's data struct*/
ELEMENT_PTR addNodeToList(LIST_HEAD list_info)
{
    list_information* temp_information = (list_information*)list_info;
    ELEMENT_PTR node_information = temp_information->add_node_data_func();
    node* current_node = temp_information->head;
    node* new_node;
    node* previous_node = NULL;

    if (node_information == NULL)
        return NULL;
    new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL)
        return NULL;
    /*Fill the node's parameters*/
    new_node->next = NULL;
    new_node->node_data = node_information;
    /*If the list is empty*/
    if (temp_information->head == NULL)
    {
        temp_information->head = new_node;
        return node_information;
    }
    previous_node = current_node;
    while (current_node != NULL)
    {
        previous_node = current_node;
        current_node = current_node->next;
    }
    if (previous_node == NULL)
        return NULL;
    /*add in the end of list*/
    previous_node->next = new_node;
    return node_information;
}

/*Return pointer to the data of the next node in the list. Update the input node to be the next node in the list*/
ELEMENT_PTR getNext(ELEMENT_PTR *node)
{
    struct node* temp;
    if ((node == NULL) || (*node == NULL))
        return NULL;
    temp = *node;
    temp = temp->next;
    *node = temp;
    if (temp == NULL)
        return NULL;
    return temp->node_data;
}

/* Return '1' if it's not the last node in the list*/
int checkIfLastNode(ELEMENT_PTR *node)
{
    struct node* temp;
    if ((node == NULL) || (*node == NULL))
        return 0;
    temp = *node;
    return (temp->next != NULL);
}

/*Received the list information struct and free all the nodes and the node's data structs*/
void freeTheList(LIST_HEAD list_info)
{
    list_information* temp_information = (list_information*)list_info;
    node* current_node = temp_information->head;
    node* previous_node;
    while (current_node != NULL)
    {
        previous_node = current_node;
        current_node = current_node->next;
        temp_information->free_node_data_func(previous_node->node_data);
        free(previous_node);
    }
    free(list_info);
}

#endif