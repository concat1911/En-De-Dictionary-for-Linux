#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <forms.h>

#include "access.h"

//FOR QSORT
int word_cmp(const void *a, const void *b)
{
    struct word *wa = (struct word *)a;
    struct word *wb = (struct word *)b;
    return strcmp(wa->english, wb->english);
}

void PrintHeaderMenu(FL_OBJECT *browser, FL_OBJECT *browser_message){
    fl_clear_browser(browser);
    fl_add_browser_line(browser, "[#]     [ENGLISH]            [GERMAN]                                                         [EXAMPLE]");
    fl_add_browser_line(browser, "");
    updateMessage(browser_message, "Bevor Sie das Programm enden wollen, bitte das \"Speichern & Verlassen\" Button anklicken!");
}

//Up date the message text
void updateMessage(FL_OBJECT *browser_message, char* message){
    fl_clear_browser(browser_message);
    fl_add_browser_line(browser_message, message);
}

// //Display the word by the last added
void sortByRecently(struct Node* head_ref, FL_OBJECT *browser, FL_OBJECT *browser_message){
    PrintHeaderMenu(browser, browser_message);
    if( head_ref == NULL ) return;

    struct Node* h = head_ref;

    int i = 1;
    while(h->next != NULL){
        struct word *w;
        w = h->data;

        sprintf(line, "%-7d %-20s %-64s %-128s", i, w->english, w->german, w->example);

        fl_add_browser_line(browser, line);

        i++;
        h=h->next;
    }
}

//Sort the word list by alphabet
void sortByEnglish(struct Node* head_ref, FL_OBJECT *browser, FL_OBJECT *browser_message, int count){
    if(count<=1 || head_ref == NULL) return;

    struct Node* h = head_ref;
    //Array of person ( for sorting without effecting to the data list )
    struct word wArray[count];

    //Coppy data to the array
    for(int i=0; i < count; i++){
        struct word *w;
        w = h->data;

        strcpy(wArray[i].english,   w->english);
        strcpy(wArray[i].german,    w->german);
        strcpy(wArray[i].example,   w->example);

        h=h->next;
    }

    //Sort using qsort
    qsort(wArray, DIM(wArray), sizeof(struct word), word_cmp);

    PrintHeaderMenu(browser, browser_message);
    updateMessage(browser_message,  "Anzeigen nach Alphabet.");

    //Printing
    for(int j=1; j <= count; j++){
        sprintf(line, "% -7d %-20s %-64s %-128s", j, wArray[j-1].english, wArray[j-1].german, wArray[j-1].example);
        fl_insert_browser_line(browser, j+2, line);
    }
}

struct word newWord(struct Node* head_ref, FL_OBJECT *input_english, FL_OBJECT *input_german, FL_OBJECT *input_example, FL_OBJECT *browser_message){

    struct word w;

    //Check textfield is empty before continue
    if(strcmp(fl_get_input(input_english), "\0") == 0 || strcmp(fl_get_input(input_english), "###") == 0 || strcmp(fl_get_input(input_german), "\0") == 0){
        updateMessage(browser_message, "\"###\" ist nicht erlaubt oder Textfeld ist leer\n");
        strcpy(w.english, "###");
        return w;
    }

    struct Node* h = head_ref;
    int found=0;

    //Check the word is already exist or not
    while(h != NULL){
        struct word *w1 = h->data;
        if(strcmp(fl_get_input(input_english), w1->english) == 0){
            found=1;
            break;
        }
        h=h->next;
    }

    if(!found){
        strcpy(w.english, fl_get_input(input_english));
    }else{
        updateMessage(browser_message, "Diese Wort ist bereit existiert.\n");
        strcpy(w.english, "###");
        return w;
    }

    strcpy(w.german,  fl_get_input(input_german));
    strcpy(w.example, fl_get_input(input_example));

    return w;
}

void searchWord    (struct Node* head_ref, FL_OBJECT *input_search, FL_OBJECT *browser, FL_OBJECT *browser_message){   
    //Check if input Field empty
    if(strcmp(fl_get_input(input_search), "\0") == 0){
        updateMessage(browser_message, "Textfeld is leer. Bitte etwas eingeben.\n");
        return;
    } 

    struct Node* h = head_ref;
    
    int found=0;

    //SEARCH IN ENGLISH
    while(h != NULL){
        struct word *w = h->data;
        if(strcmp(fl_get_input(input_search), w->english) == 0){
            found = 1;
            PrintHeaderMenu(browser, browser_message);
            sprintf(line, "% -7d %-20s %-64s %-128s", 1, w->english, w->german, w->example);

            fl_add_browser_line(browser, line);

            break;
        }
        h=h->next;
    }

    //SEARCH IN GERMAN
    h = head_ref;

    while(h != NULL){
        struct word *w = h->data;

        if(strstr(w->german, fl_get_input(input_search)) != 0){
            found++;
            //PRINT HEADER ONLY ONCE, WHEN ENGLISH NOT FOUND 
            if(found <= 1)  PrintHeaderMenu(browser, browser_message);

            sprintf(line, "% -7d %-20s %-64s %-128s", found, w->english, w->german, w->example);

            fl_add_browser_line(browser, line);
        }

        h=h->next;
    }

    if(found == 0){
        sprintf(line, "Nicht gefunden: %s", fl_get_input(input_search));
        updateMessage(browser_message ,line);
    }

    //Clear text Field
    fl_set_input(input_search, "");
}

int removeRecord   (struct Node** head_ref, FL_OBJECT *input_remove, FL_OBJECT *browser, FL_OBJECT *browser_message){   
    //Check input Field before continue
    if(strcmp(fl_get_input(input_remove), "\0") == 0 || head_ref == NULL){
        updateMessage(browser_message, "Textfeld is leer. Bitte etwas eingeben.\n");
        return 0;
    } 

    struct Node* delNode = *head_ref;

    int found = 0;

    //SEARCH IN ENGLISH WORD LIST
    while(delNode != NULL){
        struct word *w = delNode->data;

        if(strcmp(w->english, fl_get_input(input_remove)) == 0){
            found = 1;
            break;
        }

        delNode = delNode->next;
    }

    //NOT FOUND 
    if(found==0){                           
        sprintf(line, "Nicht gefunden: %s", fl_get_input(input_remove));
        updateMessage(browser_message, line);
        return 0;
    }

    //WHEN FOUND, Remove from the list
    
    //Get the last and the previous Node
    struct Node* prev = delNode->prev;
    struct Node* next = delNode->next;

    //When the user want to delete the head node
    if(prev == NULL){
        free((*delNode).data);
        (*next).prev = NULL;
        *head_ref = next;
        return 1;
    }

    //When the user want to delelte the last node
    if(next == NULL && prev != NULL){
        free((*delNode).data);
        (*prev).next = NULL;
        return 1;
    }

    //Only one node left
    if(prev == NULL && next==NULL){
        free((*head_ref)->data);
        *head_ref = NULL;
        return 1;
    }

    //normal case
    (*prev).next = delNode->next;
    (*next).prev = prev;

    //Clear text Field
    fl_set_input(input_remove, "");
    
    return 1;
}   
