#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "test.c"
#include "time.h"
#include "hash.h"
#include "hash.c"

unsigned char Inverse_S_box[256]={82,9,106,213,48,54,165,56,191,64,163,158,129,243,215,251,124,227,57,130,155,47,255,135,52,142,67,68,196,222,233,203,84,123,148,50,166,194,35,61,238,76,149,11,66,250,195,78,8,46,161,102,40,217,36,178,118,91,162,73,109,139,209,37,114,248,246,100,134,104,152,22,212,164,92,204,93,101,182,146,108,112,72,80,253,237,185,218,94,21,70,87,167,141,157,132,144,216,171,0,140,188,211,10,247,228,88,5,184,179,69,6,208,44,30,143,202,63,15,2,193,175,189,3,1,19,138,107,58,145,17,65,79,103,220,234,151,242,207,206,240,180,230,115,150,172,116,34,231,173,53,133,226,249,55,232,28,117,223,110,71,241,26,113,29,41,197,137,111,183,98,14,170,24,190,27,252,86,62,75,198,210,121,32,154,219,192,254,120,205,90,244,31,221,168,51,136,7,199,49,177,18,16,89,39,128,236,95,96,81,127,169,25,181,74,13,45,229,122,159,147,201,156,239,160,224,59,77,174,42,245,176,200,235,187,60,131,83,153,97,23,43,4,126,186,119,214,38,225,105,20,99,85,33,12,125};

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
        {
        	
            resenje[i]=mod_aes(mul_aes(state[0][j],mat[i][0]),0x11b)^mod_aes(mul_aes(state[1][j],mat[i][1]),0x11b)^mod_aes(mul_aes(state[2][j],mat[i][2]),0x11b)^mod_aes(mul_aes(state[3][j],mat[i][3]),0x11b);
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


int  Aes_Cipher_File(char *ime_fajla,char *key_name,int Nk)
{
	
	unsigned char*key,rec[16],output[16],ulaz_ispis[8*1024];
	char *ime_dest,ekstenzija[]="_aes.txt";
	int i=0,Nr; 
	while(ime_fajla[i]!='\0')i++;
	unsigned int broj_procitanih;
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
	memcpy(ime_dest,ime_fajla,i);
	memcpy(ime_dest+i,ekstenzija,strlen(ekstenzija)+1);

	
	FILE *ulaz=fopen(ime_fajla,"rb");
    FILE *izlaz=fopen(ime_dest,"w");
    fprintf(izlaz,ime_fajla);
    fprintf(izlaz,"\n");
    fseek(ulaz,0,SEEK_END);
    fprintf(izlaz,"%ld\n",ftell(ulaz));
    fseek(ulaz, 0, SEEK_SET);
    fclose(izlaz);  
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
    
    
    upisiHash(ime_dest,mojHash(ime_dest,0,key,Nk*4,Nk*4+1));
    free(ime_dest);
    free(key);
    return 0;
}


int  Aes_Decipher_File(char *ime_ciphera,char *key_name,int Nk)
{
	
	char *s,*pomocs,*ekstenzija,*ime_fajla=NULL;//s-ime fajla bez ekstenzije,pomocs-ako se promeni adresa imena
    unsigned char c,rec[16],Nr,ulaz_ispis[8*1024],*key,output[16];;
    int n=0,n_e=0,i,broj_procitanih;
    unsigned int *word;
    unsigned long long velicina_fajla=0,velicina_fajla_pad;
    
    key=(char*)calloc(4*Nk,sizeof(char));
    memcpy(key,key_name,strlen(key_name));
   
    long long hash_file=procitajHash(ime_ciphera),hash_kreiran=mojHash(ime_ciphera,1,key,Nk*4,Nk*4+1);
    

	if(hash_file!=hash_kreiran)
    {
    	return 1;
	}
    
    
	FILE *ulaz=fopen(ime_ciphera,"rb"),*izlaz;
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
    while((fread(&c,1,1,ulaz)) && c!='.')//Cita ime do tacke
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
    
    while((fread(&c,1,1,ulaz)) && c!='\n')//Cita od tacke za ekstenziju
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

    memcpy(ime_fajla,s,strlen(s)+1);
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


    izlaz=fopen(ime_fajla,"wb");  
    word=KeyExpansion(Nk,key,Nr);

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
    
    free(s);
    free(ekstenzija);
    free(ime_fajla);
    free(word);
    fclose(ulaz);
    fclose(izlaz);   
	return 0;
}

int main()
{
	char s[]="hollowknight.jpg";

Aes_Cipher_File("a (2).txt","Thats my Kung Fu",4);

    //DEKRIPCIJA POCETAK
 Aes_Decipher_File("a (2)_aes.txt","Thats my Kung Fu",4);
    
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
