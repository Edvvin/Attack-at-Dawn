#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DES.h"



int main()
{




    int i;
    char* path=(char*)calloc(100,sizeof(char));
    blok x=nula(),k=nula();
    blok* ki;
    //printf("Unesi M:\n");
    //x=ucitajblok();

    printf("Uneti put do fajla: //pls nemoj da uneses nepostojeci fajl\n");
    scanf("%s",path);

    printf("%s\n",path);

    printf("Unesi K:\n");
    k=ucitajblok();

    DES_file(path,k,0);
    DES_file(writePath(path),k,1);

    return 0;
}
