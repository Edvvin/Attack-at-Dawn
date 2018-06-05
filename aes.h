/**
 * @file aes.h
 *
 * Interfaces to Data Buffer.
 *
 * This header define the interfaces of general purpose dynamic data buffer that
 * implemented by Equinox.
 */
/**\include aes.c
* dunno
*what im doing
*/

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

    //! A constructor.
    /*!
      this here is
	  dsomsfddsosdsd
	  sdfsf
	  sfds.
    */
unsigned char inverse(unsigned char );
unsigned char mod_aes(int,int);
    //! A pure virtual member.
    /*!
      \sa testMe()
      \param c1 the first argument.
      \param c2 the second argument.
    */
unsigned int mul_aes(int s,int d);
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

int var;/*ovo je test*/
#endif // TEST_H_INCLUDED

