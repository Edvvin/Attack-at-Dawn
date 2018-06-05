#include "test.h"


/*
unsigned char multAes(unsigned char a,unsigned char b)
{
    unsigned int resenje=0,mod=0x11b,mask=1<<7,clan=0,broj_pon;
    while(mask)
    {
        broj_pon=mask&b;
        if(broj_pon==0)
            clan=0;
        else clan=a;
        while(broj_pon>1)
        {
            clan*=2;
            if(clan&256)
                clan^=mod;
            broj_pon>>=1;
        }
        resenje^=clan;
        mask>>=1;
    }

    return (resenje);
}
MOZDA BUDE DOBRO MOZDA NE*/
unsigned char S_box[256]={99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22};






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
/*
int main()
{

    printf("greska");
    int i,j;
    unsigned char test[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};

    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
        printf("%d ",test[i][j]);
    int x,y,a=0x11b,b=19;
    putchar('\n');


    int proba[]={1,2,3,4,5};
    unsigned int word[60];
    unsigned char molim[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    
    unsigned char molim1[24]={0x8e,0x73,0xb0,0xf7,0xda,0x0e,0x64,0x52,0xc8,0x10,0xf3,0x2b
    ,0x80,0x90,0x79,0xe5,0x62,0xf8,0xea,0xd2,0x52,0x2c,0x6b,0x7b};
    
    
    unsigned char molim2[32]={0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b
    ,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81
    ,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};


    unsigned char r;
    unsigned char radi[16]={0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2
    ,0xe0,0x37,0x07,0x34};

    unsigned char output[16];
    unsigned int niz[4];
    unsigned char input[16]="ATTACK AT DAWN";
    unsigned char input2[16]={'T','w','o',' ','O','n','e',' ','N','i','n','e',' ','T','w','o'};
    unsigned char molim4[16]={'T','h','a','t','s',' ','m','y',' ','K','u',
    'n','g',' ','F','u'};
    KeyExpansion(4,molim4,word);
    for(i=0;i<44;i++)
    {
        printf("\n%2d.%8.x",i,word[i]);
    }
    Aes_Cipher(word,input2,output,10);
    putchar('\n');
     for(i=0;i<16;i++)
    {
        printf("%x ",output[i]);
    }


}
*/
