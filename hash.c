#include <stdio.h>
#include <stdlib.h>


//hash je long long i nalazi se na kraju fajla

int stringHash(char *path,char fajlVecImaHash)
{
    int lengthOfCypherText/*=ftell(f)-fajlVecImaHash*(sizeof(long long))*/,startPosition=0,hashVal=0,i;
    char c=0;

        char *tmp=(char*)malloc(128*sizeof(char)),*tmp2=(char*)malloc(128*sizeof(char));
        procitajINFO(path,tmp,&lengthOfCypherText,0,tmp2,&startPosition);
        free(tmp);
        free(tmp2);

    FILE *f=fopen(path,"rb");

        fseek(f,0,SEEK_END);
        lengthOfCypherText=ftell(f)-fajlVecImaHash*sizeof(long long)-startPosition;

    //rewind(f);
	fseek(f,startPosition,SEEK_SET);
	//printf("startujem na: %d\n",startPosition);
	//printf("duzina je: %d\n",lengthOfCypherText);
	for (i=0;i<lengthOfCypherText;i++)
    {
        fread(&c,sizeof(char),1,f);
        //printf("znak je: %d\n",c);
		hashVal = (hashVal << 4) + c;
		int g = hashVal & 0xF0000000L;
		if (g != 0) hashVal ^= g >> 24;
		hashVal &= ~g;
	}
	//printf("hashVal = %d\n",hashVal);
	fclose(f);
	return hashVal;
}

long long mojHash(char *path,char fajlVecImaHash,char *kljuc,char duzinaKljuca,int metod)
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
    //printf("hes = %lld\n",hes);
    return(hes);
}

long long procitajHash(char *path)
{
    FILE *f=fopen(path,"rb");
    fseek(f,-sizeof(long long),SEEK_END);
    long long ll;
    fread(&ll,sizeof(long long),1,f);
    fclose(f);
    return(ll);
}

void upisiHash(char *path,long long hes)
{
    FILE *f=fopen(path,"ab");
    //fseek(f,0,SEEK_END);
    int succ=fwrite(&hes,sizeof(long long),1,f);
    //printf("succ = %d\n",succ);
    fclose(f);
}

int isGood(char *path,char *kljuc,char duzinaKljuca,int metod)
{
    long long target=procitajHash(path),pokusaj=mojHash(path,1,kljuc,duzinaKljuca,metod);
    if (target==pokusaj)
        return(1);
    return(0);
}

/*  nepotrebno?
void dodajInfo(char* path,char* newpath)
{
    FILE *f=fopen(path,"rb"),*newf=fopen(newpath,"wb");
    char len=strlen(path),i;
    fwrite(&len,sizeof(char),1,newf);
    for (i=0;i<len;i++)
        fwrite(path,sizeof(char),len,newf);
    fseek(f,0,SEEK_END);

}*/


int Dodaj_ime_i_velicinu(char *ime_fajla,char *ime_dest)
{
	int i=0;
	FILE *ulaz=fopen(ime_fajla,"rb"),*izlaz=fopen(ime_dest,"w");
	while(ime_fajla[i++]!='\0');
	while(i>=0 && ime_fajla[i]!='\\')i--;
	i++;
    fprintf(izlaz,ime_fajla+i);
    fprintf(izlaz,"\n");
    fseek(ulaz,0,SEEK_END);
    fprintf(izlaz,"%ld\n",ftell(ulaz));
    fseek(ulaz, 0, SEEK_SET);
    fclose(izlaz);  
	fclose(ulaz);	
}

void procitajINFO(char *path,char *ime,int *velicina,int imaIV,char *iv,int *pocetakFajla)
{
    FILE *f=fopen(path,"r");
    char i,*tmpchar=(char*)calloc(128,sizeof(char));
    //ime=(char*)calloc(128,sizeof(char));
    fscanf(f,"%s %s",ime,tmpchar);
    *velicina=atoi(tmpchar);
    *pocetakFajla=strlen(ime)+strlen(tmpchar)+4;
    if (imaIV)
    {
        //iv=(char*)calloc(8,sizeof(char));
        for (i=0;i<8;i++)
            fscanf(f,"%c",iv+i);
        *pocetakFajla+=8;
    }
    free(tmpchar);
    fclose(f);
}
