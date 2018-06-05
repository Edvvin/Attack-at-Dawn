#include <curses.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#define MAX_MENUS 20
#define MAX_COLUMNS 20
#define MAX_NAME_LEN 64
#define MAX_FIELD_NAME_LEN 8
#define MAX_SELECT 2000
#define MAX_FIELD_STRING_LEN 65
#define MAX_ERROR_MESSAGE_LEN 100
#define MAX_INPUT_LEN 500
#define MENUS_ERR 0
#define MENUS_OK 1
#define HEADER_WIDTH 0
#define edvin endwin



typedef enum menus_error {
    M_ERR_NO_ERROR,M_ERR_NO_INITIALIZED_PROG, M_ERR_NULL_PONITER, M_ERR_OVERFLOW, M_ERR_UNDERFLOW, M_ERR_ALLOC, M_ERR_FIELD_POSITION_FILLED,M_ERR_PROG_ALREADY_RUNNING,M_ERR_PROG_ALREADY_ACTIVE,M_ERR_SCREEN_DIM
} MENUS_ERROR;

static MENUS_ERROR m_err;

typedef struct field {
    void *x, (*f)(void),(*free_func)(void*);
    char field_name[MAX_FIELD_NAME_LEN], field_string[MAX_FIELD_STRING_LEN];
    int is_selected,position;
    struct field *next, *prev,*next_select,*prev_select;
} FIELD;

typedef struct column {
    int number_of_fields, number_of_selected,width,hasBorder,a_bor,b_bor,is_locked,lock_pos,isCompact,is_designated;
    WINDOW *win;
    char column_name[MAX_NAME_LEN],column_title[MAX_NAME_LEN];
    FIELD *first, *last, *first_visible,*first_selected,*last_selected;
} COLUMN;

typedef struct menu {
    int number_of_columns,start_col;
    COLUMN* columns[MAX_COLUMNS];
    char *column_names[MAX_COLUMNS], menu_name[MAX_NAME_LEN];
    FIELD* cursor_field,*special_field;
    COLUMN* cursor_column;
} MENU;

typedef struct prog {
    void (*update)(void);
    int number_of_menus, top, is_stopped,column_height,max_width;
    MENU *menus[MAX_MENUS];
    char *menu_names[MAX_MENUS];
    MENU* menu_stack[MAX_MENUS];
} PROG;

static PROG* active_prog = NULL;

unsigned char* hex2key(char* key);
int init_prog();
void set_update(void(*update)(void));
int run_prog(MENU* startMenu);
void stop_prog();
int end_prog();
MENU* get_active_menu();
void nothing(void*);
void nothing2(void);

//Menu related functions
MENU* get_menu(char* menuName);
MENU* new_menu(char* menuName);
int update_menu(MENU* M);
int add_column(MENU* M, COLUMN* C);
int prev_menu();
int next_menu(MENU* M);
void print_menu(MENU* M);

//Column related functions
COLUMN* get_column(MENU* M, char* columnName);
COLUMN* new_column(char* columnName,char* columnTitle,int width);
int add_field(COLUMN* C, FIELD* F,int position);
int print_column(COLUMN* C);
void set_border(COLUMN* C,int hasBorder,int a,int b);
void lockColumn(COLUMN* C,int is_locked,int lock_pos);
void compact(COLUMN* C);

//Field related functions
FIELD* get_field(COLUMN* C, char* fieldName);
FIELD* new_field(void* x, void (*f)(void), void(*free_func)(void*), char* fieldName, char* fieldString);
void set_field_string(FIELD* F,char* fieldString);
void execute(FIELD* F);
void select();
FIELD* get_f(COLUMN* C, int index);
void remove_field(COLUMN* C, FIELD* F);
void remove_selected_field(COLUMN* C, FIELD* F);
int getPos(COLUMN* C,FIELD* F);
void print_field(COLUMN* C,FIELD* F,int pos);
void print_title(COLUMN* C, char* s);
void free_field(FIELD* F);

//cursor
int cursor_up();
int cursor_down();
int cursor_left();
int cursor_right();

void input_box(int height,int width,char* title,char* message,char* input);
void message_box(int height,int width,char* title,char* message);