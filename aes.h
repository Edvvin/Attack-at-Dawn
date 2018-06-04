#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED


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



char *ime_provera(char *);
int  Aes_Cipher_Block(unsigned int *,unsigned char* ,unsigned char*,int Nr);
int  Aes_Decipher_Block(unsigned int*,unsigned char *,unsigned char*,int Nr);
int  Aes_Decipher_File(char *,char *,int,char*);
int  Aes_Cipher_File(char *,char *,int,char*);
#endif // TEST_H_INCLUDED

