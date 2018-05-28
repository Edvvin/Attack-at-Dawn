#include <stdio.h>
#include <stdlib.h>


//hash je long long i nalazi se na kraju fajla

int stringHash(char *path,char fajlVecImaHash)//potrebno samo citanje iz fajla
{
    FILE *f=fopen(path,"rb");
    fseek(f,0,SEEK_END);
    int lengthOfCypherText=ftell(f)-fajlVecImaHash*(sizeof(long long)),hashVal=0,i;
    char c=0;
    rewind(f);
	for (i=0;i<lengthOfCypherText;i++)
    {
        fread(&c,sizeof(char),1,f);
        //printf("znak je: %d\n",c);
		hashVal = (hashVal << 4) + c;
		int g = hashVal & 0xF0000000L;
		if (g != 0) hashVal ^= g >> 24;
		hashVal &= ~g;
	}
	printf("hashVal = %d\n",hashVal);
	fclose(f);
	return hashVal;
}

long long mojHash(char *path,char fajlVecImaHash,char *kljuc,char duzinaKljuca,int metod)//treba se dogovoriti koji metod prosledjuje koju brojku
{
    long long hes=0;
    int i=0,j;
    int prosti[]={1297193,1141253,1009991,730909,712483,492967,363161,341839,222247,97771};
    hes+=stringHash(path,fajlVecImaHash)*prosti[0];
    hes+=metod*prosti[1];
    while (i<duzinaKljuca)
    {
        hes+=prosti[2+i/4]*((kljuc[i]<<24)+(kljuc[i+1]<<16)+(kljuc[i+2]<<8)+(kljuc[i+3]));
        //printf("hes je %lld\n",hes);
        //scanf("%d",&j);
        i+=4;
    }
    printf("hes = %lld\n",hes);
    return(hes);
}

long long procitajHash(char *path)//potrebno samo citanje iz fajla
{
    FILE *f=fopen(path,"rb");
    fseek(f,-sizeof(long long),SEEK_END);
    long long ll;
    fread(&ll,sizeof(long long),1,f);
    fclose(f);
    return(ll);
}

void upisiHash(char *path,long long hes)//potrebno samo pisanje u fajl
{
    FILE *f=fopen(path,"ab");
    //fseek(f,0,SEEK_END);
    int succ=fwrite(&hes,sizeof(long long),1,f);
    printf("succ = %d\n",succ);
    fclose(f);
}
