#include "DES.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"

char pc1[]={57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4},
    pc2[]={14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32},
    ip[]={58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7},
    e[]={32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1},
    p[]={16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25},
    ip1[]={40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25},
    shl[]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
char pc1len=56,pc2len=48,iplen=64,elen=48,plen=32,ip1len=64,shllen=16;

char S[][64]={{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
            {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
            {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
            {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
            {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
            {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
            {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
            {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}};

typedef struct
{
    unsigned char a[8];
} blok;


blok nula()
{
    blok x;
    int i;
    for (i=0;i<8;i++)
        x.a[i]=0;
    return(x);
}

blok konstruktor(char *c)
{
    blok x;
    int i;
    for (i=0;i<8;i++)
        x.a[i]=c[i];
    return(x);
}

void setblokbit(blok* x,blok b,char xd,char bd)
{
    unsigned char mask=255^1<<(7-(xd%8)),mm=b.a[bd/8]&(1<<(7-(bd%8)));
    x->a[xd/8]&=mask;
    if (mm)
        x->a[xd/8]|=1<<(7-(xd%8));
}

blok permute(blok x,char* c,char l)
{
    int i;
    blok novi=nula();
    for (i=0;i<l;i++)
        setblokbit(&novi,x,i,c[i]-1);
    return(novi);
}

char shiftleft8(char c,char *prvi,char *drugi,char br)
{
    char first=c&(1<<7),second=c&(1<<6);
    char nc=c<<br;
    if (br==1)
    {
        if (*prvi)
            nc|=1;
    }
    if (br==2)
    {
        if (*prvi)
            nc|=2;
        if (*drugi)
            nc|=1;
    }
    *prvi=first;
    *drugi=second;
    return(nc);
}

blok shiftleft28(blok b,char br)
{
    blok novi=nula();
    char i,leviprvi=b.a[0]&(1<<7),levidrugi=b.a[0]&(1<<6),desniprvi=b.a[3]&(1<<3),desnidrugi=b.a[3]&(1<<2),*prvi=(char*)calloc(1,sizeof(char)),*drugi=(char*)calloc(1,sizeof(char));
    for (i=6;i>=0;i--)
        novi.a[i]=shiftleft8(b.a[i],prvi,drugi,br);
    if (br==1)
    {
        novi.a[3]&=(255-(1<<4));
        if (leviprvi)
            novi.a[3]|=(1<<4);
        novi.a[6]&=(255-1);
        if (desniprvi)
            novi.a[6]|=1;
    }
    if (br==2)
    {
        novi.a[3]&=(255-(1<<5)-(1<<4));
        if (leviprvi)
            novi.a[3]|=(1<<5);
        if (levidrugi)
            novi.a[3]|=(1<<4);
        novi.a[6]&=(255-(1<<1)-1);
        if (desniprvi)
            novi.a[6]|=(1<<1);
        if (desnidrugi)
            novi.a[6]|=1;
    }
    return(novi);
}

blok* generatesubkeys(blok key)
{
   blok *b=(blok*)calloc(16,sizeof(blok));
   int i;
   b[0]=shiftleft28(key,shl[0]);
   for (i=1;i<16;i++)
   {
       b[i]=shiftleft28(b[i-1],shl[i]);
   }
   for (i=0;i<16;i++)
    b[i]=permute(b[i],pc2,pc2len);
   return(b);
}

char replace6(unsigned char b,char d)     //koriste se samo levih 6 bita char-a b
{
    unsigned char r=0,k=0;
    if (b&(1<<7))
        r+=2;
    if (b&(1<<2))
        r+=1;
    k=b<<1;
    k>>=4;
    return(S[d][r*16+k]);
}

blok replace48(blok b)
{
    blok novi=nula();
    int i,j,k=0;
    char c;
    for (i=0;i<8;i++)
    {
        c=b.a[k/8]<<(k%8);
        if (k%8>2)
        {
            c|=(b.a[(k/8)+1]>>(10-k%8))<<2;
        }
        k+=6;
        if (i%2==0)
            novi.a[i/2]=replace6(c,i)<<4;
        else
            novi.a[i/2]|=replace6(c,i);
    }
    return(novi);
}

blok DESf(blok r,blok k)
{
    int i;
    blok novi=nula();
    novi=permute(r,e,elen);
    for (i=0;i<8;i++)
        novi.a[i]^=k.a[i];
    novi=permute(replace48(novi),p,plen);
    return(novi);
}

blok LiRi(blok stari,blok k)
{
    blok novi=nula(),temp=nula();
    int i;
    for (i=0;i<4;i++)
    {
        novi.a[i]=stari.a[i+4];
        temp.a[i]=stari.a[i+4];
    }
    temp=DESf(temp,k);
    for (i=4;i<8;i++)
        novi.a[i]=stari.a[i-4]^temp.a[i-4];
    return(novi);
}


blok DES_blok(blok b,blok *keys,char sifra)             //sifra = 0 - enkripcija; sifra != 0 - dekripcija
{
    blok novi=permute(b,ip,iplen);
    char c,i,i0=0,i15=16,di=1;
    if (sifra)
    {
        i0=15;
        i15=-1;
        di=-1;
    }
    for (i=i0;i!=i15;i+=di)
        novi=LiRi(novi,keys[i]);
    for (i=0;i<4;i++)
    {
        c=novi.a[i];
        novi.a[i]=novi.a[i+4];
        novi.a[i+4]=c;
    }
    novi=permute(novi,ip1,ip1len);
    return(novi);
}

blok DES_encrypt_blok(blok b,blok *keys)
{
    return(DES_blok(b,keys,0));
}

blok DES_decrypt_blok(blok b,blok *keys)
{
    return(DES_blok(b,keys,1));
}

char* writePath(char* path,char* dest)//treba pozvati srand u mainu
{
    char *put=(char*)calloc(128,sizeof(char));
    int smanji=strlen(path)-1;
    while ((smanji>=0) && (path[smanji]!='\\'))
        smanji--;
    strcpy(put,path+smanji+1);
    int len=strlen(put);
    if (dest!=NULL)
    {
        int slanina=strlen(dest);
        if (dest[slanina-1]!='\\')
        {
            dest[slanina]='\\';
            dest[slanina+1]='\0';
        }
        len+=strlen(dest);
    }
    int i=len-1,dodatak,j=0,memind=1,ajnoresejnivde=0;
    char *newpath=(char*)calloc(len+5,sizeof(char)),*tmppath=(char*)calloc(len+5+5,sizeof(char));
    FILE *f;
    while (put[i]!='.')
        i--;
    if (dest)
    {
        strcpy(newpath,dest);
        strncat(newpath,put,i);
        ajnoresejnivde=strlen(dest);
    }
    else
        strncpy(newpath,put,i);
    strcpy(tmppath,newpath);
    strcat(tmppath,put+i);
    while (f=fopen(tmppath,"r"))
    {
        fclose(f);
        dodatak=rand()%10;
        newpath[ajnoresejnivde+i+j++]=dodatak+'0';
        strncpy(tmppath,newpath,ajnoresejnivde+i+j);
        tmppath[ajnoresejnivde+i+j]=0;
        if (newpath[len+memind*5-1]!=0)
        {
            newpath=realloc(newpath,len+(++memind)*5);
            tmppath=realloc(tmppath,len+(memind+1)*5);
        }
        strcat(tmppath,put+i);
    }
    newpath=realloc(newpath,len+(++memind)*5);
    strcat(newpath,put+i);
    //strcat(newpath,".txt");   mozda, mozda ne?
    return(newpath);
}


// nova verzije, iste brzine kao i stara?! nista nema smisla
void DES_file(char* path,char** newpath,blok *keys,char sifraIfajlImaHash,char dozvola,char doRemovePadding,char *dest)     //sifraIfajlImaHash = 0 - enkripcija; 1 - dekripcija
{
    //optimizeCode(fixBugs);
    int i,j,poc,removePadding=8,startPosition=0,textLength;
    long int OGlength;
    FILE *f,*newf;
    char c[64*1024];
    blok b=nula();
    f=fopen(path,"rb");

    if (sifraIfajlImaHash)
    {
        char *tmppath=(char*)calloc(128,sizeof(char));
        procitajINFO(path,dozvola?(*newpath):tmppath,&OGlength,&startPosition);
        if (dozvola)
            *newpath=writePath(*newpath,dest);
        free(tmppath);
        if (doRemovePadding)
            removePadding=OGlength%8;
    }

    fseek(f,0,SEEK_END);
    textLength=ftell(f)-sifraIfajlImaHash*sizeof(long long)-startPosition;
    fseek(f,startPosition,SEEK_SET);
    newf=fopen(*newpath,"ab+");
    if (!f || !newf)
    {
        //printf("Neuspesno otvaranje fajlova!\n");
        return(1);
    }
    i=0;
    int chunk=64*1024,komad;
    while (textLength)
    {
        komad=1;
        if (textLength>=chunk)
            komad=chunk;
        i+=fread(c+i,sizeof(char),komad,f);
        poc=0;
        while (poc<i && i>=8)
        {
            for (j=0;j<8;j++)
                b.a[j]=c[poc+j];
            b=DES_blok(b,keys,sifraIfajlImaHash);
            for (j=0;j<8;j++)
                c[poc+j]=b.a[j];
            poc+=8;
            b=nula();
        }
        if (i>=8)
        {
            fwrite(c,sizeof(char),textLength==komad?removePadding:(komad!=1?komad:8),newf);
            i=0;
        }
        textLength-=komad;
    }
    if (i>0)
    {
        b=nula();
        for (j=0;j<i;j++)
            b.a[j]=c[j];
        b=DES_blok(b,keys,sifraIfajlImaHash);
        for (j=0;j<8;j++)
            c[j]=b.a[j];
        fwrite(c,sizeof(char),removePadding,newf);
        i=0;
        b=nula();
    }
    fclose(f);
    fclose(newf);
}

int DES_encrypt_file(char* path,char *c,char *dest)
{
    FILE *f=fopen(path,"rb");
    if (!f)
        return(-1);
    fclose(f);

    long long hes;
    char *newpath=writePath(path,dest),i;

    Dodaj_ime_i_velicinu(path,newpath);

    blok k=konstruktor(c),*keys=generatesubkeys(k);
    DES_file(path,&newpath,keys,0,0,0,dest);


    hes=mojHash(newpath,0,c,8,17);
    upisiHash(newpath,hes);
    return(0);
}

int DES_decrypt_file(char* path,char *c,char *dest)//vraca 1 ako je kljuc ili metod pogresan
{
    char *newpath=(char*)calloc(128,sizeof(char)),i;
    FILE *f=fopen(path,"r");
    if (!f)
        return(-1);

    fclose(f);

    blok k=konstruktor(c),*keys=generatesubkeys(k);

    if (!isGood(path,c,8,17))
    {
        //printf("pogresno!\n");
        return(1);
    }
    DES_file(path,&newpath,keys,1,1,1,dest);
    return(0);
}

int triple_DES_encrypt_file(char* path,char *c,char *dest)
{
    long long carapa=5318008;
    FILE *f=fopen(path,"rb");
    if (!f)
        return(-1);
    fclose(f);
    long long hes;
    char *newpath=writePath(path,NULL),*tmppath,i;
    blok k1=konstruktor(c),k2=konstruktor(c+8),*keys1=generatesubkeys(k1),*keys2=generatesubkeys(k2);
    char *josjedanput=(char*)calloc(128,sizeof(char));


    Dodaj_ime_i_velicinu(path,newpath);
    DES_file(path,&newpath,keys1,0,0,0,NULL);
    upisiHash(newpath,carapa);


    tmppath=writePath(newpath,NULL);
    DES_file(newpath,&tmppath,keys2,1,0,0,NULL);


    josjedanput=writePath(tmppath,dest);
    Dodaj_ime_i_velicinu(path,josjedanput);
    DES_file(tmppath,&josjedanput,keys1,0,0,0,NULL);
    hes=mojHash(josjedanput,0,c,16,71);
    upisiHash(josjedanput,hes);

    remove(newpath);
    remove(tmppath);
    return(0);
}

int triple_DES_decrypt_file(char* path,char *c,char *dest)
{
    char *newpath=(char*)calloc(128,sizeof(char)),*tmppath,i,*majstor=(char*)calloc(10,sizeof(char));
    FILE *f=fopen(path,"r");
    long carapa=5318008;
    if (!f)
        return(-1);
    fclose(f);


    blok k1=konstruktor(c),k2=konstruktor(c+8),*keys1=generatesubkeys(k1),*keys2=generatesubkeys(k2);
    if (!isGood(path,c,16,71))
        return(1);
    char *josjedanput=(char*)calloc(128,sizeof(char));


    DES_file(path,&newpath,keys1,1,1,0,NULL);



    tmppath=writePath(newpath,NULL);
    int morase;
    procitajINFO(path,josjedanput,&carapa,&morase);
    f=fopen(tmppath,"w");
    fprintf(f,josjedanput);
    fprintf(f,"\n");
    fprintf(f,"%ld\n",carapa);
    fclose(f);
    long long dingDong=procitajHash(path);
    DES_file(newpath,&tmppath,keys2,0,0,0,NULL);
    upisiHash(tmppath,dingDong);


    josjedanput=writePath(josjedanput,dest);
    DES_file(tmppath,&josjedanput,keys1,1,0,1,NULL);


    remove(newpath);
    remove(tmppath);
    return(0);
}

//////////////////////////////////funkcije za testiranje, suvisne za krajnju verziju
/*
char charodbinarnog()
{
    char c=0;
    int i,x;
    for (i=0;i<8;i++)
    {
        scanf("%d",&x);
        if (x==1)
            c+=1<<(7-i);
    }
    return(c);
}

void ispisbinarni(char c)
{
    int i;
    for (i=7;i>=0;i--)
        if (c&1<<i)
            printf("1");
        else
            printf("0");
    printf(" ");
}

blok ucitajblok()
{
    int i,x;
    blok b;
    for (i=0;i<8;i++)
        b.a[i]=charodbinarnog();
    return(b);
}

void ispisiblok(blok b)
{
    int i;
    for (i=0;i<8;i++)
    {
        ispisbinarni(b.a[i]);
    }
    printf("\n");
    printf("\n");
}
*/

