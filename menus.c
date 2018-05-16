#include "menus.h"

int init_prog() {
    PROG* p = (PROG*) calloc(1, sizeof (PROG));
    if (!p) {
        m_err = M_ERR_ALLOC;
        return MENUS_ERR;
    }
    active_prog = p;

    initscr();
    noecho();
    raw();
    start_color();
    keypad();

    return MENUS_OK;
}

int run_prog(MENU* startMenu) {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    active_prog->is_stopped = 0;
    while (!active_prog->is_stopped) {
        update_menu(get_active_menu());
    }
}

int stop_prog() {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    active_prog->is_stopped = 1;
}

int next_menu(MENU* M) {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    if (active_prog->top == MAX_MENUS) {
        m_err = M_ERR_OVERFLOW;
        return MENUS_ERR;
    }
    active_prog->menu_stack[active_prog->top++] = M;
    return MENUS_OK;
}

int prev_menu() {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    if (!active_prog->top) {
        m_err = M_ERR_UNDERFLOW;
        return MENUS_ERR;
    }
    active_prog->top--;
}

MENU* get_active_menu() {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    if (!active_prog->top)
        return NULL;
    return active_prog->menu_stack[active_prog->top - 1];
}

MENU* get_menu(char* menuName) {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    for (int i = 0; i < active_prog->number_of_menus; i++) {
        if (!strcmp(active_prog->menu_names[i], menuName)) {
            return active_prog->menus[i];
        }
    }
    return NULL;
}

COLUMN* get_column(MENU* M, char* columnName) {
    if (!M) {
        m_err = M_ERR_ALLOC;
        return MENUS_ERR;
    }
    for (int i = 0; i < M->number_of_columns; i++) {
        if (!strcmp(columnName, M->column_names[i]))
            return M->columns[i];
    }
    return NULL;
}

FIELD* get_field(COLUMN* C, char* fieldName) {
    if (!C) {
        m_err = M_ERR_ALLOC;
        return MENUS_ERR;
    }
    FIELD* next = C->first;
    while (next) {
        if (!strcmp(next->field_name, fieldName))
            return next;
        next = next->next;
    }
    return NULL;
}

MENU* new_menu(char* menuName) {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    if (active_prog->number_of_menus == MAX_MENUS) {
        m_err = M_ERR_OVERFLOW;
        return MENUS_ERR;
    }
    MENU* M = (MENU*) calloc(1, sizeof (MENU));
    if (!M) {
        m_err = M_ERR_ALLOC;
        return MENUS_ERR;
    }
    M->menu_name = menuName;
    active_prog->menus[active_prog->number_of_menus] = M;
    active_prog->menu_names[active_prog->number_of_menus++] = M->menu_name;
    return M;
}

int add_column(MENU* M, COLUMN* C) {
    if (!(M && C)) {
        m_err = M_ERR_NULL_PONITER;
        return MENUS_ERR;
    }
    if (M->number_of_columns == MAX_COLUMNS) {
        m_err = M_ERR_OVERFLOW;
        return MENUS_ERR;
    }
    M->columns[M->number_of_columns] = C;
    M->column_names[M->number_of_columns++] = C->column_name;
    return MENUS_OK;
}

int execute(FIELD F) {
    (*F->f)();
}

int cursor_up() {
    MENU* curr = get_active_menu();
    if (curr->cursor_field->position == 0) {
        if (curr->cursor_column->first_visible->position > 0)
            curr->cursor_column->first_visible = curr->cursor_column->first_visible->prev;
    }
}

int update_menu(MENU* M) {
    if (!M) {
        m_err = M_ERR_NULL_PONITER;
        return MENUS_ERR;
    }
    for (int i = 0; i < M->number_of_columns; i++) {
        print_column(M->columns[i]);
    }
    chtype input = getch();
    switch (input) {
        case KEY_UP:
            cursor_up();
            break;
        case KEY_DOWN:
            cursor_down();
            break;
        case KEY_LEFT:
            cursor_left();
            break;
        case KEY_RIGHT:
            cursor_right();
            break;
        case KEY_ENTER:
            execute(M->cursor_field);
            break;
    }
}

void select() {
    MENU* M = get_active_menu();
    if (M->cursor_field->is_selected) {
        M->cursor_field->prev_select->next_select = M->cursor_field->next_select;
        M->cursor_field->next_select->prev_select = M->cursor_field->prev_select;
        M->cursor_field->next_select = NULL;
        M->cursor_field->prev_select = NULL;
        M->cursor_field->is_selected = 0;
        M->cursor_column->number_of_selected--;
    } else {
        if (!M->cursor_column->number_of_selected) {
            M->cursor_column->first_selected = M->cursor_field;
            M->cursor_column->last_selected = M->cursor_field;
            M->cursor_column->number_of_selected = 1;
            M->cursor_field->next_select = NULL;
            M->cursor_field->prev_select = NULL;
            M->cursor_field->is_selected = 1;
        } else {
            M->cursor_column->last_selected->next_select = M->cursor_field;
            M->cursor_field->prev_select = M->cursor_column->last_selected;
            M->cursor_column->last_selected = M->cursor_field;
            M->cursor_column->number_of_selected++;
            M->cursor_field->is_selected = 1;
        }
    }
}

COLUMN* new_column(char* columnName, int width) {
    COLUMN* col = (COLUMN*) calloc(1, sizeof (COLUMN));
    col->column_name = columnName;
    col->width = width;
    return col;
}

FIELD* new_field(void* x, void (*func)(void), char* fieldName, char* fieldString) {
    FIELD* F = calloc(1, sizeof (FIELD));
    if (!F) {
        m_err = M_ERR_ALLOC;
        return MENUS_ERR;
    }
    if (!func) {
        F->f = select;
    } else {
        F->f = func;
    }
    F->field_name = fieldName;
    F->field_string = fieldString;
    F->position = -1;
    return F;
}

int add_field(COLUMN* C, FIELD* F, int position) {
    if (!(C & F)) {
        m_err = M_ERR_NULL_PONITER;
        return MENUS_ERR;
    }
    if(!C->number_of_fields){
        C->first = F;
        C->last = F;
        C->number_of_fields = 1;
        C->first_visible = F;
        if(position<0)
            F->position = 0;
        else
            F->position = position;
        return MENUS_OK;
    }
    if (position >= 0) {
        F->position = position;
        FIELD* temp = C->first;
        while (temp && temp->position < F->position) {
            temp = temp->next;
        }
        if (!temp) {
            C->last->next = F;
            F->prev = C->last->prev;
            C->last = F;
        } else {
            if(temp->position == F->position){
                m_err = M_ERR_FIELD_POSITION_FILLED;
                return MENUS_ERR;
            }
            temp->prev->next = F;
            F->prev = temp->prev;
            temp->prev = F;
            F->next = temp;
        }
    }
    else{
        F->position = C->last->position+1;
        C->last->next = F;
        F->prev = C->last;
        F->next = NULL;
    }
    C->number_of_fields++;
    return MENUS_OK;
}

int cursor_left(){
    int n = 0;
    MENU* M = get_active_menu();
    while(M->columns[n]!=M->cursor_column)n++;
    if(!n)return MENUS_OK;
    int i;
    for(i = n-1;i>=0;i++){
        if(M->columns[i]->number_of_fields)
            break;
    }
    if(i!=-1){
        M->cursor_column = M->columns[i];
        M->cursor_field = M->cursor_column->first;
    }
    return MENUS_OK;
}
int cursor_right(){
    int n = 0;
    MENU* M = get_active_menu();
    while(M->columns[n]!=M->cursor_column)n++;
    if(n == M->number_of_columns)return MENUS_OK;
    int i;
    for(i = n+1;i<M->number_of_columns;i++){
        if(M->columns[i]->number_of_fields)
            break;
    }
    if(i!=M->number_of_columns){
        M->cursor_column = M->columns[i];
        M->cursor_field = M->cursor_column->first;
    }
    return MENUS_OK;
}
int cursor_up(){
    MENU* M = get_active_menu();
    if(!M->cursor_field->prev)
        return MENUS_OK;
    M->cursor_field = M->cursor_field->prev;
    if(M->cursor_field->position < M->cursor_column->first_visible->position){
        M->cursor_column->first_visible = M->cursor_field;
    }
    return MENUS_OK;
}
int cursor_down(){
    MENU* M = get_active_menu();
    if(!M->cursor_field->next)
        return MENUS_OK;
    M->cursor_field = M->cursor_field->next;
    while(M->cursor_field->position > M->cursor_column->first_visible->position + active_prog->column_height - 1){
        M->cursor_column->first_visible = M->cursor_column->first_visible->next;
    }
    return MENUS_OK;
}