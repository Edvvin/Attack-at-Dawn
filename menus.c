#include "menus.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>

void nothing2(){
    
}

int init_prog() {
    if (active_prog) {
        m_err = M_ERR_PROG_ALREADY_ACTIVE;
        return MENUS_ERR;
    }
    PROG* p = (PROG*) calloc(1, sizeof (PROG));
    if (!p) {
        m_err = M_ERR_ALLOC;
        return MENUS_ERR;
    }
    active_prog = p;
    active_prog->is_stopped = 1;
    active_prog->top = 0;
    active_prog->update = nothing2;
    struct winsize w;
    /*
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        active_prog->column_height = w.ws_row;
        active_prog->max_width = w.ws_col;
     */
    initscr();
    getmaxyx(stdscr, active_prog->column_height, active_prog->max_width);
    noecho();
    cbreak();
    srand(time(0));
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    }
    keypad(stdscr, TRUE);
    return MENUS_OK;
}

int run_prog(MENU* startMenu) {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    if (!active_prog->is_stopped) {
        m_err = M_ERR_PROG_ALREADY_RUNNING;
        return MENUS_ERR;
    }
    for (int i = 0; i < startMenu->number_of_columns; i++) {
        if (startMenu->columns[i]->number_of_fields) {
            startMenu->cursor_column = startMenu->columns[i];
            startMenu->cursor_column->first_visible = startMenu->cursor_field = startMenu->columns[i]->first;
            break;
        }
    }
    active_prog->menu_stack[active_prog->top++] = startMenu;
    active_prog->is_stopped = 0;
    while (!active_prog->is_stopped) {
        update_menu(get_active_menu());
    }
    return MENUS_OK;
}

void stop_prog() {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return;
    }
    active_prog->is_stopped = 1;
}
void set_update(void(*update)(void)){
    if(!active_prog)
        return;
    active_prog->update = update;
}
int end_prog() {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    int n = 0;
    for (int i = 0; i < active_prog->number_of_menus; i++) {
        MENU* M = active_prog->menus[i];
        n += M->number_of_columns;
    }

    COLUMN **q = malloc(sizeof (COLUMN*) * n);
    n = 0;
    for (int i = 0; i < active_prog->number_of_menus; i++) {
        MENU* M = active_prog->menus[i];
        for (int j = 0; j < M->number_of_columns; j++) {
            COLUMN* C = M->columns[j];
            if (!C->is_designated) {
                while (C->first) {
                    remove_field(C, C->first);
                }
                delwin(C->win);
                C->is_designated = 1;
                q[n++] = C;
            }
        }
        free(M);
    }
    for(int i = 0;i<n;i++){
        free(q[i]);
    }
    free(q);
    edvin();
    free(active_prog);
    active_prog = NULL;
    return MENUS_OK;
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

//MENU FUNCTIONS

int next_menu(MENU* M) {
    if (!active_prog) {
        m_err = M_ERR_NO_INITIALIZED_PROG;
        return MENUS_ERR;
    }
    if (active_prog->top == MAX_MENUS) {
        m_err = M_ERR_OVERFLOW;
        return MENUS_ERR;
    }
    for (int i = 0; i < M->number_of_columns; i++) {
        if (M->columns[i]->number_of_fields) {
            M->cursor_column = M->columns[i];
            M->cursor_column->first_visible = M->cursor_field = M->columns[i]->first;
            break;
        }
    }
    active_prog->menu_stack[active_prog->top++] = M;
    clear();
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
    MENU* M = active_prog->menu_stack[--active_prog->top];
    for (int i = 0; i < M->number_of_columns; i++) {
        if (M->columns[i]->number_of_fields) {
            M->cursor_column = M->columns[i];
            M->cursor_column->first_visible = M->cursor_field = M->columns[i]->first;
            break;
        }
    }
    clear();
    return MENUS_OK;
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
    strcpy(M->menu_name, menuName);
    active_prog->menus[active_prog->number_of_menus] = M;
    active_prog->menu_names[active_prog->number_of_menus++] = M->menu_name;
    M->start_col = 1;
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
    if (C->width + M->start_col >= active_prog->max_width) {
        m_err = M_ERR_SCREEN_DIM;
        return MENUS_ERR;
    }
    C->win = newwin(active_prog->column_height, C->width, HEADER_WIDTH, M->start_col);
    //refresh();
    M->start_col += (C->width + 1);
    M->columns[M->number_of_columns] = C;
    M->column_names[M->number_of_columns++] = C->column_name;
    return MENUS_OK;
}

void execute(FIELD* F) {
    (*F->f)();
}

void print_menu(MENU* M) {
    for (int i = 0; i < M->number_of_columns; i++) {
        print_column(M->columns[i]);
    }
}

int update_menu(MENU* M) {
    if (!M) {
        m_err = M_ERR_NULL_PONITER;
        return MENUS_ERR;
    }
    active_prog->update();
    refresh();
    print_menu(M);
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
        case KEY_ENTER:case 10:
            execute(M->cursor_field);
            break;
    }
}

void select() {
    MENU* M = get_active_menu();
    if (M->cursor_field->is_selected) {
        if (M->cursor_column->number_of_selected == 1) {
            M->cursor_column->last_selected = NULL;
            M->cursor_column->first_selected = NULL;
            M->cursor_field->next_select = M->cursor_field->prev_select = NULL;
        } else if (!M->cursor_field->next_select) {
            M->cursor_field->prev_select->next_select = NULL;
            M->cursor_field->prev_select = NULL;
        } else if (!M->cursor_field->prev_select) {
            M->cursor_field->next_select->prev_select = NULL;
            M->cursor_field->next_select = NULL;
        } else {
            M->cursor_field->prev_select->next_select = M->cursor_field->next_select;
            M->cursor_field->next_select->prev_select = M->cursor_field->prev_select;
            M->cursor_field->next_select = NULL;
            M->cursor_field->prev_select = NULL;
        }
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
    return;
}

COLUMN* new_column(char* columnName, char* columnTitle, int width) {
    COLUMN* col = (COLUMN*) calloc(1, sizeof (COLUMN));
    if (!col) {
        m_err = M_ERR_ALLOC;
        return MENUS_ERR;
    }
    width = (width >= 0 ? width : -width);
    strcpy(col->column_name, columnName);
    strcpy(col->column_title, columnTitle);
    col->width = width;
    col->a_bor = col->b_bor = col->hasBorder = col->is_designated = 0;
    return col;
}

void compact(COLUMN* C) {
    C->isCompact = 1;
}

void lockColumn(COLUMN* C, int is_locked, int lock_pos) {
    C->is_locked = is_locked;
    C->lock_pos = lock_pos;
}

void set_border(COLUMN* C, int hasBorder, int a, int b) {
    if (C) {
        C->hasBorder = hasBorder;
        C->a_bor = a;
        C->b_bor = b;
    }
}

FIELD* new_field(void* x, void (*func)(void), void (*freeFunc)(void*), char* fieldName, char* fieldString) {
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
    if (!freeFunc) {
        F->free_func = free;
    } else {
        F->free_func = freeFunc;
    }
    F->x = x;
    strcpy(F->field_name, fieldName);
    strcpy(F->field_string, fieldString);
    F->position = -1;
    return F;
}

void set_field_string(FIELD* F, char* fieldString) {
    if (!F)
        return;
    strcpy(F->field_string, fieldString);
}

FIELD* get_f(COLUMN* C, int index) {
    FIELD* curr = C->first;
    int i = 0;
    while (curr && i != index) {
        curr = curr->next;
        i++;
    }
    return curr;
}

void remove_selected_field(COLUMN* C, FIELD* F) {
    if (!F->is_selected)
        return;
    FIELD *p = F->prev_select, *n = F->next_select;
    if (!p && !n) {
        C->first_selected = C->last_selected = 0;
    } else if (!p) {
        n->prev_select = NULL;
        C->first_selected = n;
    } else if (!n) {
        p->next_select = NULL;
        C->last_selected = p;
    } else {
        p->next_select = n;
        n->prev_select = p;
    }
    F->is_selected = 0;
    F->prev_select = F->next_select = NULL;
    C->number_of_selected--;
}

void remove_field(COLUMN* C, FIELD* F) {
    FIELD *p = F->prev, *n = F->next;
    if (F->is_selected) {
        remove_selected_field(C, F);
    }

    if (!p && !n) {
        C->first = C->last = 0;
    } else if (!p) {
        n->prev = NULL;
        C->first = n;
    } else if (!n) {
        p->next = NULL;
        C->last = p;
    } else {
        p->next = n;
        n->prev = p;
    }
    if (C->first_visible == F)
        if (n)
            C->first_visible = n;
        else if (p)
            C->first_visible = p;
        else
            C->first_visible = NULL;
    free_field(F);
    C->number_of_fields--;
}

int add_field(COLUMN* C, FIELD* F, int position) {
    if (!(C && F)) {
        m_err = M_ERR_NULL_PONITER;
        return MENUS_ERR;
    }
    F->prev = NULL;
    F->next = NULL;
    if (!C->number_of_fields) {
        C->first = F;
        C->last = F;
        C->number_of_fields = 1;
        C->first_visible = F;
        if (position < 0)
            F->position = 0;
        else
            F->position = position;
        return MENUS_OK;
    }
    if (position >= 0) {
        F->position = position;
        FIELD* temp = C->first;
        if (C->isCompact) {
            int i = 0;
            while (temp && i <= F->position) {
                temp = temp->next;
                i++;
            }
        } else {
            while (temp && temp->position <= F->position) {
                temp = temp->next;
            }
        }
        if (!temp) {
            if (C->isCompact || F->position >= C->last->position) {
                C->last->next = F;
                F->prev = C->last;
                C->last = F;
            } else {
                temp = C->last;
                F->next = temp;
                F->prev = temp->prev;
                if (F->prev)
                    F->prev->next = F;
                temp->prev = F;
            }
        } else {
            F->next = temp;
            F->prev = temp->prev;
            if (F->prev)
                F->prev->next = F;
            temp->prev = F;
        }
    } else {
        F->position = C->last->position + 1;
        C->last->next = F;
        F->prev = C->last;
        C->last = F;
    }
    C->number_of_fields++;
    return MENUS_OK;
}

int cursor_left() {
    int n = 0;
    MENU* M = get_active_menu();
    while (M->columns[n] != M->cursor_column)n++;
    if (!n)return MENUS_OK;
    int i;
    for (i = n - 1; i >= 0; i--) {
        if (M->columns[i]->number_of_fields)
            break;
    }
    if (i != -1) {
        M->cursor_column = M->columns[i];
        M->cursor_field = M->cursor_column->first_visible;
    }
    return MENUS_OK;
}

int getPos(COLUMN* C, FIELD* F) {
    if (!C->isCompact)
        return F->position;
    int i = 0;
    FIELD* curr = C->first;
    while (curr && curr != F) {
        curr = curr->next;
        i++;
    }
    if (!curr)
        return -1;
    return i;
}

int cursor_right() {
    int n = 0;
    MENU* M = get_active_menu();
    while (M->columns[n] != M->cursor_column)n++;
    if (n == M->number_of_columns)return MENUS_OK;
    int i;
    for (i = n + 1; i < M->number_of_columns; i++) {
        if (M->columns[i]->number_of_fields)
            break;
    }
    if (i != M->number_of_columns) {
        M->cursor_column = M->columns[i];
        M->cursor_field = M->cursor_column->first_visible;
    }
    return MENUS_OK;
}

int cursor_up() {
    MENU* M = get_active_menu();
    if (!M->cursor_field->prev)
        return MENUS_OK;
    M->cursor_field = M->cursor_field->prev;
    if (getPos(M->cursor_column, M->cursor_field) < getPos(M->cursor_column, M->cursor_column->first_visible)) {
        M->cursor_column->first_visible = M->cursor_field;
    }
    return MENUS_OK;
}

int cursor_down() {
    MENU* M = get_active_menu();
    if (!M->cursor_field->next)
        return MENUS_OK;
    M->cursor_field = M->cursor_field->next;
    while (getPos(M->cursor_column, M->cursor_field) > getPos(M->cursor_column, M->cursor_column->first_visible) + active_prog->column_height - 3) {
        M->cursor_column->first_visible = M->cursor_column->first_visible->next;
    }
    return MENUS_OK;
}

int print_column(COLUMN* C) {
    wclear(C->win);
    if (C->hasBorder)
        box(C->win, C->a_bor, C->b_bor);
    print_title(C, C->column_title);
    int pos = 1;
    FIELD* curr = C->first_visible;
    if (C->is_locked) {
        curr = C->first;
        while (curr && curr->position < C->lock_pos)
            curr = curr->next;
        if (curr)
            pos = curr->position - C->lock_pos + 1;
    }
    while (curr && pos < active_prog->column_height - 1) {
        print_field(C, curr, pos);
        int temp = curr->position;
        curr = curr->next;
        if (!curr)break;
        if (C->isCompact)
            pos++;
        else
            pos += curr->position - temp;
    }
    wrefresh(C->win);
}

void print_field(COLUMN* C, FIELD* F, int pos) {
    char str[MAX_NAME_LEN];
    MENU* M = get_active_menu();
    int i = 0;
    char* fieldString = F->field_string;
    while (*fieldString != '\0' && *fieldString != '\n')
        str[i++] = *fieldString++;
    str[i] = '\0';
    if (i > C->width - 2) {
        i = C->width - 2;
        if (i > 2)
            str[i - 1] = '.';
        if (i > 3)
            str[i - 2] = '.';
        if (i > 4)
            str[i - 3] = '.';
    }
    wmove(C->win, pos, 1);
    if (F == M->cursor_field) {
        if (has_colors()) {
            wattron(C->win, COLOR_PAIR(1));
        } else {
            wattron(C->win, A_REVERSE);
        }
    } else if (F->is_selected)
        wattron(C->win, A_REVERSE);
    for (int j = 0; j < i; j++) {
        wprintw(C->win, "%c", str[j]);
    }
    if (F == M->cursor_field) {
        if (has_colors()) {
            wattroff(C->win, COLOR_PAIR(1));
        } else {
            wattroff(C->win, A_REVERSE);
        }
    } else if (F->is_selected)
        wattroff(C->win, A_REVERSE);
}

void print_title(COLUMN* C, char* s) {
    char str[MAX_NAME_LEN];
    int i = 0;
    char* fieldString = s;
    while (*fieldString != '\0' && *fieldString != '\n')
        str[i++] = *fieldString++;
    str[i] = '\0';
    if (i > C->width - 2) {
        i = C->width - 2;
        if (i > 2)
            str[i - 1] = '.';
        if (i > 3)
            str[i - 2] = '.';
        if (i > 4)
            str[i - 3] = '.';
    }
    wmove(C->win, 0, 1);
    for (int j = 0; j < i; j++) {
        wprintw(C->win, "%c", str[j]);
    }
}

void free_field(FIELD * F) {
    if (F->x)
        F->free_func(F->x);
    free(F);
}

void message_box(int height, int width, char* title, char* message) {
    WINDOW* W = newwin(height, width, active_prog->column_height / 2 - height / 2, active_prog->max_width / 2 - width / 2);
    refresh();
    box(W, 0, 0);
    mvwprintw(W, 0, 1, "%s", title);
    char* c = message;
    int i = 1, j = 0;
    wmove(W, 1, 1);
    while (*c != '\0') {
        if (*c == '\n' || j % (width - 2) == (width - 3)) {
            wmove(W, ++i, 1);
            j = 0;
        } else {
            wprintw(W, "%c", *c);
            j++;
        }
        c++;
    }
    wrefresh(W);
    while (1) {
        char c = getch();
        if (c == KEY_ENTER || c == 10)
            break;
    }
    delwin(W);
    clear();
    refresh();
    print_menu(get_active_menu());
}

void input_box(int height, int width, char* title, char* message, char* input) {
    char str[MAX_INPUT_LEN];
    str[0] = '\0';
    WINDOW* W = newwin(height, width, active_prog->column_height / 2 - height / 2, active_prog->max_width / 2 - width / 2);
    WINDOW* inwin = newwin(3, width - 2, active_prog->column_height / 2 - height / 2 + height - 5, active_prog->max_width / 2 - width / 2 + 1);
    refresh();
    box(W, 0, 0);
    box(inwin, 0, 0);
    wrefresh(W);
    wrefresh(inwin);
    mvwprintw(W, 0, 1, "%s", title);
    char* c = message;
    int i = 1, j = 0, count = 0;
    wmove(W, 1, 1);
    while (*c != '\0') {
        if (*c == '\n' || j % (width - 2) == (width - 3)) {
            wmove(W, ++i, 1);
            j = 0;
        } else {
            wprintw(W, "%c", *c);
            j++;
        }
        c++;
    }
    wrefresh(W);
    while (1) {
        wrefresh(inwin);
        char c = getch();
        if (c == KEY_ENTER || c == 10) {
            break;
        } else if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127 || c == 8) && count > 0) {
            str[--count] = '\0';
        } else if (isprint(c) && count < MAX_INPUT_LEN) {
            str[count++] = c;
            str[count] = '\0';
        }
        wclear(inwin);
        box(inwin, 0, 0);
        if (count - (width - 4) <= 0) {
            mvwprintw(inwin, 1, 1, "%s", str);
        } else {
            mvwprintw(inwin, 1, 1, "%s", str + count - (width - 4));
        }
    }
    strcpy(input, str);
    delwin(inwin);
    delwin(W);
    clear();
    refresh();
    print_menu(get_active_menu());
}

void nothing(void* v) {
    return;
}