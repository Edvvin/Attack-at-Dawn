#pragma once


#ifndef BLOKJEDEFINISAN

#define BLOKJEDEFINISAN 420

typedef struct
{
    unsigned char a[8];
} blok;


#endif


void DES_encrypt_file(char*,blok);
void DES_decrypt_file(char*,blok);

void triple_DES_encrypt_file(char*,blok,blok);
void triple_DES_decrypt_file(char*,blok,blok);


blok nula();
blok ucitajblok();              //ucitava blok bit po bit, sa razmakom. Primer za kljuc - "0 0 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 0 0 1 0 0 1 0 0 0 1 1 0 0 1 0 1 1 1 0 1 0 1 0 0 1 1 0 1 1 0 1 0 0 0 0 1 1 0 1 0 1 1 1 0 0 1 1"
void ispisiblok(blok);

char* writePath(char*);             //writePath za sada samo nakaci "novi" pre ekstenzije
