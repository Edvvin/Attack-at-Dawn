#include <curses.h>
#define MAX_MENUS 20
#define MAX_COLUMNS 20
#define MAX_NAME_LEN 20
#define MAX_SELECT 1000

typedef struct prog{
    int number_of_menus;
    MENU *menus[MAX_MENUS];
    char menu_names[MAX_MENUS][MAX_NAME_LEN];
    MENU* active_menu;
} PROG;

typedef struct menu{
    int number_of_columns;
    COLUMN* columns[MAX_COLUMNS];
    MENU* super_menu;
} MENU;

typedef struct column{
    int height,width,number_of_fields,first_visible;
    WINDOW *win;
    FIELD *fields, *selected_fields[MAX_SELECT];
    
} COLUMN;

typedef struct field{
    void *x,(*f)(PROG);
    char field_name[MAX_NAME_LEN 20],*field_string;
    int isSelected;
    struct field* next;
}FIELD;


PROG* new_prog();
void add_menu(MENU* M);

//Menu related functions
MENU* get_menu(char* menuName);
MENU* new_menu(char* menuName);
void run_menu(MENU* M);
void add_column(MENU* M,COLUMN* C);

//Column related functions
COLUMN* get_column(char* menuName,char* columnName);
COLUMN* new_column(char* columnName);
void add_field(COLUMN* C,FIELD F);
void print_column(COLUMN);

//Field related functions
FIELD* get_field(char* menuName,char* columnName,char* fieldName);
FIELD* new_field(void* x,void (*f)(PROG),char* fieldName,char* fieldString);
void execute(FIELD F);