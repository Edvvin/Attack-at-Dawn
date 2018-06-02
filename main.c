#include <stdio.h>
#include <stdlib.h>
#include "menus.h"

void load();
void key_manager_func();
void enc_dec_func();
void exit_func();
void km_right_func();
void km_left_func();
void km_add_func();
void km_remv_func();
void km_edit_func();
void km_load_func();
void km_save_func();
void km_back_func();
void ed_add_func();
void ed_remv_func();
void ed_addf_func();
void ed_back_func();

int main(int argc, char** argv) {
    load();
    run_prog(get_menu("main_menu"));
    end_prog();
}

void load() {
    init_prog();

    //Main menu
    MENU* main_menu = new_menu("main_menu");

    COLUMN* mm_spacing = new_column("mm_spacing", "", 20);
    add_column(main_menu, mm_spacing);

    COLUMN* mm_btns = new_column("mm_btns", "", 30);
    add_column(main_menu, mm_btns);
    FIELD* key_manager_btn = new_field(0, key_manager_func, 0, "k_m_btn", "Key Manager");
    add_field(mm_btns, key_manager_btn, 0);
    FIELD* enc_dec_btn = new_field(0, enc_dec_func, 0, "e_d_btn", "Encryption Decryption Menu");
    add_field(mm_btns, enc_dec_btn, 2);
    FIELD* exit_btn = new_field(0, exit_func, 0, "ext_btn", "Exit Program");
    add_field(mm_btns, exit_btn, 4);

    //key manager
    MENU* key_manager = new_menu("key_manager");

    COLUMN* km_file_keys = new_column("km_file_keys", "File", 20);
    set_border(km_file_keys, 1, 0, 0);
    compact(km_file_keys);
    add_column(key_manager, km_file_keys);

    COLUMN* km_left_right = new_column("km_left_right", "", 4);
    lockColumn(km_left_right, 1, 0);
    add_column(key_manager, km_left_right);
    FIELD* km_right_btn = new_field(0, km_right_func, 0, "kmr_btn", ">>");
    add_field(km_left_right, km_right_btn, 10);
    FIELD* km_left_btn = new_field(0, km_left_func, 0, "kmr_btn", "<<");
    add_field(km_left_right, km_left_btn, 12);

    COLUMN* km_keys = new_column("km_keys", "Keys", 20);
    set_border(km_keys, 1, 0, 0);
    compact(km_keys);
    add_column(key_manager, km_keys);

    COLUMN* km_btns = new_column("km_btns", "", 8);
    add_column(key_manager, km_btns);
    FIELD* km_add = new_field(0, km_add_func, 0, "km_add", "Add");
    add_field(km_btns, km_add, -1);
    FIELD* km_remv = new_field(0, km_remv_func, 0, "km_remv", "Remove");
    add_field(km_btns, km_remv, -1);
    FIELD* km_edit = new_field(0, km_edit_func, 0, "km_edit", "Edit");
    add_field(km_btns, km_edit, -1);
    FIELD* km_load = new_field(0, km_load_func, 0, "km_load", "Load");
    add_field(km_btns, km_load, -1);
    FIELD* km_save = new_field(0, km_save_func, 0, "km_save", "Save");
    add_field(km_btns, km_save, -1);
    FIELD* km_back = new_field(0, km_back_func, 0, "km_back", "Back");
    add_field(km_btns, km_back, -1);

    // enc dec menu
    MENU* enc_dec = new_menu("enc_dec");

    COLUMN* ed_files = new_column("ed_files", "Files", 20);
    set_border(ed_files, 1, 0, 0);
    compact(ed_files);
    add_column(enc_dec, ed_files);

    COLUMN* ed_keys = new_column("ed_keys", "Keys", 20);
    set_border(ed_keys, 1, 0, 0);
    compact(ed_keys);
    add_column(enc_dec, ed_keys);

    COLUMN* ed_keys_aux = new_column("ed_keys_aux", "Choose Key", 20);
    set_border(ed_keys_aux, 1, 0, 0);
    compact(ed_keys_aux);

    COLUMN* ed_algs = new_column("ed_algs", "Algorithm", 20);
    set_border(ed_algs, 1, 0, 0);
    compact(ed_algs);
    add_column(enc_dec, ed_algs);

    COLUMN* ed_btns = new_column("ed_btns", "", 15);
    add_column(enc_dec,ed_btns);
    FIELD* ed_add = new_field(0, ed_add_func, 0, "ed_add", "Add");
    add_field(ed_btns, ed_add, -1);
    FIELD* ed_remv = new_field(0, ed_remv_func, 0, "ed_remv", "Remove");
    add_field(ed_btns, ed_remv, -1);
    FIELD* ed_addf = new_field(0, ed_addf_func, 0, "ed_addf", "Add from File");
    add_field(ed_btns, ed_addf, -1);
    FIELD* ed_back = new_field(0, ed_back_func, 0, "ed_back", "Back");
    add_field(ed_btns, ed_back, -1);
}

void key_manager_func() {
    next_menu(get_menu("key_manager"));
}

void enc_dec_func() {
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
}

void km_left_func() {
}

void km_add_func() {
}

void km_remv_func() {
}

void km_edit_func() {
}

void km_load_func() {
}

void km_save_func() {
}

void ed_add_func() {
}

void ed_remv_func() {
}

void ed_addf_func() {
}