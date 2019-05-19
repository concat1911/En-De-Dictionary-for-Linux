#include <forms.h>
#include "list.h"

#define MAX_EN 20+1
#define MAX_DE 64+1
#define MAX_EX 128+1
#define DIM(x)  (sizeof(x)/sizeof(*(x)))

//the line to display on browser
char line[8 + MAX_EN + MAX_DE + MAX_EX + 3];

//DEFINITION STRUCT OF A WORD
struct word{
    char english[MAX_EN];
    char german [MAX_DE];
    char example[MAX_EX];
};

//PRINT THE HEADER MENU
void PrintHeaderMenu(FL_OBJECT *browser, FL_OBJECT *browser_message);

//UPDATE MESSAGE TO GIVE USER INFO                                    
void updateMessage  (FL_OBJECT *browser_message, char* message);

//SORTING WORDS BY RECENTLY ORDER                                            
void sortByRecently(struct Node* head_ref, FL_OBJECT *browser, FL_OBJECT *browser_message);

//SORTING WORDS BY ENGLISH ALPHABET ORDER               
void sortByEnglish (struct Node* head_ref, FL_OBJECT *browser, FL_OBJECT *browser_message, int count);

//SEARCHING THE GIVEN WORD
void searchWord    (struct Node* head_ref, FL_OBJECT *input_search, FL_OBJECT *browser, FL_OBJECT *browser_message);                                                                                        

//ADDING NEW WORD
struct word newWord(struct Node* head_ref, FL_OBJECT *input_english, FL_OBJECT *input_german, FL_OBJECT *input_example, FL_OBJECT *browser_message);

//REMOVE THE GIVEN WORD, return 1 if successfully, 0 if failed
int removeRecord   (struct Node**head, FL_OBJECT *input_remove, FL_OBJECT *browser, FL_OBJECT *browser_message);

//USING FOR QSORT
int  word_cmp(const void *a, const void *b);                                                                                                                                               