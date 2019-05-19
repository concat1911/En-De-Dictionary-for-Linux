#include <stdlib.h>
#include "list.h"

void push(struct Node** head_ref, void *new_data, size_t data_size) {   
    // Allocate memory for node 
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node)); 
  
    new_node->data  = malloc(data_size);
    // Copy contents of new_data to newly allocated memory. 
    // Assumption: char takes 1 byte. 
    int i; 
    for (i=0; i<data_size; i++) 
        *(char *)(new_node->data + i) = *(char *)(new_data + i); 

    new_node->next  = (*head_ref);
    new_node->prev  = NULL; 

    if((*head_ref) != NULL)
        (*head_ref)->prev = new_node;

  
    // Change head pointer as new node is added at the beginning 
    (*head_ref)    = new_node; 
}

void append(struct Node** head_ref, void *new_data, size_t data_size){
    struct Node* last = *head_ref; //clone head

    // Allocate memory for node 
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node)); 
    new_node->data  = malloc(data_size);

    //Assign Data
    for (int i = 0; i<data_size; i++){
        *(char *)(new_node->data + i) = *(char *)(new_data + i); 
    }
    
    //This is the last node, so next node should be NULL
    new_node->next = NULL;

    //If the Linked List is empty, then make the new node as head
    if (*head_ref == NULL) { 
        new_node->prev = NULL;
        *head_ref = new_node; 
        return; 
    }

    //Else traversse to the last node
    while (last->next != NULL) 
        last = last->next;

    last->next = new_node;
    new_node->prev = last;

    return;
}