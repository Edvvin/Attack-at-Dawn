#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"
#include "time.h"
#include "hash.h"
#include "hash.c"
unsigned char Inverse_S_box[256]={82,9,106,213,48,54,165,56,191,64,163,158,129,243,215,251,124,227,57,130,155,47,255,135,52,142,67,68,196,222,233,203,84,123,148,50,166,194,35,61,238,76,149,11,66,250,195,78,8,46,161,102,40,217,36,178,118,91,162,73,109,139,209,37,114,248,246,100,134,104,152,22,212,164,92,204,93,101,182,146,108,112,72,80,253,237,185,218,94,21,70,87,167,141,157,132,144,216,171,0,140,188,211,10,247,228,88,5,184,179,69,6,208,44,30,143,202,63,15,2,193,175,189,3,1,19,138,107,58,145,17,65,79,103,220,234,151,242,207,206,240,180,230,115,150,172,116,34,231,173,53,133,226,249,55,232,28,117,223,110,71,241,26,113,29,41,197,137,111,183,98,14,170,24,190,27,252,86,62,75,198,210,121,32,154,219,192,254,120,205,90,244,31,221,168,51,136,7,199,49,177,18,16,89,39,128,236,95,96,81,127,169,25,181,74,13,45,229,122,159,147,201,156,239,160,224,59,77,174,42,245,176,200,235,187,60,131,83,153,97,23,43,4,126,186,119,214,38,225,105,20,99,85,33,12,125};

unsigned char S_box[256]={99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22};

unsigned char mul_9[256]={0x00,0x09,0x12,0x1b,0x24,0x2d,0x36,0x3f,0x48,0x41,0x5a,0x53,0x6c,0x65,0x7e,0x77,
0x90,0x99,0x82,0x8b,0xb4,0xbd,0xa6,0xaf,0xd8,0xd1,0xca,0xc3,0xfc,0xf5,0xee,0xe7,
0x3b,0x32,0x29,0x20,0x1f,0x16,0x0d,0x04,0x73,0x7a,0x61,0x68,0x57,0x5e,0x45,0x4c,
0xab,0xa2,0xb9,0xb0,0x8f,0x86,0x9d,0x94,0xe3,0xea,0xf1,0xf8,0xc7,0xce,0xd5,0xdc,
0x76,0x7f,0x64,0x6d,0x52,0x5b,0x40,0x49,0x3e,0x37,0x2c,0x25,0x1a,0x13,0x08,0x01,
0xe6,0xef,0xf4,0xfd,0xc2,0xcb,0xd0,0xd9,0xae,0xa7,0xbc,0xb5,0x8a,0x83,0x98,0x91,
0x4d,0x44,0x5f,0x56,0x69,0x60,0x7b,0x72,0x05,0x0c,0x17,0x1e,0x21,0x28,0x33,0x3a,
0xdd,0xd4,0xcf,0xc6,0xf9,0xf0,0xeb,0xe2,0x95,0x9c,0x87,0x8e,0xb1,0xb8,0xa3,0xaa,
0xec,0xe5,0xfe,0xf7,0xc8,0xc1,0xda,0xd3,0xa4,0xad,0xb6,0xbf,0x80,0x89,0x92,0x9b,
0x7c,0x75,0x6e,0x67,0x58,0x51,0x4a,0x43,0x34,0x3d,0x26,0x2f,0x10,0x19,0x02,0x0b,
0xd7,0xde,0xc5,0xcc,0xf3,0xfa,0xe1,0xe8,0x9f,0x96,0x8d,0x84,0xbb,0xb2,0xa9,0xa0,
0x47,0x4e,0x55,0x5c,0x63,0x6a,0x71,0x78,0x0f,0x06,0x1d,0x14,0x2b,0x22,0x39,0x30,
0x9a,0x93,0x88,0x81,0xbe,0xb7,0xac,0xa5,0xd2,0xdb,0xc0,0xc9,0xf6,0xff,0xe4,0xed,
0x0a,0x03,0x18,0x11,0x2e,0x27,0x3c,0x35,0x42,0x4b,0x50,0x59,0x66,0x6f,0x74,0x7d,
0xa1,0xa8,0xb3,0xba,0x85,0x8c,0x97,0x9e,0xe9,0xe0,0xfb,0xf2,0xcd,0xc4,0xdf,0xd6,
0x31,0x38,0x23,0x2a,0x15,0x1c,0x07,0x0e,0x79,0x70,0x6b,0x62,0x5d,0x54,0x4f,0x46};

unsigned char mul_b[256]={0x00,0x0b,0x16,0x1d,0x2c,0x27,0x3a,0x31,0x58,0x53,0x4e,0x45,0x74,0x7f,0x62,0x69,
0xb0,0xbb,0xa6,0xad,0x9c,0x97,0x8a,0x81,0xe8,0xe3,0xfe,0xf5,0xc4,0xcf,0xd2,0xd9,
0x7b,0x70,0x6d,0x66,0x57,0x5c,0x41,0x4a,0x23,0x28,0x35,0x3e,0x0f,0x04,0x19,0x12,
0xcb,0xc0,0xdd,0xd6,0xe7,0xec,0xf1,0xfa,0x93,0x98,0x85,0x8e,0xbf,0xb4,0xa9,0xa2,
0xf6,0xfd,0xe0,0xeb,0xda,0xd1,0xcc,0xc7,0xae,0xa5,0xb8,0xb3,0x82,0x89,0x94,0x9f,
0x46,0x4d,0x50,0x5b,0x6a,0x61,0x7c,0x77,0x1e,0x15,0x08,0x03,0x32,0x39,0x24,0x2f,
0x8d,0x86,0x9b,0x90,0xa1,0xaa,0xb7,0xbc,0xd5,0xde,0xc3,0xc8,0xf9,0xf2,0xef,0xe4,
0x3d,0x36,0x2b,0x20,0x11,0x1a,0x07,0x0c,0x65,0x6e,0x73,0x78,0x49,0x42,0x5f,0x54,
0xf7,0xfc,0xe1,0xea,0xdb,0xd0,0xcd,0xc6,0xaf,0xa4,0xb9,0xb2,0x83,0x88,0x95,0x9e,
0x47,0x4c,0x51,0x5a,0x6b,0x60,0x7d,0x76,0x1f,0x14,0x09,0x02,0x33,0x38,0x25,0x2e,
0x8c,0x87,0x9a,0x91,0xa0,0xab,0xb6,0xbd,0xd4,0xdf,0xc2,0xc9,0xf8,0xf3,0xee,0xe5,
0x3c,0x37,0x2a,0x21,0x10,0x1b,0x06,0x0d,0x64,0x6f,0x72,0x79,0x48,0x43,0x5e,0x55,
0x01,0x0a,0x17,0x1c,0x2d,0x26,0x3b,0x30,0x59,0x52,0x4f,0x44,0x75,0x7e,0x63,0x68,
0xb1,0xba,0xa7,0xac,0x9d,0x96,0x8b,0x80,0xe9,0xe2,0xff,0xf4,0xc5,0xce,0xd3,0xd8,
0x7a,0x71,0x6c,0x67,0x56,0x5d,0x40,0x4b,0x22,0x29,0x34,0x3f,0x0e,0x05,0x18,0x13,
0xca,0xc1,0xdc,0xd7,0xe6,0xed,0xf0,0xfb,0x92,0x99,0x84,0x8f,0xbe,0xb5,0xa8,0xa3};

unsigned char mul_d[256]={0x00,0x0d,0x1a,0x17,0x34,0x39,0x2e,0x23,0x68,0x65,0x72,0x7f,0x5c,0x51,0x46,0x4b,
0xd0,0xdd,0xca,0xc7,0xe4,0xe9,0xfe,0xf3,0xb8,0xb5,0xa2,0xaf,0x8c,0x81,0x96,0x9b,
0xbb,0xb6,0xa1,0xac,0x8f,0x82,0x95,0x98,0xd3,0xde,0xc9,0xc4,0xe7,0xea,0xfd,0xf0,
0x6b,0x66,0x71,0x7c,0x5f,0x52,0x45,0x48,0x03,0x0e,0x19,0x14,0x37,0x3a,0x2d,0x20,
0x6d,0x60,0x77,0x7a,0x59,0x54,0x43,0x4e,0x05,0x08,0x1f,0x12,0x31,0x3c,0x2b,0x26,
0xbd,0xb0,0xa7,0xaa,0x89,0x84,0x93,0x9e,0xd5,0xd8,0xcf,0xc2,0xe1,0xec,0xfb,0xf6,
0xd6,0xdb,0xcc,0xc1,0xe2,0xef,0xf8,0xf5,0xbe,0xb3,0xa4,0xa9,0x8a,0x87,0x90,0x9d,
0x06,0x0b,0x1c,0x11,0x32,0x3f,0x28,0x25,0x6e,0x63,0x74,0x79,0x5a,0x57,0x40,0x4d,
0xda,0xd7,0xc0,0xcd,0xee,0xe3,0xf4,0xf9,0xb2,0xbf,0xa8,0xa5,0x86,0x8b,0x9c,0x91,
0x0a,0x07,0x10,0x1d,0x3e,0x33,0x24,0x29,0x62,0x6f,0x78,0x75,0x56,0x5b,0x4c,0x41,
0x61,0x6c,0x7b,0x76,0x55,0x58,0x4f,0x42,0x09,0x04,0x13,0x1e,0x3d,0x30,0x27,0x2a,
0xb1,0xbc,0xab,0xa6,0x85,0x88,0x9f,0x92,0xd9,0xd4,0xc3,0xce,0xed,0xe0,0xf7,0xfa,
0xb7,0xba,0xad,0xa0,0x83,0x8e,0x99,0x94,0xdf,0xd2,0xc5,0xc8,0xeb,0xe6,0xf1,0xfc,
0x67,0x6a,0x7d,0x70,0x53,0x5e,0x49,0x44,0x0f,0x02,0x15,0x18,0x3b,0x36,0x21,0x2c,
0x0c,0x01,0x16,0x1b,0x38,0x35,0x22,0x2f,0x64,0x69,0x7e,0x73,0x50,0x5d,0x4a,0x47,
0xdc,0xd1,0xc6,0xcb,0xe8,0xe5,0xf2,0xff,0xb4,0xb9,0xae,0xa3,0x80,0x8d,0x9a,0x97};

unsigned char mul_e[256]={
0x00,0x0e,0x1c,0x12,0x38,0x36,0x24,0x2a,0x70,0x7e,0x6c,0x62,0x48,0x46,0x54,0x5a,
0xe0,0xee,0xfc,0xf2,0xd8,0xd6,0xc4,0xca,0x90,0x9e,0x8c,0x82,0xa8,0xa6,0xb4,0xba,
0xdb,0xd5,0xc7,0xc9,0xe3,0xed,0xff,0xf1,0xab,0xa5,0xb7,0xb9,0x93,0x9d,0x8f,0x81,
0x3b,0x35,0x27,0x29,0x03,0x0d,0x1f,0x11,0x4b,0x45,0x57,0x59,0x73,0x7d,0x6f,0x61,
0xad,0xa3,0xb1,0xbf,0x95,0x9b,0x89,0x87,0xdd,0xd3,0xc1,0xcf,0xe5,0xeb,0xf9,0xf7,
0x4d,0x43,0x51,0x5f,0x75,0x7b,0x69,0x67,0x3d,0x33,0x21,0x2f,0x05,0x0b,0x19,0x17,
0x76,0x78,0x6a,0x64,0x4e,0x40,0x52,0x5c,0x06,0x08,0x1a,0x14,0x3e,0x30,0x22,0x2c,
0x96,0x98,0x8a,0x84,0xae,0xa0,0xb2,0xbc,0xe6,0xe8,0xfa,0xf4,0xde,0xd0,0xc2,0xcc,
0x41,0x4f,0x5d,0x53,0x79,0x77,0x65,0x6b,0x31,0x3f,0x2d,0x23,0x09,0x07,0x15,0x1b,
0xa1,0xaf,0xbd,0xb3,0x99,0x97,0x85,0x8b,0xd1,0xdf,0xcd,0xc3,0xe9,0xe7,0xf5,0xfb,
0x9a,0x94,0x86,0x88,0xa2,0xac,0xbe,0xb0,0xea,0xe4,0xf6,0xf8,0xd2,0xdc,0xce,0xc0,
0x7a,0x74,0x66,0x68,0x42,0x4c,0x5e,0x50,0x0a,0x04,0x16,0x18,0x32,0x3c,0x2e,0x20,
0xec,0xe2,0xf0,0xfe,0xd4,0xda,0xc8,0xc6,0x9c,0x92,0x80,0x8e,0xa4,0xaa,0xb8,0xb6,
0x0c,0x02,0x10,0x1e,0x34,0x3a,0x28,0x26,0x7c,0x72,0x60,0x6e,0x44,0x4a,0x58,0x56,
0x37,0x39,0x2b,0x25,0x0f,0x01,0x13,0x1d,0x47,0x49,0x5b,0x55,0x7f,0x71,0x63,0x6d,
0xd7,0xd9,0xcb,0xc5,0xef,0xe1,0xf3,0xfd,0xa7,0xa9,0xbb,0xb5,0x9f,0x91,0x83,0x8d};
//aes enkripcija
int ShiftRows(unsigned char **state)
{
    unsigned char c;
    int i,j,z;
    for(i=0;i<3;i++)
    {
        for(j=i;j>=0;j--)
        {
            c=state[i+1][0];
           for(z=0;z<3;z++)
           {
            state[i+1][z]=state[i+1][z+1];
           }
           state[i+1][3]=c;
        }
    }
    return 0;
}


int MixColumns(unsigned char **state)
{
    int i,j;
    unsigned char *resenje=(unsigned char*)malloc(sizeof(unsigned char)*4);
    unsigned char a[4],b[4],h;
    for(j=0;j<4;j++)
    {
        for(i=0;i<4;i++)
        {
    		a[i]=state[i][j];
    		b[i]=state[i][j]<<1;
    		b[i]^=0x1b & (unsigned char)((signed char)state[i][j]>>7);
        }
		state[0][j]=b[0]^a[3]^a[2]^b[1]^a[1];
		state[1][j]=b[1]^a[0]^a[3]^b[2]^a[2];
		state[2][j]=b[2]^a[1]^a[0]^b[3]^a[3];
		state[3][j]=b[3]^a[2]^a[1]^b[0]^a[0];
    }
    free(resenje);
    return 0;
}



int SubBytes(unsigned char  **state)
{
    unsigned char medju_korak,mask,broj;
    int i,j,resenje;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			state[i][j]=S_box[state[i][j]];
    return 0;
}




unsigned int *KeyExpansion(int Nk,unsigned char key[],int Nr)
{
	unsigned int * word;
    int i=0,j;
    unsigned int temp;
    unsigned char Rcon[]={1,2,4,8,16,32,64,128,0x1b,0x36};

	
	word=(unsigned int*)malloc((Nr+1)*4*sizeof(unsigned int));
    while(i<Nk)
    {
        word[i]=(key[4*i]<<24)|(key[4*i+1]<<16)|(key[4*i+2]<<8)|(key[4*i+3]);
        i++;
    }
     
    while(i<4*(Nr+1))
    {
    	temp=word[i-1];
        if(i% Nk==0)
        {
            temp=SubWord((temp>>24)^(temp<<8))^(Rcon[i/Nk-1]<<24);
           
        }
        else if(Nk>6 && (i% Nk)==4)
            temp=SubWord(temp);
        word[i]=word[i-Nk]^temp;

        i++;
    }
    return word;
}



unsigned int SubWord(unsigned int a)
{   unsigned char temp;
    unsigned int resenje=0;
    int i,j,broj;
    for(j=0;j<4;j++)
    {
    	temp=S_box[(unsigned char)(a>>(j*8))];
        resenje^=temp<<(j*8);

    }

    return resenje;
}


int AddKey(unsigned char **state,unsigned int rec[])
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            state[j][i]^=rec[i]>>(8*(3-j));
        }
    }
    return 0;
}


int  Aes_Cipher_Block(unsigned int word[],unsigned char input[],unsigned char output[],int Nr)
{
    int i,j,z;
    unsigned char **state=(unsigned char**)malloc(sizeof(unsigned char*)*4);
    for(i=0;i<4;i++)
        state[i]=(unsigned char*)malloc(sizeof(unsigned char)*4);
    unsigned int *round_key=(unsigned int*)malloc(sizeof(unsigned int)*4);

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            state[j][i]=input[i*4+j];
        }
        round_key[i]=word[i];
    }
    AddKey(state,round_key);

    for(i=1;i<Nr;i++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        for(j=0;j<4;j++)
        {
            round_key[j]=word[i*4+j];
        }
        AddKey(state,round_key);
    }

    for(j=0;j<4;j++)
    {
        round_key[j]=word[(Nr)*4+j];
    }
    SubBytes(state);

    ShiftRows(state);
    AddKey(state,round_key);
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            output[4*i+j]=state[j][i];
        }
    }
    free(round_key);
    for(i=0;i<4;i++)
        free(state[i]);
    free(state);
    return 0;
}
//Pocinje dekripcija
unsigned int mul_aes(int a,int b)
{
    unsigned int resenje=0,mask=1<<13;
    while(mask)
    {
        resenje^=(mask&a)*b;
        mask>>=1;
    }
    return resenje;
}

unsigned char mod_aes(int a,int b)
{
    unsigned int mask1=0x4000,mask2=0x100,resenje=0;
    while(mask1 && !(mask1&a))
    {
        mask1>>=1;
    }
    if(mask1<mask2)
        return (unsigned char)a;


    while(mask1>=mask2)
    {
    	if((mask1&a))
        a^=mul_aes(b,mask1/mask2);
        mask1>>=1;
    }
    return (unsigned char)a;
}

int InverseShiftRows(unsigned char **state)
{
    unsigned char c;
    int i,j,z;
    for(i=0;i<3;i++)
    {
        for(j=i;j>=0;j--)
        {
            c=state[i+1][3];
           for(z=3;z>0;z--)
           {
            state[i+1][z]=state[i+1][z-1];
           }
           state[i+1][0]=c;
        }
    }
    return 0;
}



int InverseMixColumns(unsigned char **state)
{
    unsigned char mat[4][4]={{0x0e,0x0b,0x0d,0x09},{0x09,0x0e,0x0b,0x0d},{0x0d,0x09,0x0e,0x0b},{0x0b,0x0d,0x09,0x0e}},resenje[4];
    int i,j;
    for(j=0;j<4;j++)
    {	
    	for(i=0;i<4;i++)
        resenje[i]=mul_e[state[i][j]]^mul_b[state[(i+1)%4][j]]^mul_d[state[(i+2)%4][j]]^mul_9[state[(i+3)%4][j]];
        for(i=0;i<4;i++)
        {
            state[i][j]=resenje[i];
        }
    }
    return 0;
}


int InverseSubBytes(unsigned char  **state)
{
    int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			state[i][j]=Inverse_S_box[state[i][j]];
    return 0;

}


int  Aes_Decipher_Block(unsigned int word[],unsigned char input[],unsigned char output[],int Nr)
{
    int i,j,z;
    unsigned char **state=(unsigned char**)malloc(sizeof(unsigned char*)*4);
    for(i=0;i<4;i++)
        state[i]=(unsigned char*)malloc(sizeof(unsigned char)*4);
    unsigned int *round_key=(unsigned int*)malloc(sizeof(unsigned int)*4);
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            state[j][i]=input[i*4+j];
        }
        round_key[i]=word[Nr*4+i];
    }
    AddKey(state,round_key);

    for(i=Nr-1;i>0;i--)
    {
        InverseShiftRows(state);
        InverseSubBytes(state);
        for(j=0;j<4;j++)
        {
            round_key[j]=word[i*4+j];
        }
        AddKey(state,round_key);
        InverseMixColumns(state);
    }

    for(j=0;j<4;j++)
    {
        round_key[j]=word[j];
    }
    InverseShiftRows(state);
    InverseSubBytes(state);
    AddKey(state,round_key);
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            output[4*i+j]=state[j][i];
        }
    }

    free(round_key);
    for(i=0;i<4;i++)
        free(state[i]);
    free(state);
    return 0;
}
	// dobijanje imena fajla i ekstenzije
char *ime_provera(char *s)
{
 	int n=strlen(s)+1,n_e=0;
	unsigned char c;
	char *pomocs,*ekstenzija,*ime_fajla;
    FILE *izlaz;
	while(s[n]!='.')
	{
		n--;
		n_e++;
	}
	n_e+=1;
    ekstenzija=(char*)malloc((n_e)*(sizeof(char)));
    memcpy(ekstenzija,s+n,n_e-1);
    ekstenzija[n_e-1]='\0';
    pomocs=realloc(s,++n);
    s=pomocs;
    s[n-1]='\0';
    
    //pravljenje novog imena
    ime_fajla=(char*)malloc((n+n_e-1)*sizeof(char));

    memcpy(ime_fajla,s,n);
    strcat(ime_fajla,ekstenzija);
    while(izlaz=fopen(ime_fajla,"r"))
    {


        s[n-1]=rand()%10+'0';
        free(ime_fajla);
        pomocs=(char*)realloc(s,(++n)*sizeof(char));
        s=pomocs;

        s[n-1]='\0';
        ime_fajla=(char*)malloc((strlen(s)+strlen(ekstenzija)+1)*sizeof(char));

        memcpy(ime_fajla,s,strlen(s)+1);
        strcat(ime_fajla,ekstenzija);
        fclose(izlaz);
    }	
    free(s);
    free(ekstenzija);
	return ime_fajla ;
}
int  Aes_Cipher_File(char *ime_fajla,char *key_name,int Nk,char *path)
{
	
	unsigned char*key,rec[16],output[16],ulaz_ispis[8*1024];
	char *ime_dest,ekstenzija[]="_aes.txt";
	int i=0,Nr,velicina_path=0,mod; 
	while(ime_fajla[i]!='\0')i++;
	unsigned int broj_procitanih;
	unsigned int *word;
	if(Nk==4)
    {
        Nr=10;
        mod=23;
    }
    else if(Nk==6)
    {
        Nr=12;
        mod=47;
    }
    else 
    {
    	Nr=14;
    	mod=61;
	}
	while(ime_fajla[i]!='.')i--;
	
	if(path!=NULL)
	{
		while(path[velicina_path++]!='\0');
		velicina_path+=1;
	}
	
	
	ime_dest=(char*)malloc((i+9+velicina_path)*sizeof(char));
	if(path!=NULL)
	{
		memcpy(ime_dest,path,velicina_path-2);
		memcpy(ime_dest+velicina_path-2,"\\\\",2);
	}
	memcpy(ime_dest+velicina_path,ime_fajla,i);
	memcpy(ime_dest+i+velicina_path,ekstenzija,strlen(ekstenzija)+1);
			
	ime_dest=ime_provera(ime_dest);
	
	Dodaj_ime_i_veliinu(ime_fajla,ime_dest);
	
	FILE *ulaz=fopen(ime_fajla,"rb"),*izlaz;
    izlaz=fopen(ime_dest,"ab");
   
    
    key=(char*)calloc(4*Nk,sizeof(char));

    memcpy(key,key_name,strlen(key_name));
    word=KeyExpansion(Nk,key,Nr);
    i=0;
    broj_procitanih=fread(ulaz_ispis,1,8*1024,ulaz);
    while(broj_procitanih!=0)
    {	
		while(broj_procitanih-i>16)
	    {
	    	memcpy(rec,ulaz_ispis+i,16);
			i+=16;
			Aes_Cipher_Block(word,rec,output,Nr);
        	fwrite(output,1,16,izlaz);			
		}
		if((broj_procitanih-i)!=0)
		{
			memset(rec,'\0',16);
			memcpy(rec,ulaz_ispis+i,broj_procitanih-i);
        	Aes_Cipher_Block(word,rec,output,Nr);
        	fwrite(output,1,16,izlaz);				
			i=8*1024;
			
		}
		
		if(i==8*1024)
		{
			i=0;
			broj_procitanih=fread(ulaz_ispis,1,8*1024,ulaz);
		}
    }
    
    fclose(ulaz);
    fclose(izlaz);
    free(word);
    
    
    upisiHash(ime_dest,mojHash(ime_dest,0,key,Nk*4,mod));
    free(ime_dest);
    free(key);
    return 0;
}


int  Aes_Decipher_File(char *ime_ciphera,char *key_name,int Nk,char *path)
{
	
	char *pomocs,*ime_fajla=NULL;//s-ime fajla bez ekstenzije,pomocs-ako se promeni adresa imena
    unsigned char c,rec[16],Nr,ulaz_ispis[8*1024],*key,output[16];;
    int n=0,i,velicina_path=0,mod;
    unsigned int *word;
    unsigned long long velicina_fajla=0,velicina_fajla_pad,broj_procitanih;
    
    key=(char*)calloc(4*Nk,sizeof(char));
    memcpy(key,key_name,strlen(key_name));
    if(Nk==4)
    {
        Nr=10;
        mod=23;
    }
    else if(Nk==6)
    {
        Nr=12;
        mod=47;
    }
    else 
    {
		Nr=14;
    	mod=61;    	
	}
    long long hash_file=procitajHash(ime_ciphera),hash_kreiran=mojHash(ime_ciphera,1,key,Nk*4,mod);
	if(hash_file!=hash_kreiran)
    {
    	return 1;
	}   
	 
	FILE *ulaz=fopen(ime_ciphera,"rb"),*izlaz;
    
	if(path!=NULL)
	{
		while(path[velicina_path++]!='\0');
		velicina_path+=1;
		n=velicina_path;
		pomocs=(char*)malloc((n)*(sizeof(char)));
		ime_fajla=pomocs;
		memcpy(ime_fajla,path,velicina_path-2);
		memcpy(ime_fajla+velicina_path-2,"\\\\",2);		
	}    
	
    while((fread(&c,1,1,ulaz)) && c!='\n')
    {
        if(c!='\r')
		{
		if(n==0)
            pomocs=(char*)malloc((++n)*(sizeof(char)));
        else pomocs=(char*)realloc(ime_fajla,(++n)*sizeof(char));
        ime_fajla=pomocs;
        ime_fajla[n-1]=c;			
		}
    }
	pomocs=(char*)realloc(ime_fajla,(++n)*sizeof(char));
    ime_fajla=pomocs;
    ime_fajla[n-1]='\0';
	
	
    ime_fajla=ime_provera(ime_fajla);
    izlaz=fopen(ime_fajla,"wb");  
    word=KeyExpansion(Nk,key,Nr);

//Trazenje velicine_fajla
    while((fread(&c,1,1,ulaz)) && c!='\n')
    {

    	if(c!='\r')
    	{
    		velicina_fajla=velicina_fajla*10 + c-'0';
    		
		}
	}
	
   	velicina_fajla_pad=(velicina_fajla/16 +(velicina_fajla%16!=0))*16;
    while(velicina_fajla!=0)
    {	
    	i=0;

    	broj_procitanih=fread(ulaz_ispis,1,8*1024,ulaz);
    	if(velicina_fajla_pad>broj_procitanih)
	    {
	    	velicina_fajla-=broj_procitanih;
	    	velicina_fajla_pad-=broj_procitanih;
		}
    	else 
		{
			broj_procitanih=velicina_fajla_pad; 
		}  
    
		while(broj_procitanih-i>=16)
	    {
	    	memcpy(rec,ulaz_ispis+i,16);
   			Aes_Decipher_Block(word,rec,output,Nr);
   			if((broj_procitanih-i)==16 && velicina_fajla_pad==broj_procitanih)
   			{
   			fwrite(output,1,16-(velicina_fajla_pad-velicina_fajla),izlaz);	
   			velicina_fajla=0;
			}
			else
        	fwrite(output,1,16,izlaz);		
			i+=16;	
		}
	
    }
    
    free(ime_fajla);
    free(word);
    fclose(ulaz);
    fclose(izlaz);   
	return 0;
}
int main()
{
	Aes_Cipher_File("slika.jpg","Thats my Kung Fu",4,"C:\\Users\\jovan98\\Desktop\\PROJEKAT PP2");
	Aes_Decipher_File("C:\\Users\\jovan98\\Desktop\\PROJEKAT PP2\\slika_aes174094.txt","Thats my Kung Fu",4,"C:\\Users\\jovan98\\Desktop\\PROJEKAT PP2");
}

/*
    unsigned char test[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    int i,j;
    unsigned int word[60];
    unsigned char output[16];
    unsigned char input1[16]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,
    0xbb,0xcc,0xdd,0xee,0xff};
    unsigned char input2[16]={'T','w','o',' ','O','n','e',' ','N','i','n','e',' ','T','w','o'};
    unsigned char molim4[16]={'T','h','a','t','s',' ','m','y',' ','K','u',
    'n','g',' ','F','u'};
    unsigned char molim5[32]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
    0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
    KeyExpansion(8,molim5,word);
    Aes_Cipher(word,input1,output,14);
    for(i=0;i<16;i++)
        printf("%x ",output[i]);
    putchar('\n');
    Aes_Decipher(word,output,input1,14);
    for(i=0;i<16;i++)
        printf("%0.2x ",input1[i]);
*/
