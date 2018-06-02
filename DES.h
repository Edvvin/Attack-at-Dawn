#pragma once


#ifndef BLOKJEDEFINISAN

#define BLOKJEDEFINISAN 420

typedef struct
{
    unsigned char a[8];
} blok;


#endif



int DES_encrypt_file(char* path,char *c);   //c je duzine 8
int DES_decrypt_file(char* path,char *c);   //c je duzine 8


int triple_DES_encrypt_file(char* path,char *c);   //c je duzine 16
int triple_DES_decrypt_file(char* path,char *c);   //c je duzine 16

