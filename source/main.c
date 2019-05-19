/*
BELEG C PROGRAMMING ( DICTIONARY EN-DE / DE-EN <libforms> )
STUDENT: Tran Nhat Linh     s79226      45538
*/

#include <stdlib.h>
#include "access.h"

//FUNCTIONS PROTOTYPE //CALL BACK
void removeWord     (FL_OBJECT *obj, long data);
void showRecently   (FL_OBJECT *obj, long data);
void showEnlishOrder(FL_OBJECT *obj, long data);
void addWord        (FL_OBJECT *obj, long data);
void search         (FL_OBJECT *obj, long data);
void exitProg       (FL_OBJECT *obj, long data);

//GUI VARIABLES
    //BROWSER
    FL_OBJECT *browser;
    FL_OBJECT *browser_message;
 
    //INPUT FIELD
    FL_OBJECT *input_search;
    FL_OBJECT *input_remove;
    FL_OBJECT *input_english;
    FL_OBJECT *input_german;
    FL_OBJECT *input_example;

//GLOBAL VARIABLES
//Head is a pointer to the first word in list
struct Node* head = NULL;
int    count =0;

//TEMP FILE TO WRITE DATA AFTER EACH SESSION
FILE *fOutput;

int main(int argc, char **argv){

    FL_FORM   *form;

    //BUTTONS
    FL_OBJECT *btn_add;
    FL_OBJECT *btn_remove;
    FL_OBJECT *btn_search;
    FL_OBJECT *btn_recently;
    FL_OBJECT *btn_sort;
    FL_OBJECT *btn_quit;

    fl_initialize(&argc, argv, "Dictionary", 0, 0);

    //Window
    form         = fl_bgn_form(FL_UP_BOX, 800, 600);

    //SETIINGS: position and attributes
    //SEARCH FEATURE
    input_search = fl_add_input (FL_NORMAL_INPUT,  40,  10, 150, 25, "");
    btn_search   = fl_add_button(FL_NORMAL_BUTTON, 190, 10, 50,  25, "Suchen");

    //REMOVE FEATURE
    input_remove = fl_add_input (FL_NORMAL_INPUT,  280, 10,  150, 25, "");
    btn_remove   = fl_add_button(FL_NORMAL_BUTTON, 430, 10,  50,  25, "Entf.");

    //SAVE AND QUIT
    btn_quit     = fl_add_button(FL_NORMAL_BUTTON, 690, 10, 100, 60,  "Speichern &\nVerlassen");

    //SHOW WORD BY RECENTLY ORDER AND BY ENGLISH WORD
    btn_recently   = fl_add_button(FL_NORMAL_BUTTON, 510, 10,  80,  25, "vor Kurzem");
    btn_sort     = fl_add_button(FL_NORMAL_BUTTON, 600, 10,  80,  25, "sortieren");

    //ADD NEW RECORD FEATURE
    input_english = fl_add_input (FL_NORMAL_INPUT, 40,  45,  150, 25, "En ");    
    input_german  = fl_add_input (FL_NORMAL_INPUT, 220, 45,  150, 25, "De ");
    input_example = fl_add_input (FL_NORMAL_INPUT, 430, 45,  170, 25, "Beispiel");
    btn_add       = fl_add_button(FL_NORMAL_BUTTON,600, 45,  80,  25, "addieren");

    //INPUT FIELD SETTING
    //MAXIMAL CHARACTERS 
    fl_set_input_maxchars(input_english, MAX_EN);
    fl_set_input_maxchars(input_german,  MAX_DE);
    fl_set_input_maxchars(input_example, MAX_EX);

    fl_set_input_maxchars(input_search,  MAX_EN);
    fl_set_input_maxchars(input_remove,  MAX_EN);

    fl_set_input_return(input_search,   FL_RETURN_NONE);
    fl_set_input_return(input_remove,   FL_RETURN_NONE);
    fl_set_input_return(input_english,  FL_RETURN_NONE);
    fl_set_input_return(input_german,   FL_RETURN_NONE);
    fl_set_input_return(input_example,  FL_RETURN_NONE);

    //Browser is the panel to show all the words in our list
    browser = fl_add_browser(FL_MULTI_BROWSER, 15, 100, 775, 480, "ALL WORDS");
    fl_set_object_return(browser, FL_RETURN_NONE);

    //Browser message is a text to show user what happend at the moment
    browser_message = fl_add_browser(FL_NORMAL_BROWSER, 15, 75,  775,  22, "");
    fl_set_browser_hscrollbar(browser_message, FL_OFF);

    //Add some color and style
    fl_set_object_color(btn_quit,         FL_PALEGREEN, 0 );
    fl_set_object_color(btn_recently,       FL_SLATEBLUE, 0 );
    fl_set_object_color(btn_sort,         FL_DODGERBLUE, 0 );
    fl_set_object_color(browser_message,  FL_TOMATO, 0 );

    //TOOLTIP to help user know all the functions
    fl_set_object_helper(btn_add,      "Add new words to the list");
    fl_set_object_helper(btn_remove,   "Remove word from list");
    fl_set_object_helper(btn_search,   "Search word from list");
    fl_set_object_helper(btn_recently, "Order by recently add");
    fl_set_object_helper(btn_sort,     "Order by english alphabeta");
    fl_set_object_helper(btn_quit,     "All changes must be saved before quit.");

    //CALLBACK FUNCTIONS SETUP
    fl_set_object_callback (btn_recently, showRecently, 0);
    fl_set_object_callback (btn_sort,   showEnlishOrder, 0);
    fl_set_object_callback (btn_add,    addWord, 0);
    fl_set_object_callback (btn_search, search, 0);
    fl_set_object_callback (btn_quit,   exitProg, 0);
    fl_set_object_callback (btn_remove, removeWord, 0);              

    fl_end_form();
 
    fl_show_form(form, FL_PLACE_MOUSE, FL_FULLBORDER, "English - Deutsch Dictionary");

    //FILE DATA VARIABLES
    FILE *fInput = fopen ("data.dat", "ab+"); 
    fOutput      = fopen ("tmp.dat",  "wb+");

    if (fInput == NULL || fOutput == NULL){
        fprintf(stderr, "Daten-File kann nicht angelegt werden!\n");
        exit (1);
    }

    //Load all Records and save in a List
    //data lesen bis EOF
    printf("Dateien einlesen ... Bitte warten Sie einen Augenblick!\n");
    
    struct word wToRead;

    while(fread(&wToRead, sizeof(struct word), 1, fInput)){
        append(&head, &wToRead, sizeof(struct word));
        count++;
    }

    //close
    fclose(fInput);

    //SHOW ALL THE WORDS WHEN THE PROGRAMM START
    sortByRecently(head, browser, browser_message);

    //PROGRAMM LOOP
    fl_do_forms();

    return 0;
}

void showRecently(FL_OBJECT *obj, long data){
    sortByRecently(head, browser, browser_message);
}

void showEnlishOrder(FL_OBJECT *obj, long data){
    sortByEnglish(head, browser, browser_message, count);
}

void addWord(FL_OBJECT *obj, long data){
    struct word w = newWord(head, input_english, input_german, input_example, browser_message);

    if(strcmp(w.english, "###") != 0){
        push(&head, &w, sizeof(struct word));
        count++;
        sortByRecently(head, browser, browser_message);

        fl_set_input(input_english, "");
        fl_set_input(input_german,  "");
        fl_set_input(input_example, ""); 
    }
}

void search(FL_OBJECT *obj, long data){
    searchWord(head, input_search, browser, browser_message);
}

void removeWord(FL_OBJECT *obj, long data){
    if(removeRecord(&head, input_remove, browser, browser_message)){
        count--;
        sortByRecently(head, browser, browser_message);
        fl_set_input(input_remove, "");
    }
}

void exitProg(FL_OBJECT *obj, long data){

    for(int i=0; i < count; i++){
        fwrite(head->data, sizeof(struct word), 1, fOutput);
        head=head->next;
    }

    fclose(fOutput);

    remove("data.dat");
    rename("tmp.dat", "data.dat");

    printf("Exit successfully\n");
    exit(0);
}
