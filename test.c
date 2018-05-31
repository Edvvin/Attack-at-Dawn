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
unsigned char mul_grupa(unsigned char a,unsigned char b)
{   int mod=0x11b;
    return mul_aes(a,b)^mul_aes(div_aes(mul_aes(a,b),mod),mod);
}

int mul_aes(int a,int b)
{
    unsigned int resenje=0,mask=1<<13;
    while(mask)
    {
        resenje^=(mask&a)*b;
        mask>>=1;
    }
    return resenje;
}

int div_aes(int a,int b)
{
    unsigned int mask1=0x4000,mask2=0x4000,resenje=0;

    while(mask1 && !(mask1&a))
    {
        mask1>>=1;
    }
    while(mask2 && !(mask2&b))
    {
        mask2>>=1;
    }
    if(mask1<mask2)
        return 0;

    while(mask1>=mask2)
    {
        a^=mul_aes(b,mask1/mask2);
        resenje+=mask1/mask2;
     while(mask1 && !(mask1&a))
        {
        mask1>>=1;
        }
    }
    return resenje;
}





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
    unsigned char mat[4][4]={{2,3,1,1},{1,2,3,1},{1,1,2,3},{3,1,1,2}};

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
    free(resenje);
    return 0;
}



int SubBytes(unsigned char  **state)
{
    unsigned char medju_korak,mask,broj;
    int i,j,z,resenje;

    for(z=0;z<4;z++)
    {
        for(j=0;j<4;j++)
        {
        resenje=0;
         state[z][j]=inverse(state[z][j]);
         for(i=7;i>=0;i--)
            {
                mask=(1<<i)| (1<<((i+4)%8))| (1<<((i+5)%8))| (1<<((i+6)%8))|(1<<((i+7)%8));
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
            state[z][j]=resenje^0x63;
        }
    }
    return 0;
}


unsigned char inverse(unsigned char a)
{
    int b=283;
    int t=0,newt=1,r=b,newr=a,q,p;
    while(newr!=0)
    {
        q=div_aes(r,newr);
        p=newt;
        newt=t^mul_aes(q,p);
        t=p;
        p=newr;
        newr=r^mul_aes(q,p);
        r=p;
    }
    return mul_aes(1/r,t);
}


unsigned int *KeyExpansion(int Nk,unsigned char key[],int Nr)
{
	unsigned int * word;
    int i=0,j;
    unsigned int temp;
    unsigned char Rcon=1;

	
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
            temp=SubWord((temp>>24)^(temp<<8))^(Rcon<<24);
            Rcon=mul_grupa(Rcon,2);
        }
        else if(Nk>6 && (i% Nk)==4)
            temp=SubWord(temp);
        word[i]=word[i-Nk]^temp;
        i++;
    }
    return word;
}



unsigned int SubWord(unsigned int a)
{   unsigned char temp,pomoc,medju_korak,mask;
    unsigned int resenje=0;
    int i,j,broj;
    for(j=0;j<4;j++)
    {
        temp=a>>(j*8);
        temp=inverse(temp);
        pomoc=0;
        for(i=7;i>=0;i--)
        {
            mask=(1<<i)| (1<<((i+4)%8))| (1<<((i+5)%8))| (1<<((i+6)%8))|(1<<((i+7)%8));
            medju_korak=temp & mask;
            broj=0;
            while(medju_korak)
            {
                if(medju_korak & 1)
                    broj^=1;
                medju_korak>>=1;
            }
            pomoc|=broj<<i;
        }
        pomoc^=0x63;
        resenje^=pomoc<<(j*8);

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
