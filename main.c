#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/dirent.h>
#include "DES.h"
#include "aes.h"
#include "aadcmd.h"
#include "menus.h"
#include "hash.h"
#include "keydecode.h"
#define MAX_KEY_LEN 65
#define MAX_KEY_NAME_LEN 64
#define READ_BLOCK_SIZE 10
#define FOR_ALL_SELECTED(C,F,temp) for(F = C->first_selected;F;temp = F->next_select,remove_selected_field(C,F),F=temp,print_menu(get_active_menu()))
/**
 * @file
 * @author Edvin Maid
 * @brief Funkcije vezane za ucitavanje GUI elemenata i njihovo koriscenje zajedno sa main funkciojom
 * 
 * Sve funkcije potrebne za pozivanje aplikacije sastavljenje i ucitavanje menija
 *  i pokretanje algoritama dekripcije i enkripcije
 *
 */

/**
 *@brief enum koriscen da razlikuje algoritme koji ce biti primenjivani na neki fajl
 * 
 */
typedef enum algorithm {
    NAA, AES, DES, TRIPLE_DES, AES_DECRYPT, DES_DECRYPT, TRIPLE_DES_DECRYPT
} Algorithm;

/**
 * @brief Funkcija koja loaduje sve potrebne elemente i povezuje ih u GUI
 */
void load();
/** 
 * @brief Funkcija koja se poziva u svakoj iteraciji programske petlje
 */
void update();

/**
 * @brief Konvertuje odgovarajuci enum algoritma u njegov reprezentativni string
 * @param a algoritam koji se konvertuje
 * @return niz karaktera reprezentativnog string za algoritam a
 */
char* alg2string(Algorithm a);

/**
 * @brief Funkcija koja definise akciju klika na dugme key manager u glavnom meniju
 * Funkcija koja definise akciju klika na dugme key manager u glavnom meniju
 */
void key_manager_func();
/**
 * @brief Funkcija koja definise akciju klika na dugme encryption decryption menu u glavnom meniju
 * Funkcija koja definise akciju klika na dugme encryption decryption menu u glavnom meniju
 */
void enc_dec_func();
/**
 * @brief Funkcija koja definise akciju klika na dugme exit u glavnom meniju
 * Funkcija koja definise akciju klika na dugme exit u glavnom meniju
 */
void exit_func();
/**
 * @brief funkcija koja definise akciju klika na dugme >> u key manager podmeniju
 * funkcija koja definise akciju klika na dugme >> u key manager podmeniju
 */
void km_right_func();
/**
 * @brief funkcija koja definise akciju klika na dugme << u key manager podmeniju
 * funkcija koja definise akciju klika na dugme << u key manager podmeniju
 */
void km_left_func();
/**
 * @brief funkcija koja definise akciju klika na dugme add u key manager podmeniju
 * funkcija koja definise akciju klika na dugme add u key manager podmeniju
 */
void km_add_func();
/**
 * @brief funkcija koja definise akciju klika na dugme remove u key manager podmeniju
 * funkcija koja definise akciju klika na dugme remove u key manager podmeniju
 */
void km_remv_func();
/**
 * @brief funkcija koja definise akciju klika na dugme name u key manager podmeniju
 * funkcija koja definise akciju klika na dugme name u key manager podmeniju
 */
void km_edit_func();
/**
 * @brief funkcija koja definise akciju klika na dugme clear u key manager podmeniju
 * funkcija koja definise akciju klika na dugme clear u key manager podmeniju
 */
void km_clear_func();
/**
 * @brief funkcija koja definise akciju klika na dugme load u key manager podmeniju
 * funkcija koja definise akciju klika na dugme load u key manager podmeniju
 */
void km_load_func();
/**
 * @brief funkcija koja definise akciju klika na dugme save u key manager podmeniju
 * funkcija koja definise akciju klika na dugme save u key manager podmeniju
 */
void km_save_func();
/**
 * @brief funkcija koja definise akciju klika na dugme back u key manager podmeniju
 * funkcija koja definise akciju klika na dugme back u key manager podmeniju
 */
void km_back_func();
/**
 * @brief funkcija koja definise akciju klika na dugme add u encryption decryption podmeniju
 * funkcija koja definise akciju klika na dugme add u encryption decryption podmeniju
 */
void ed_add_func();
/**
 * @brief funkcija koja definise akciju klika na dugme remove u encryption decryption podmeniju
 * funkcija koja definise akciju klika na dugme remove u encryption decryption podmeniju
 */
void ed_remv_func();
/**
 * @brief funkcija koja definise akciju klika na dugme add from file u encryption decryption podmeniju
 * funkcija koja definise akciju klika na dugme add from file u encryption decryption podmeniju
 */
void ed_addf_func();
/**
 * @brief funkcija koja definise akciju klika na dugme solve u encryption decryption podmeniju
 * funkcija koja definise akciju klika na dugme solve u encryption decryption podmeniju
 */
void ed_solve_func();
/**
 * @brief funkcija koja definise akciju klika na dugme run u encryption decryption podmeniju
 * funkcija koja definise akciju klika na dugme run u encryption decryption podmeniju
 */
void ed_run_func();
/**
 * @brief funkcija koja definise akciju klika na dugme back u encryption decryption podmeniju
 * funkcija koja definise akciju klika na dugme back u encryption decryption podmeniju
 */

/**
 * @brief funkcija koja definise akciju klika na dugme Destination u encryption decryption podmeniju
 * funkcija koja definise akciju klika na dugme Destination u encryption decryption podmeniju
 */
void ed_dest_func();

void ed_back_func();
/**
 * @brief funkcija koja definise akciju klika na polje is kolone keys u encryption decryption podmeniju
 * funkcija koja definise akciju klika na polje is kolone keys u encryption decryption podmeniju
 */
void ed_swap();
/**
 * @brief funkcija koja definise akciju klika na polje is kolone algorithm u encryption decryption podmeniju
 * funkcija koja definise akciju klika na polje is kolone algorithm u encryption decryption podmeniju
 */
void ed_pick_alg();
/**
 * @brief funkcija koja se poziva svaki put kada se pokrene encryption decryption podmeni koja prepisuje kljuceve u skrivenu kolonu
 * funkcija koja se poziva svaki put kada se pokrene encryption decryption podmeni koja prepisuje kljuceve u skrivenu kolonu
 */
void gen_aux();
/**
 * @brief funkcija koja definise akciju klika na polje is skrivene kolone keys u encryption decryption podmeniju
 * funkcija koja definise akciju klika na polje is skrivene kolone keys u encryption decryption podmeniju
 */
void aux_choose();

char** joker(char* path);
int check(char* s, char* name);

/**
 * @brief struktura koja sluzi za skladistenje kljuca u hexadecimalnoj formi zajedno sa imenom
 */
typedef struct key {
    char name[MAX_KEY_NAME_LEN], key[MAX_KEY_LEN];
    int is_named;
} KEY;

/**
 * @brief pomocna struktura za povezivanje kolona files keys i algorithm u encryption decryption podmeniju
 */
typedef struct couple {
    FIELD *file_field, *key_field, *alg_field;
    char file_name[MAX_INPUT_LEN + 1];
    KEY* key;
    Algorithm alg;
} COUPLE;

int main(int argc, char** argv) {
    if (argc > 1) {
        start_cmd_mode(argc - 1, argv + 1);
    } else {
        load();
        run_prog(get_menu("main_menu"));
        end_prog();
    }
}

void load() {
    init_prog();
    set_update(update);
    int v, wid;

    getmaxyx(stdscr, v, wid);
    double w = (double) wid;
    //Main menu
    MENU* main_menu = new_menu("main_menu");

    COLUMN* mm_spacing = new_column("mm_spacing", "", (int) (w / 80 * 20));
    add_column(main_menu, mm_spacing);

    COLUMN* mm_btns = new_column("mm_btns", "", (int) (w / 80 * 30));
    add_column(main_menu, mm_btns);
    FIELD* key_manager_btn = new_field(0, key_manager_func, 0, "k_m_btn", "Key Manager");
    add_field(mm_btns, key_manager_btn, 0);
    FIELD* enc_dec_btn = new_field(0, enc_dec_func, 0, "e_d_btn", "Encryption Decryption Menu");
    add_field(mm_btns, enc_dec_btn, 2);
    FIELD* exit_btn = new_field(0, exit_func, 0, "ext_btn", "Exit Program");
    add_field(mm_btns, exit_btn, 4);

    //key manager
    MENU* key_manager = new_menu("key_manager");

    COLUMN* km_file_keys = new_column("km_file_keys", "File", (int) (w / 80 * 20));
    set_border(km_file_keys, 1, 0, 0);
    compact(km_file_keys);
    add_column(key_manager, km_file_keys);

    COLUMN* km_left_right = new_column("km_left_right", "", (int) (w / 80 * 4));
    lockColumn(km_left_right, 1, 0);
    add_column(key_manager, km_left_right);
    FIELD* km_right_btn = new_field(0, km_right_func, 0, "kmr_btn", ">>");
    add_field(km_left_right, km_right_btn, 10);
    FIELD* km_left_btn = new_field(0, km_left_func, 0, "kmr_btn", "<<");
    add_field(km_left_right, km_left_btn, 12);

    COLUMN* km_keys = new_column("km_keys", "Keys", (int) (w / 80 * 20));
    set_border(km_keys, 1, 0, 0);
    compact(km_keys);
    add_column(key_manager, km_keys);

    COLUMN* km_btns = new_column("km_btns", "", (int) (w / 80 * 8));
    add_column(key_manager, km_btns);
    FIELD* km_add = new_field(0, km_add_func, 0, "km_add", "Add");
    add_field(km_btns, km_add, -1);
    FIELD* km_remv = new_field(0, km_remv_func, 0, "km_remv", "Remove");
    add_field(km_btns, km_remv, -1);
    FIELD* km_edit = new_field(0, km_edit_func, 0, "km_edit", "Name");
    add_field(km_btns, km_edit, -1);
    FIELD* km_clear = new_field(0, km_clear_func, 0, "km_clear", "Clear");
    add_field(km_btns, km_clear, -1);
    FIELD* km_load = new_field(0, km_load_func, 0, "km_load", "Load");
    add_field(km_btns, km_load, -1);
    FIELD* km_save = new_field(0, km_save_func, 0, "km_save", "Save");
    add_field(km_btns, km_save, -1);
    FIELD* km_back = new_field(0, km_back_func, 0, "km_back", "Back");
    add_field(km_btns, km_back, -1);

    // enc dec menu
    MENU* enc_dec = new_menu("enc_dec");

    COLUMN* ed_files = new_column("ed_files", "Files", (int) (w / 80 * 20));
    set_border(ed_files, 1, 0, 0);
    compact(ed_files);
    add_column(enc_dec, ed_files);

    COLUMN* ed_keys = new_column("ed_keys", "Keys", (int) (w / 80 * 20));
    set_border(ed_keys, 1, 0, 0);
    compact(ed_keys);
    add_column(enc_dec, ed_keys);

    COLUMN* ed_algs = new_column("ed_algs", "Algorithm", (int) (w / 80 * 20));
    set_border(ed_algs, 1, 0, 0);
    compact(ed_algs);
    add_column(enc_dec, ed_algs);

    COLUMN* ed_btns = new_column("ed_btns", "", (int) (w / 80 * 15));
    add_column(enc_dec, ed_btns);
    FIELD* ed_add = new_field(0, ed_add_func, 0, "ed_add", "Add");
    add_field(ed_btns, ed_add, -1);
    FIELD* ed_remv = new_field(0, ed_remv_func, 0, "ed_remv", "Remove");
    add_field(ed_btns, ed_remv, -1);
    FIELD* ed_addf = new_field(0, ed_addf_func, 0, "ed_addf", "Add from File");
    add_field(ed_btns, ed_addf, -1);
    FIELD* ed_solve = new_field(0, ed_solve_func, 0, "ed_sol", "Solve");
    add_field(ed_btns, ed_solve, -1);
    FIELD* ed_run = new_field(0, ed_run_func, 0, "ed_run", "Run");
    add_field(ed_btns, ed_run, -1);
    char* dest = calloc(MAX_INPUT_LEN, sizeof (char));
    FIELD* ed_dest = new_field(dest, ed_dest_func, 0, "ed_dest", "Destination");
    add_field(ed_btns, ed_dest, -1);
    FIELD* ed_back = new_field(0, ed_back_func, 0, "ed_back", "Back");
    add_field(ed_btns, ed_back, -1);

    MENU* enc_dec_hidden = new_menu("enc_dec_hidden");

    add_column(enc_dec_hidden, ed_files);

    COLUMN* ed_keys_aux = new_column("ed_keys_aux", "Choose Key", (int) (w / 80 * 20));
    set_border(ed_keys_aux, 1, 0, 0);
    compact(ed_keys_aux);
    add_column(enc_dec_hidden, ed_keys_aux);

    add_column(enc_dec_hidden, ed_algs);

    COLUMN* edh_btns = new_column("edh_btns", "", (int) (w / 80 * 15));
    FIELD* edh_back = new_field(0, ed_back_func, 0, "edh_back", "Back");
    add_field(edh_btns, edh_back, -1);
    add_column(enc_dec_hidden, edh_btns);
}

void key_manager_func() {
    next_menu(get_menu("key_manager"));
}

void enc_dec_func() {
    gen_aux();
    next_menu(get_menu("enc_dec"));
}

void exit_func() {
    stop_prog();
}

void km_back_func() {
    prev_menu();
}

void ed_back_func() {
    prev_menu();
}

void km_right_func() {
    COLUMN* C = get_column(get_active_menu(), "km_file_keys");
    COLUMN* CF = get_column(get_active_menu(), "km_keys");
    FIELD *F, *temp;

    FOR_ALL_SELECTED(C, F, temp) {
        KEY* K = malloc(sizeof (KEY));
        K->is_named = ((KEY*) F->x)->is_named;
        if (K->is_named)
            strcpy(K->name, ((KEY*) F->x)->name);
        strcpy(K->key, ((KEY*) F->x)->key);
        FIELD* Fld = new_field(K, 0, 0, "", K->is_named ? K->name : K->key);
        add_field(CF, Fld, -1);
    }
}

void km_left_func() {
    COLUMN* C = get_column(get_active_menu(), "km_keys");
    COLUMN* CF = get_column(get_active_menu(), "km_file_keys");
    FIELD *F, *temp;

    FOR_ALL_SELECTED(C, F, temp) {
        KEY* K = malloc(sizeof (KEY));
        K->is_named = ((KEY*) F->x)->is_named;
        if (K->is_named)
            strcpy(K->name, ((KEY*) F->x)->name);
        strcpy(K->key, ((KEY*) F->x)->key);
        FIELD* Fld = new_field(K, 0, 0, "", K->is_named ? K->name : K->key);
        add_field(CF, Fld, -1);
    }
}

void km_add_func() {
    COLUMN* C = get_column(get_active_menu(), "km_keys");
    char key[MAX_INPUT_LEN + 1];
    while (1) {
        input_box(10, 52, "Input key", "Input key as a series of characters,\naccepted lengths are 16, 32, 48 and 64 characters.\n(Leave empty to cancel)", key);
        if (!*key)
            break;
        int len = strlen(key);
        int i;
        for (i = 0; i < len; i++) {
            if (!strchr("0123456789ABCDEFabcdef", key[i]))break;
        }
        if (i < len) {
            message_box(10, 30, "Error", "Input must contain only\n hexadecimal digits");
            continue;
        }
        if (len == 16 || len == 32 || len == 48 || len == 64) {
            KEY* K = malloc(sizeof (KEY));
            strcpy(K->key, key);
            K->is_named = 0;
            FIELD* F = new_field(K, 0, 0, "", K->key);
            add_field(C, F, -1);
        }
        print_menu(get_active_menu());
        wrefresh(C->win);
    }
}

void km_remv_func() {
    COLUMN* C = get_column(get_active_menu(), "km_keys");
    FIELD* F = C->first_selected;
    for (F = C->first_selected; F; F = F->next_select) {
        if (F->prev_select)
            remove_field(C, F->prev_select);
    }
    if (C->first_selected)
        remove_field(C, C->first_selected);
    C = get_column(get_active_menu(), "km_file_keys");
    F = C->first_selected;
    for (F = C->first_selected; F; F = F->next_select) {
        if (F->prev_select)
            remove_field(C, F->prev_select);
    }
    if (C->first_selected)
        remove_field(C, C->first_selected);
}

void km_edit_func() {
    COLUMN* C = get_column(get_active_menu(), "km_keys");
    FIELD* F;
    char name[MAX_INPUT_LEN + 1];
    FIELD* temp;

    FOR_ALL_SELECTED(C, F, temp) {
        char str[33];
        sprintf(str, "Type in which name you would like to associate\nwith the key:\n%s", ((KEY*) F->x)->key);
        input_box(15, 52, "Name", str, name);
        if (!*name)
            break;
        ((KEY*) F->x)->is_named = 1;
        strncpy(((KEY*) F->x)->name, name,MAX_FIELD_STRING_LEN);
        strncpy(F->field_string, name,MAX_FIELD_STRING_LEN);
    }
}

void km_clear_func() {
    COLUMN* C = get_column(get_active_menu(), "km_keys");
    FIELD *F, *temp;
    FOR_ALL_SELECTED(C, F, temp);
    C = get_column(get_active_menu(), "km_file_keys");
    FOR_ALL_SELECTED(C, F, temp);
}

void km_load_func() {
    FILE* f = NULL;
    char file_name[MAX_INPUT_LEN + 4];
    do {
        input_box(10, 50, "Load", "Input file name", file_name);
        if (!*file_name)
            return;
        f = fopen(file_name, "rb");
        if (!f)
            message_box(10, 30, "Error", "Please input valid \nfile name");
    } while (!f);
    MENU* M = get_active_menu();
    COLUMN* CF = get_column(M, "km_file_keys");
    FIELD *F, *p;
    F = CF->first;
    p = F;
    while (F) {
        F = F->next;
        remove_field(CF, p);
        p = F;
    }
    int len;
    fread(&len, sizeof (int), 1, f);
    for (int i = 0; i < len; i++) {
        KEY* K = malloc(sizeof (KEY));
        fread(K, sizeof (KEY), 1, f);
        F = new_field(K, 0, 0, "", K->is_named ? K->name : K->key);
        add_field(CF, F, -1);
    }
    fclose(f);
}

void km_save_func() {
    MENU* M = get_active_menu();
    COLUMN* CF = get_column(M, "km_file_keys");
    int bad = 0, i = 0;
    char format[5] = ".kez";
    if (!CF->number_of_fields)
        return;
    char file_name[MAX_INPUT_LEN + 4];
    do {
        input_box(10, 50, "Save", "Input file name", file_name);
        if (!*file_name)return;
        for (i = 0; file_name[i]; i++) {
            char c = file_name[i];
            switch (c) {
                case '\\':case'/':case'"': case'?': case '<': case '>':case'|':case'*':case':':
                    bad = 1;
                default:
                    break;
            }
            if (bad) {
                message_box(10, 30, "Error", "File name must not contain \\ / \" ? < > | * or :");
                break;
            }

        }
    } while (bad);

    for (int j = 0; j < strlen(format); j++) {
        file_name[i++] = format[j];
    }
    file_name[i] = '\0';
    FILE* f = fopen(file_name, "wb");
    fwrite(&(CF->number_of_fields), sizeof (int), 1, f);
    FIELD* F = CF->first;
    while (F) {
        fwrite(F->x, sizeof (KEY), 1, f);
        F = F->next;
    }
    fclose(f);
}

void ed_add_func() {
    COLUMN* files = get_column(get_active_menu(), "ed_files");
    COLUMN* keys = get_column(get_active_menu(), "ed_keys");
    COLUMN* algs = get_column(get_active_menu(), "ed_algs");

    COUPLE* cup = malloc(sizeof (COUPLE));
    while (1) {
        input_box(10, 50, "Load File", "Input file name or directory", cup->file_name);
        if (!*cup->file_name) {
            free(cup);
            return;
        }
        FILE* f = fopen(cup->file_name, "r");
        if (f) {
            fclose(f);
        } else {
            message_box(10, 30, "Error", "File with given directory\nor name not found");
            continue;
        }
        cup->alg = NAA;
        cup->key = NULL;
        cup->file_field = new_field(cup, 0, 0, "", cup->file_name);
        add_field(files, cup->file_field, -1);
        cup->key_field = new_field(cup, ed_swap, nothing, "", "...");
        add_field(keys, cup->key_field, -1);
        cup->alg_field = new_field(cup, ed_pick_alg, nothing, "", alg2string(cup->alg));
        add_field(algs, cup->alg_field, -1);
        print_menu(get_active_menu());
        cup = malloc(sizeof (COUPLE));
    }
}

void ed_remv_func() {
    COLUMN* C = get_column(get_active_menu(), "ed_files");
    COLUMN* CK = get_column(get_active_menu(), "ed_keys");
    COLUMN* CA = get_column(get_active_menu(), "ed_algs");
    FIELD* F = C->first_selected;
    for (F = C->first_selected; F; F = F->next_select) {
        if (F->prev_select) {
            COUPLE* cup = F->prev_select->x;
            remove_field(CA, cup->alg_field);
            remove_field(CK, cup->key_field);
            remove_field(C, F->prev_select);
        }
    }
    if (C->first_selected) {
        COUPLE* cup = C->first_selected->x;
        remove_field(CA, cup->alg_field);
        remove_field(CK, cup->key_field);
        remove_field(C, C->first_selected);
    }
}

void ed_addf_func() {
    FILE* f = NULL;
    char file_name[MAX_INPUT_LEN + 4];
    do {
        input_box(10, 50, "Load", "Input file name", file_name);
        if (!*file_name)
            return;
        f = fopen(file_name, "r");
        if (!f)
            message_box(10, 30, "Error", "Please input valid file name");
    } while (!f);
    COLUMN* files = get_column(get_active_menu(), "ed_files");
    COLUMN* keys = get_column(get_active_menu(), "ed_keys");
    COLUMN* algs = get_column(get_active_menu(), "ed_algs");
    char str[MAX_INPUT_LEN + 4];
    char c;
    while (1) {
        int n = 0, temp;
        COUPLE* cup = calloc(1, sizeof (COUPLE));
        while (temp = fscanf(f, "%c", &c), c != '\n' && c != '\r' && temp != EOF) {
            cup->file_name[n++] = c;
        }
        cup->file_name[n] = '\0';
        FILE* check = fopen(cup->file_name, "r");
        if (check) {
            fclose(check);
            cup->file_field = new_field(cup, 0, 0, "", cup->file_name);
            add_field(files, cup->file_field, -1);
            cup->key_field = new_field(cup, ed_swap, nothing, "", "...");
            add_field(keys, cup->key_field, -1);
            cup->alg_field = new_field(cup, ed_pick_alg, nothing, "", alg2string(cup->alg));
            add_field(algs, cup->alg_field, -1);
        } else {
            free(cup);
        }
        if (temp == EOF)
            break;
    }
    fclose(f);
}

void ed_run_func() {
    MENU* M = get_active_menu();
    COLUMN* C = get_column(M, "ed_btns");
    FIELD* D = get_field(C, "ed_dest");
    char* temp = D->x;
    char* dest;
    if (strlen(temp)) {
        dest = temp;
    } else {
        dest = NULL;
    }
    COLUMN* files = get_column(get_active_menu(), "ed_files");
    COLUMN* keys = get_column(get_active_menu(), "ed_keys");
    COLUMN* algs = get_column(get_active_menu(), "ed_algs");
    FIELD *F = files->first, *p;
    p = F;
    char* k;
    while (F) {
        F = F->next;
        COUPLE* cup = p->x;
        if (cup->key && cup->alg != NAA) {
            Algorithm a = cup->alg;
            int er = 0;
            k = hex2key(cup->key->key);
            switch (a) {
                case AES:
                    er = Aes_Cipher_File(cup->file_name, k, (strlen(cup->key->key) / 8), dest);
                    break;
                case DES:
                    er = DES_encrypt_file(cup->file_name, k, dest);
                    break;
                case TRIPLE_DES:
                    er = triple_DES_encrypt_file(cup->file_name, k, dest);
                    break;
                case AES_DECRYPT:
                    er = Aes_Decipher_File(cup->file_name, k, (strlen(cup->key->key) / 8), dest);
                    break;
                case DES_DECRYPT:
                    er = DES_decrypt_file(cup->file_name, k, dest);
                    break;
                case TRIPLE_DES_DECRYPT:
                    er = triple_DES_decrypt_file(cup->file_name, k, dest);
                    break;
            }
            free(k);
            if (!er) {
                remove_field(algs, cup->alg_field);
                remove_field(keys, cup->key_field);
                remove_field(files, p);
            }

        }
        p = F;
    }
}

char* alg2string(Algorithm a) {
    static char temp[][13] = {"AES", "DES", "3DES", "AES DECRYPT", "DES DECRYPT", "3DES DECRYPT", "..."};
    switch (a) {
        case NAA:
            return temp[6];
        case AES:
            return temp[0];
        case DES:
            return temp[1];
        case TRIPLE_DES:
            return temp[2];
        case AES_DECRYPT:
            return temp[3];
        case DES_DECRYPT:
            return temp[4];
        case TRIPLE_DES_DECRYPT:
            return temp[5];
        default:
            break;
    }
    return temp[0];
}

void ed_swap() {
    MENU* hidden = get_menu("enc_dec_hidden");
    hidden->special_field = get_active_menu()->cursor_field;
    next_menu(hidden);
    if (get_column(hidden, "ed_keys_aux")->number_of_fields) {
        hidden->cursor_column = get_column(hidden, "ed_keys_aux");
        hidden->cursor_field = hidden->cursor_column->first;
    }
}

Algorithm shift_alg(Algorithm a, int len) {
    if (len == 16) {
        switch (a) {
            case NAA:
                a = DES;
                break;
            case DES:
                a = DES_DECRYPT;
                break;
            case DES_DECRYPT:
                a = DES;
        }
    } else if (len == 32) {
        switch (a) {
            case NAA:
                a = AES;
                break;
            case AES:
                a = TRIPLE_DES;
                break;
            case TRIPLE_DES:
                a = AES_DECRYPT;
                break;
            case AES_DECRYPT:
                a = TRIPLE_DES_DECRYPT;
                break;
            case TRIPLE_DES_DECRYPT:
                a = AES;
        }
    } else if (len == 48 || len == 64) {
        switch (a) {
            case NAA:
                a = AES;
                break;
            case AES:
                a = AES_DECRYPT;
                break;
            case AES_DECRYPT:
                a = AES;
        }
    }
    return a;
}

void ed_pick_alg() {
    FIELD* F = get_active_menu()->cursor_field;
    Algorithm a = ((COUPLE*) F->x)->alg;
    if (!((COUPLE*) F->x)->key)
        return;
    int len = strlen(((COUPLE*) F->x)->key->key);
    a = shift_alg(a, len);
    ((COUPLE*) F->x)->alg = a;
    set_field_string(((COUPLE*) F->x)->alg_field, alg2string(a));
}

void gen_aux() {
    MENU* hidden = get_menu("enc_dec_hidden");
    MENU* M = get_menu("key_manager");
    COLUMN* aux = get_column(hidden, "ed_keys_aux");
    COLUMN* keys = get_column(M, "km_keys");
    FIELD *F = aux->first, *p;
    p = F;
    while (F) {
        F = F->next;
        remove_field(aux, p);
        p = F;
    }
    F = keys->first;
    while (F) {
        FIELD* temp = new_field(F->x, aux_choose, nothing, "", F->field_string);
        add_field(aux, temp, -1);
        F = F->next;
    }
}

void aux_choose() {
    MENU *M = get_active_menu();
    COUPLE* cup = M->special_field->x;
    FIELD* F = M->cursor_field;
    cup->key = F->x;
    cup->alg = NAA;
    set_field_string(cup->key_field, cup->key->is_named ? cup->key->name : cup->key->key);
    set_field_string(cup->alg_field, alg2string(cup->alg));
    prev_menu();
}

void update() {
    MENU* M = get_active_menu();
    COLUMN *CF = get_column(M, "ed_files");
    if ((!strcmp(M->menu_name, "enc_dec") || !strcmp(M->menu_name, "enc_dec_hidden")) && CF->first_visible) {
        M = get_menu("enc_dec");
        MENU* A = get_active_menu();
        COLUMN *CK = get_column(M, "ed_keys");
        COLUMN *CA = get_column(M, "ed_algs");
        if (A->cursor_column == CF) {
            COUPLE* cup = CF->first_visible->x;
            CK->first_visible = cup->key_field;
            CA->first_visible = cup->alg_field;
        } else if (A->cursor_column == CK) {
            COUPLE* cup = CK->first_visible->x;
            CF->first_visible = cup->file_field;
            CA->first_visible = cup->alg_field;
        } else if (A->cursor_column == CA) {
            COUPLE* cup = CA->first_visible->x;
            CK->first_visible = cup->key_field;
            CF->first_visible = cup->file_field;
        }
    }
}

void ed_solve_func() {
    MENU* km = get_menu("key_manager");
    MENU* ed = get_menu("enc_dec");
    COLUMN* keys = get_column(km, "km_keys");
    COLUMN* files = get_column(ed, "ed_files");
    FIELD* F = files->first;
    while (F) {
        FIELD* K = keys->first;
        COUPLE* cup = F->x;
        while (K) {
            KEY* k = K->x;
            unsigned char* kk = hex2key(k->key);
            if (isGood(cup->file_name, kk, strlen(k->key) / 2, 17)) {
                cup->key = k;
                cup->alg = DES_DECRYPT;
                set_field_string(cup->key_field, cup->key->is_named ? cup->key->name : cup->key->key);
                set_field_string(cup->alg_field, alg2string(cup->alg));
                break;
            } else if (isGood(cup->file_name, kk, strlen(k->key) / 2, 71)) {
                cup->key = k;
                cup->alg = TRIPLE_DES_DECRYPT;
                set_field_string(cup->key_field, cup->key->is_named ? cup->key->name : cup->key->key);
                set_field_string(cup->alg_field, alg2string(cup->alg));
                break;
            } else if (isGood(cup->file_name, kk, strlen(k->key) / 2, 23)) {
                cup->key = k;
                cup->alg = AES_DECRYPT;
                set_field_string(cup->key_field, cup->key->is_named ? cup->key->name : cup->key->key);
                set_field_string(cup->alg_field, alg2string(cup->alg));
                break;
            } else if (isGood(cup->file_name, kk, strlen(k->key) / 2, 47)) {
                cup->key = k;
                cup->alg = AES_DECRYPT;
                set_field_string(cup->key_field, cup->key->is_named ? cup->key->name : cup->key->key);
                set_field_string(cup->alg_field, alg2string(cup->alg));
                break;
            } else if (isGood(cup->file_name, kk, strlen(k->key) / 2, 61)) {
                cup->key = k;
                cup->alg = AES_DECRYPT;
                set_field_string(cup->key_field, cup->key->is_named ? cup->key->name : cup->key->key);
                set_field_string(cup->alg_field, alg2string(cup->alg));
                break;
            }
            K = K->next;
            free(kk);
        }
        F = F->next;
    }
}

void ed_dest_func() {
    char str[MAX_INPUT_LEN];
    MENU* M = get_active_menu();
    COLUMN* C = get_column(M, "ed_btns");
    FIELD* F = get_field(C, "ed_dest");
    char* dest = F->x;
    for (int i = strlen(dest) - 1; i >= 0; i--) {
        ungetch(dest[i]);
    }
    while (1) {
        input_box(10, 45, "Destination", "Input destination of encryption/decryption", str);
        struct stat sb;
        if (!*str) {
            break;
        }
        if (stat(str, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            break;
        } else {
            message_box(6, 30, "Error", "Given destination not found");
        }
    }
    strcpy(dest, str);
}