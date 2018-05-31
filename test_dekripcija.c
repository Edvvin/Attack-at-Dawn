#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "test.c"
#include "time.h"

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
        {
            resenje[i]=mul_grupa(state[0][j],mat[i][0])^mul_grupa(state[1][j],mat[i][1])^mul_grupa(state[2][j],mat[i][2])^mul_grupa(state[3][j],mat[i][3]);
        }
        for(i=0;i<4;i++)
        {
            state[i][j]=resenje[i];
        }
    }
    return 0;
}


int InverseSubBytes(unsigned char  **state)
{
    unsigned char medju_korak,mask,broj;
    int i,j,z,resenje;

    for(z=0;z<4;z++)
    {
        for(j=0;j<4;j++)
        {
        resenje=0;
         for(i=7;i>=0;i--)
            {
                mask=(1<<((i+2)%8))| (1<<((i+5)%8))|(1<<((i+7)%8));
                medju_korak=state[z][j] & mask;
                broj=0;
                while(medju_korak)
                {
                    if(medju_korak & 1)
                        broj^=1;
                    medju_korak>>=1;
                }
                resenje|=broj<<i;
            }
            state[z][j]=resenje^0x05;
            state[z][j]=inverse(state[z][j]);
        }
    }
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


int  Aes_Cipher_File(char *ime_fajla,char *key_name)
{
	
	unsigned char*key,*rec,output[16];
	char *ime_dest,ekstenzija[]="_aes.txt";
	int i=strlen(ime_fajla),Nk=strlen(key_name)/4,Nr;
	unsigned int *word;
	if(Nk==4)
    {
        Nr=10;
    }
    else if(Nk==6)
    {
        Nr=12;
    }
    else Nr=14;

	while(ime_fajla[i]!='.')i--;
	ime_dest=(char*)malloc((i+9)*sizeof(char));
	strncpy(ime_dest,ime_fajla,i);
	strcpy(ime_dest+i,ekstenzija);
	
	FILE *ulaz=fopen(ime_fajla,"rb");
    FILE *izlaz=fopen(ime_dest,"w");
    fprintf(izlaz,ime_fajla);
    fprintf(izlaz,"\n");
    fseek(ulaz,0,SEEK_END);
    fprintf(izlaz,"%ld\n",ftell(ulaz));
    fseek(ulaz, 0, SEEK_SET);
    fclose(izlaz);  
    izlaz=fopen(ime_dest,"ab");
   
    
    key=(char*)malloc(strlen(key_name)*sizeof(char));

    strncpy(key,key_name,strlen(key_name));
     
    word=KeyExpansion(Nk,key,Nr);
    rec=(unsigned char*)calloc(16,sizeof(unsigned char));
    while((fread(rec,1,16,ulaz)-16)==0)
    {
        Aes_Cipher_Block(word,rec,output,Nr);
        fwrite(output,1,16,izlaz);
        memset(rec,'\0',16);
    }
    if(rec[0]!=0)
    {
        Aes_Cipher_Block(word,rec,output,Nr);
        fwrite(output,1,16,izlaz);
    }
    
    free(rec);
    fclose(ulaz);
    fclose(izlaz);
    free(word);
    free(ime_dest);
    free(key);
}


int  Aes_Decipher_File(char *ime_ciphera,char *key_name)
{
	unsigned char*key,output[16];
	unsigned int *word;
	char *s,*pomocs,*ekstenzija,*ime_fajla=NULL;//s-ime fajla bez ekstenzije,pomocs-ako se promeni adresa imena
    unsigned char c,*rec=(unsigned char*)calloc(16,sizeof(unsigned char)),Nk=strlen(key_name)/4,Nr;
    int n=0,n_e=0;
    
	FILE *ulaz=fopen(ime_ciphera,"rb"),*izlaz;
	
	key=(char*)malloc(strlen(key_name)*sizeof(char));
    strncpy(key,key_name,strlen(key_name));
	if(Nk==4)
    {
        Nr=10;
    }
    else if(Nk==6)
    {
        Nr=12;
    }
    else Nr=14;
    
	// dobijanje imena fajla i ekstenzije
    while((fread(&c,1,1,ulaz)) && c!='.')
    {
        if(n==0)
            pomocs=(char*)malloc((++n)*(sizeof(char)));
        else pomocs=(char*)realloc(s,(++n)*sizeof(char));
        s=pomocs;
        s[n-1]=c;

    }
    pomocs=(char*)realloc(s,(++n)*sizeof(char));
    s=pomocs;
    s[n-1]='\0';

    ekstenzija=(char*)malloc((++n_e)*(sizeof(char)));
    ekstenzija[n_e-1]='.';
    while((fread(&c,1,1,ulaz)) && c!='\n')
    {
        if(c!='\r')
        {
            pomocs=(char*)realloc(ekstenzija,(++n_e)*sizeof(char));
            ekstenzija=pomocs;
            ekstenzija[n_e-1]=c;
        }
    }

    pomocs=(char*)realloc(ekstenzija,(++n_e)*sizeof(char));
    ekstenzija=pomocs;
    ekstenzija[n_e-1]='\0';	
    //pravljenje novog imena
    ime_fajla=(char*)malloc((strlen(s)+strlen(ekstenzija)+1)*sizeof(char));

    strcpy(ime_fajla,s);
    strcat(ime_fajla,ekstenzija);
    while(izlaz=fopen(ime_fajla,"r"))
    {


        s[n-1]=rand()%10+'0';
        free(ime_fajla);
        pomocs=(char*)realloc(s,(++n)*sizeof(char));
        s=pomocs;

        s[n-1]='\0';
        ime_fajla=(char*)malloc((strlen(s)+strlen(ekstenzija)+1)*sizeof(char));

        strcpy(ime_fajla,s);
        strcat(ime_fajla,ekstenzija);
        fclose(izlaz);
    }


    izlaz=fopen(ime_fajla,"wb");  
    word=KeyExpansion(Nk,key,Nr);
    while((fread(&c,1,1,ulaz)) && c!='\n');
    while((fread(rec,1,16,ulaz)-16)==0)
    {
    Aes_Decipher_Block(word,rec,output,Nr);
    fwrite(output,1,16,izlaz);
    memset(rec,'\0',16);
    }
    
    if(rec[0]!=0)
    {
        Aes_Decipher_Block(word,rec,output,Nr);
        fwrite(output,1,16,izlaz);
    }
    
    free(s);
    free(ekstenzija);
    free(ime_fajla);
    free(rec);
    free(word);
    fclose(ulaz);
    fclose(izlaz);   
}
int main()
{	
Aes_Cipher_File("hollowknight.jpg","Thats my Kung Fu");

    //DEKRIPCIJA POCETAK
Aes_Decipher_File("hollowknight_aes.txt","Thats my Kung Fu");
    
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
