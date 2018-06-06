#include "keydecode.h"

int hexchar2int(char x){
    char str[2];
    str[0] = x;
    str[1] = '\0';
    int i;
    sscanf(str,"%x",&i);
    return i;
}

unsigned char* hex2key(char* key){
    int len = strlen(key);
    unsigned char* str = malloc(sizeof(unsigned char)*(len/2+2));
    int i;
    for(i = 0;i<len;i+=2){
        unsigned char c = 0;
        c+=hexchar2int(key[i]);
        c*=16;
        c+=hexchar2int(key[i+1]);
        str[i/2] = c;
    }
    str[i/2] = 0;
    return str;
}
