#include <curses.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_MENUS 20
#define MAX_COLUMNS 20
#define MAX_NAME_LEN 20
#define MAX_SELECT 2000
#define MAX_ERROR_MESSAGE_LEN 100
#define MENUS_ERR 0
#define MENUS_OK 1

static PROG* active_prog = NULL;

typedef enum menus_error {
    M_ERR_NO_INITIALIZED_PROG, M_ERR_NULL_PONITER, M_ERR_OVERFLOW, M_ERR_UNDERFLOW, M_ERR_ALLOC, M_ERR_FIELD_POSITION_FILLED
} MENUS_ERROR;

static MENUS_ERROR m_err;

typedef struct prog {
    int number_of_menus, top, is_stopped,column_height;
    MENU *menus[MAX_MENUS];
    char *menu_names[MAX_MENUS];
    MENU* menu_stack[MAX_MENUS];
} PROG;

typedef struct menu {
    int number_of_columns;
    COLUMN* columns[MAX_COLUMNS];
    char *column_names[MAX_COLUMNS], menu_name[MAX_NAME_LEN];
    FIELD* cursor_field;
    COLUMN* cursor_column;
} MENU;

typedef struct column {
    int number_of_fields, number_of_selected,width;
    WINDOW *win;
    char column_name[MAX_NAME_LEN];
    FIELD *first, *last, *first_visible,*first_selected,*last_selected;
} COLUMN;

typedef struct field {
    void *x, (*f)(void);
    char field_name[MAX_NAME_LEN], *field_string;
    int is_selected,position;
    struct field *next, *prev,*next_select,*prev_select;
} FIELD;


int init_prog();
int run_prog(MENU startMenu);
int stop_prog();
int end_prog();
MENU* get_active_menu();

//Menu related functions
MENU* get_menu(char* menuName);
MENU* new_menu(char* menuName);
int update_menu(MENU* M);
int add_column(MENU* M, COLUMN* C);
int prev_menu();
int next_menu(MENU* M);


//Column related functions
COLUMN* get_column(MENU* M, char* columnName);
COLUMN* new_column(char* columnName,int width);
int add_field(COLUMN* C, FIELD F);
int print_column(COLUMN* C);

//Field related functions
FIELD* get_field(COLUMN* C, char* fieldName);
FIELD* new_field(void* x, void (*f)(void), char* fieldName, char* fieldString);
int execute(FIELD F);
int select(FIELD F);

//cursor
int cursor_up();
int cursor_down();
int cursor_left();
int cursor_right();