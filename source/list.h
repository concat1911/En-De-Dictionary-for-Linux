/*GENERIC DOUBLE LINKED LIST
Tran Nhat Linh
email: nhatlinh.nh2511@gmail.com 
*/

struct Node {
    void*  data;
    struct Node *next;
    struct Node *prev;
};

//ADD NEW NODE AT FIST POSITION
void push       (struct Node** head_ref, void *new_data, size_t data_size);
//ADD NEW NODE AT LAST POSITION
void append     (struct Node** head_ref, void *new_data, size_t data_size);
