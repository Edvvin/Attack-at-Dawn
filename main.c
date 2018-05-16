#include <stdio.h>
#include <stdlib.h>

#include "menus.h"

int smth(char* str){
    printf("%s",str);
    return 0;
}
int main(int argc, char** argv) {
    init_prog();
    MENU main_menu = new_menu("main_menu");
    COLUMN options = new_column("options");
    FIELD* btn_1 = new_field(0,0,"key_manager_btn","Key Manager");
    FIELD* btn_2 = new_field(0,0,"encryption_btn","Encryption");
    FIELD* btn_3 = new_field(0,0,"decryption_btn","Decryption");
    FIELD* btn_4 = new_field(0,0,"command_line_btn","Command Line");
    FIELD* btn_5 = new_field(0,0,"exit_btn","Exit",4);
    add_field(options,btn_1,-7);
    add_field(options,btn_2,-7);
    add_field(options,btn_3,-7);
    add_field(options,btn_4,-7);
    add_field(options,btn_5,-7);
    add_column(main_menu,options,20);
    run_prog(main_menu);
    end_prog();
    return (EXIT_SUCCESS);
}

