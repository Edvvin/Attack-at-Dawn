#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

unsigned char inverse(unsigned char );
unsigned char mod_aes(int,int);
unsigned int mul_aes(int,int);
unsigned int*KeyExpansion(int ,unsigned char *,int);


int SubBytes(unsigned char  **);
unsigned int SubWord(unsigned int );
int ShiftRows(unsigned char**);
int MixColumns(unsigned char**);


int InverseShiftRows(unsigned char **);
int InverseMixColumns(unsigned char **);
int InverseSubBytes(unsigned char  **);
int AddKey(unsigned char **,unsigned int*);




int  Aes_Cipher_Block(unsigned int *,unsigned char* ,unsigned char*,int Nr);
int  Aes_Decipher_Block(unsigned int*,unsigned char *,unsigned char*,int Nr);
int  Aes_Decipher_File(char *,char *,int);
int  Aes_Cipher_File(char *,char *,int);
#endif // TEST_H_INCLUDED

