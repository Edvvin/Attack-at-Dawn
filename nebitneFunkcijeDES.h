#define ovoJeNepotrebno 5318008

#ifndef

/**
  * @file
  * @author Andrija Kolic
  * @brief Sve funkcije koje se interno koriste u DES.c
  *
  * Sve funkcije ikad koriscene u DES.c, korisnik nema direktan prisup njima, ali ocigledno neko zeli da cita o njima.
  *
  */




/**
  * @brief DES uglavnom radi sa blokovima od 8 bajtova, vecina ovih funkcija koristi ovu strukturu
  *
  * U nekim slucajevima nece biti iskorisceni svi bajtovi, ali koriscenje jedne strukture uproscuje program i smanjuje broj funkcija.
  *
  */
typedef struct
{
    unsigned char a[8];
} blok;


/**
  *
  * @brief Vraca vrednost bloka ciji su svi elementi 0
  *
  */
blok nula();

/**
  *
  * @brief Vraca vrednost bloka cije vrednosti elemenata odgovaraju vrednostima niza charova c. Niz c je predvidjene duzine 8.
  *
  *
  */
blok konstruktor(char *c);


/**
  * @brief Postavlja vrednost odredjenog bita jednog bloka u zavisnosti od odredjenog bita drugog bloka
  * @param[in,out] x Pokazivac na blok ciji se bit postavlja
  * @param[in] b Blok cija se vrednost bita koristi
  * @param[in] xd Redni broj bita koji se postavlja
  * @param[in] bd Redni broj bita cija se vrednost koristi
  *
  * Dodeljuje vrednost bd-tog bita bloka b xd-tom bitu bloka x. Nulti bit bloka je najlevlji bit prvog char-a bloka. 63. (poslednji) bit je najdesnji bit osmog char-a bloka.
  *
  */
void setblokbit(blok* x,blok b,char xd,char bd);

/**
  *
  * @brief Permutuje bitove bloka x
  * @param[in] x Blok ciji se bitovi permutuju
  * @param[in] c Pravilo po kome se bitovi permutuju, dato u vidu niza char-ova
  * @param[in] l Duzina niza c
  *
  * Bitovi bloka x se permutuju tako sto na i-to mesto dolazi c[i]-ti bit. Vraca se vrednost dobijenog bita.
  *
  */
blok permute(blok x,char* c,char l);



/**
  * @brief Shiftuje char c ulevo za jedno ili dva mesta
  * @param[in] c Char koji se shiftuje levo
  * @param[in,out] prvi Vrednost bita koji se umece umesto najdesnjeg (2. sa desne strane) bita. Takodje vraca vrednost najlevljeg bita koji bi inace bio izgubljen.
  * @param[in,out] drugi Promenljiva se koristi samo ako se shiftuje ulevo za 2 mesta. U tom slucaju se umece umesto najdesnjeg bita i vraca vrednost 2. bita sa leve strane.
  * @param[in] br Broj mesta za koji se broj pomera ulevo. Predvidjene vrednosti su 1 ili 2.
  *
  *
  */
char shiftleft8(char c,char *prvi,char *drugi,char br)

/**
  *
  * @brief Vrsi pomeranje svih bitova unutar bajta ulevo
  * @param[in] b Blok koji se pomera ulevo
  * @param[in] br Broj mesta za koji se bitovi pomeraju. Predvidjene vrednostu su 1 ili 2.
  *
  * Koristi se samo prvih 7 bajtova datog bloka, tj. samo prvih 56 bitova. Ti bitovi se posmatraju kao 2 niza od 28 bitova. Bitovi ne prelaze iz jednog niza u drugi.
  * Pri pomeranju ulevo, najlevlji bit niza prelazi na najdesnju poziciju, dok se svi ostali samo pomere ulevo. Rezultat pomeranja ulevo za 2 mesta je isti kao rezultat 2
  * pomeranja ulevo za 1 mesto.
  *
  */
blok shiftleft28(blok b,char br);

/**
  * @brief Generise 16 48-bitnih subkljuceva od jednog 64-bitnog kljuca
  * @param[in] key Kljuc od koga se generisu subkljucevi
  *
  * Koristeci funkcije shiftleft28() i permute() generisu se subkljucevi po pravilima DES algoritma.
  *
  *
  */
blok* generatesubkeys(blok key);


/**
  * @brief Koristeci samo 6 bitova kao kljuc, dobija se 4-bitna vrednost
  * @param[in] b U ovom char-u se nalazi 6 bita koji se koriste kao kljuc
  * @param[in] d Redni broj tabelice koja se koristi
  *
  * Koristeci jednu od 8 predefinisanih tabela, dobija se vrednost u opsegu [0,15] pomocu kljuca b. Koristi se d-ta tabela, i koristi se samo 6 levih bitova char-a b. Najlevlji i
  * najdesnji bit formiraju broj u opsegu [0,3] i to odredjuje red tabele. Ostalih 4 bita formiraju broj u opsegu [0,15] i odredjuju kolonu tabele. Element u dobijenom redu i koloni
  * se vraca kao rezultat.
  *
  */
char replace6(unsigned char b,char d);

/**
  * @brief Koristeci 48 bitova datog bloka, dobija se vrednost od 32 bita
  * @param[in] b Blok ciji se bitovi koriste. Koriste se samo prvih 48 bitova.
  *
  * Uzimajuci 6 po 6 bitova iz bloka b, dobijaju se vrednosti od 4 bita pomocu funkcije replace6().
  * Dobijenih 32 bita se vracaju u vidu bloka.
  *
  *
  *
  */
blok replace48(blok b);



/**
  *
  * @brief Funkcija unutar DES algoritma. Koristi replace48() i permute()
  *
  */
blok DESf(blok r,blok k);

/**
  * @brief Funkcija koja vrsi odredjene operacije nad levom i desnom polovinom bloka kao celinama
  * @param[in] stari Blok cije se leva i desna polovina koriste
  * @param[in] k Kljuc koji se koristi pri operacijama
  *
  * U ovom delu DES algoritma se formira novi blok na osnovu starog po pravilu: leva polovina novog bloka je jednaka desnoj polovini starog;
  * desna polovina novog bloka je jednaka xor zbiru leve polovine starog i funkcije DESf() koja se poziva nad desnom polovinom starog bloka i nad kljucem.
  * Dobijeni novi blok se vraca.
  *
  */
blok LiRi(blok stari,blok k)


/**
  * @brief Srz DES algoritma, ova funkcija enkriptuje ili dekriptuje blok od 8 bajtova koristeci DES algoritam
  * @param[in] b Blok koji se enkriptuje ili dekriptuje
  * @param[in] keys Niz subkljuceva koji se koriste za enkripciju/dekripciju
  * @param[in] sifra Vrednost 0 znaci da se radi enkripcija, dok vrednost razlicita od 0 znaci da se vrsi dekripcija
  *
  * Ova funkcija uklapa sve prethodno definisane funkcije u poretku datom u DES algoritmu, i vraca novodobijeni blok - enkriptovanu/dekriptovanu verziju originala.
  * Zbog dobro definisanih funkcija, DES_blok() je vrlo proste gradje, poziva se permute() jednom, zatim se 16 puta poziva LiRi(), i finalno se, nakon par premestanja,
  * poziva permute() jos jednom. U zavisnosti od sifre kljucevi se koriste pocev od onog sa najmanjim indeksom (u slucaju enkripcije), ili pocev od onog sa najvecim
  * indeksom (u slucaju dekripcije).
  *
  */
blok DES_blok(blok b,blok *keys,char sifra);

/**
  * @brief Poziva DES_blok() sa vrednoscu argumenta sifra od 0
  *
  */
blok DES_encrypt_blok(blok b,blok *keys);

/**
  * @brief Poziva DES_blok() sa vrednoscu argumenta sifra od 1
  *
  */
blok DES_decrypt_blok(blok b,blok *keys)


/**
  *
  * @brief Funkcija koja u zavisnosti od zeljenog imena i destinacije, vraca semi-nasumicno dostupno ime
  * @param[in] path Zeljeno ime fajla
  * @param[in] dest Zeljena lokacija fajla
  *
  * Funkcija uklapa ime i lokaciju u jedan string i proverava dostupnost. U slucaju da zeljena kombinacija nije dostupna, nasumicno se dodaju cifre na ime.
  * Vraca se prva dostupna kombinacija dobijena ovim algoritmom.
  *
  */
char* writePath(char* path,char* dest);

/**
  *
  * @brief Najbitnija funkcija za rad sa fajlovima. Sve funkcije kojima korisnik ima pristup pozivaju DES_file(). Izvrsava enkripciju ili dekripciju celog fajla
  * @param[in] path Ime fajla nad kojim se vrsi enkripcija/dekripcija
  * @param[in,out] newpath Zeljeno ime rezultujuceg fajla. Moze biti izmenjeno
  * @param[in] keys Subkljucevi koji se koriste za enkripciju/dekripciju
  * @param[in] sifraIfajlImaHash Ima vrednost 1 ako se vrsi dekripcija i tada dati fajl uvek ima hash kod. Ima vrednost 0 ako se vrsi enkripcija, tada fajl ne bi trebao da ima hash kod
  * @param[in] dozvola Dozvola funkciji da menja ime rezultujuceg fajla (newpath)
  * @param[in] doRemovePadding Govori funkciji da li se treba ukloniti visak bajtova sa kraja fajla
  * @param[in] dest Lokacija rezultujuceg fajla
  *
  * Funkcija vrsi enkripciju ili dekripciju datog fajla zavisno od vrednosti sifraIfajlImaHash. Ako se radi o dekripciji, vrsi se citanje podataka iz datog fajla pomocu funkcije
  * procitajINFO() iz hash.h fajla. Pod uslovom da funkcija ima dozvolu dolazi do potencijalne promene imena rezultujuceg fajla. Dokle god je to moguce, iz datog fajla se ucitavaju
  * podaci u komadima od 64kB, a potom se nastavlja sa ucitavanjem bajt po bajt. Svaki bajt se nezavisno enkriptuje/dekriptuje pomocu DES_blok() i rezultat se upisuje u zeljeni fajl.
  * Ako je to trazeno, izostavljaju se bajtovi sa kraja koji su dodati pri enkripciji.
  *
  */
void DES_file(char* path,char** newpath,blok *keys,char sifraIfajlImaHash,char dozvola,char doRemovePadding,char *dest)

#endif
