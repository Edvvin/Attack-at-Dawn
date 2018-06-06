/**
 * @file aes.h
 */


#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

/**
  * @file
  
  * @brief Funkcije koriscenje u enkripciji i dekripciji koriscenja Aes standarda
  *
  * Ovaj header fajl sadrzi funkcije koriscenje prilikom enkripciji i dekripciji 
  * Aes fajlova kljucem duzina 128,192,256 bita duzine.
  * @author Jovan Spasojevic
  */
 
 /**
 * @brief Funkcija za kreiranje round blokova koji se koriste u Aes enkripciji i dekripciji
 * @param[in] Nk Predstavlja broj pocetnih reci duzine 4-bajta
 * @param[in] key Kljuc od koga se prave svi round blokovi duzine 4 reci
 * @param[in] Nr Broj  rundi koji se desavaju u Aes enkripciji i dekripciji
 *@param[out] word Niz reci svi duzine 4-bajta
 *
 * Funkcija na pocetku ucita <b>Nk</b> broj reci duzine 4-bajta iz <b>key</b> niza bajtova u niz <b>word</b>.
 * Onda na osnovu 4 poslednje reci iz niza <b>word</b> putem raznih transformacija napravi 4 nove reci koje dodaje u niz.
 * To radi dok niz nema (<b>Nr</b>+1)*4 reci u nizu. Na zavrsetku funkcije se vraca pokazivac na taj niz.
 */ 
unsigned int* KeyExpansion(int Nk,unsigned char *key,int Nr);
 /**
 * @brief Funkcija koja za svaki byte u <b>state</b> matrici zameni sa onom na koju pokazuje u tabeli S-box.
 * @param[in,out] state Matrica 4x4 koja sadrzi trenutno stanje bloka memorije koji se trenutno enkriptuje
*
 * Za svaki bajt u matrici <b>state</b> trazi se njegova vrednost u tabeli S-box(S_Box[state[i][j]) i onda vrednost
 * u matrici biva zamenjen vrednoscu u S-Box-u.
 */ 

int SubBytes(unsigned char  **state);
 /**
 * @brief Funkcija koja za svaki byte u reci od 4 bajta zameni sa onom koji se nalazu u tabeli S-box
 * @param[in] a Kopija reci duzine 4 bajta kojoj treba menjati vrednosti
* @param[out] resenje Nova kreirana rec koriscenjem <b>a</b>
*
 *Funkcija podeli rec <b>a</b> duzine 4-bajta na 4 dela. Onda se za svaki taj deo trazi njegova vrednost
 * u S-Box-u i onda sve sve te nove vrednosti spoje u novu rec <b>resenje</b>. Funkcija na kraju vraca <b>resenje</b> kao izlaz.
 */ 
 
 
unsigned int SubWord(unsigned int a);

 /**
 * @brief Funkcija koja vrsi pomeranje redova u <b>state</b> matrici 4x4.

 * @param[in,out] state Matrica 4x4 koja sadrzi trenutno stanje bloka memorije koji se trenutno enkriptuje
 *
 * Funkcija vrsi pomeranje redova tako sto pomeri tako sto za 2. red matrice sve njene clanove pomeri ulevo,tako da state[i][0]=state[i][1],state[i][1]=state[i][2],
 *state[i][2]=state[i][3] i state[i][3]=state[i][0]. 3. red se pomera 2 puta ulevo,4. red 3 puta ulevo na isti nacin.
 */ 
int ShiftRows(unsigned char** state);
 /**
 * @brief Funkcija koja vrsi razne promene nad kolonama <b>state</b> matrice.

 * @param[in,out] state Matrica 4x4 koja sadrzi trenutno stanje bloka memorije koji se trenutno enkriptuje
  *
 * Matrica <b>state</b> se podeli na 4 kolone. Svaka kolona ce se mnoziti sa 4x4 konstantnom matricom. Novim proizvodenim matricicama 4x1 tj. kolonama se dodaje konstantna kolona. 
 *Te nove kolone se posle spajaju i zamenjuju <b>state</b> matricu.
 
 */ 
int MixColumns(unsigned char** state);

 /**
 * @brief Funkcija koja vrsi inverzno pomeranje od funkcije<b>ShiftRows</b>.

 * @param[in,out] state Matrica 4x4 koja sadrzi trenutno stanje bloka memorije koji se trenutno dekriptuje
   *
 *Funkcija radi slicno kao <b>ShiftRows</b> samo sto se pomeranja desavaju udesno.
  * @see ShiftRows
 */ 
int InverseShiftRows(unsigned char ** state);
 /**
 * @brief Funkcija koja radi inverzno od funkcije <b>MixColumns</b>.


 * @param[in,out] state Matrica 4x4 koja sadrzi trenutno stanje bloka memorije koji se trenutno dekriptuje
   *
 * Radi slicno kao <b>MixColumns</b> samo sto se koriste druga konstantna matrica 4x4 i 4x1.
  *@see MixColumns
 */ 
int InverseMixColumns(unsigned char ** state);

 /**
 * @brief Funkcija koja radi inverzno od funkcije <b>SubBytes</b>.
 * @param[in,out] state Matrica 4x4 koja sadrzi trenutno stanje bloka memorije koji se trenutno dekriptuje
  *
 * Radi slicno kao <b>SubBytes</b> samo sto umesto S-box  tabele koristi Inverse S-box tabelu.
  *@see SubBytes
 */ 

int InverseSubBytes(unsigned char  **state);

 /**
 * @brief Xor-uje round blok(niz od 4 reci duzine 4-bajta) i <b>state</b> matricu
* @param[in] word Niz od 4 reci duzine 4-bajta koji pretstavlja jedan round blok
 * @param[in,out] state Matrica 4x4 koja sadrzi trenutno stanje bloka memorije koji se trenutno dekriptuje
  *
 *Funkcija XOR-uje svaku kolonu iz <b>state</b> matrice 4x4 sa reci i niza <b>word</b> (Column[i]=Column[i] Xor word[i]).
* Ova funkcija je sama sebi inverzna.
 */ 
int AddKey(unsigned char ** state,unsigned int* word);

 /**
 * @brief Funkcija proverava da li lokacija fajla koja je poslata postoji i ako postoji onda ga menja.
* @param[in] ime_fajla Lokacija fajla koji se proverava
 *
* Funkcija vraca Lokaciju fajla sa promenjenim imenom(ako fajl sa takvim imenom vec postoji),ili sa ne promenjenim.
 */ 

char *ime_provera(char * ime_fajla);

 /**
 * @brief Funkcija koji enkriptuje blok memorije duzine 128-bita
 * @param[in] input Blok memorije koji treba enkriptovati
 * @param[in,out] output Blok memorije koji je enkriptovan
 * @param[in] Nr Broj rundi koje rade Aes enkripcija i dekripcija 
* @param[in] word Niz reci od (<b>Nr</b>+1)*4 duzine
 *
* Funkcija koristi prethodne definisane funkcije <b>ShiftRows</b>,<b>MixColumns</b>,<b>AddKey</b>,
*<b>SubBytes</b> da vrsi razne transformacije <b>state</b> matrice kreirane deljenjem <b>input</b> niza 
* po kolonama(state[j][i]=word[i*4+j]). Na kraju se <b>state</b> matrica razdeli na kolone i sve spoje u niz <b>output</b>
*(output[i*4+j)=state[j][j]). Funkcija vraca pokazivac na taj niz.
 * @see ShiftRows
  * @see MixColumns
  * @see AddKey
  * @see SubBytes
 */ 
int  Aes_Cipher_Block(unsigned int * word,unsigned char*input ,unsigned char*output,int Nr);
 /**
 * @brief Funkcija koji dekriptuje blok memorije duzine 128-bita
 * @param[in] input Blok memorije koji je enkriptovan
 * @param[in,out] output Blok memorije koji je dekriptovan
 * @param[in] Nr Broj rundi koje rade Aes enkripcija i dekripcija 
* @param[in] word Niz reci od (<b>Nr</b>+1)*4 duzine
 *
* Funkcija koristi prethodne definisane funkcije <b>InverseShiftRows</b>,<b>InverseMixColumns</b>,<b>InverseAddKey</b>,
*<b>InverseSubBytes</b> da vrsi razne transformacije <b>state</b> matrice kreirane deljenjem <b>input</b> niza 
* po kolonama(state[j][i]=word[i*4+j]). Na kraju se <b>state</b> matrica razdeli na kolone i sve spoje u niz <b>output</b>
*(output[i*4+j)=state[j][j]). Funkcija vraca pokazivac na taj niz.
 * @see InverseShiftRows
  * @see InverseMixColumns
  * @see AddKey
  * @see InverseSubBytes
 */ 
int  Aes_Decipher_Block(unsigned int * word,unsigned char*input ,unsigned char*output,int Nr);
 /**
 * @brief Funkcija koja vrsi dekripciju fajla
 * @param[in] ime_ciphera Lokacija fajla koji treba da bude dekriptovan
 * @param[in] key_name Kljuc koji se koristi u dekripciji
 * @param[in] Nk Broj koji predstavlja duzinu reci u bajtovima podeljeni sa 4
 * @param[in] path Nova lokacija dekriptovanog fajla dobijenog ovom funkcijom(moze biti NULL)
 *
  *Pre pocetka citanje u blokovima, funkcija  na pocetku procita 8 bajta na kraju fajla <b>ime_ciphera</b> koji predstavljaju Mac koji se koristi za 
 *proveru da li je kljuc odgovarajuci. Ako jeste nastavi se dekripcija ako nije vrati izlaz kao 1.
 *Takodje cita pocetakak fajla gde se nalaze ime fajla u kome treba da se upisu dekriptovani podaci i kao njegova velicina.
 * Posle toga cita fajl na lokaciji <b>ime_ciphera </b> u blokovima od po 8kb i onda deli taj blok
 *  na delove od po 16 bajta i koristi <b>Aes_Decipher_Block</b> da dekriptira te blokove.
* Funkcija  ako je path NULL dekriptovan fajl stavlja gde je i sam program, a ako path postoji
* onda ga postavlja na tu lokaciju. Fajl nece vrsiti dekripciju ako je unet pogresan kljuc.
*Ime fajla i njegova lokacija se proveravaju pre dekripcije da se utvrdi da li postoji fajl sa istim imenom i menja svoje ime ako postoji.

 * @see Aes_Decipher_Block
  * @see ime_provera
 */ 
int  Aes_Decipher_File(char *ime_ciphera,char *key_name,int Nk,char*path);
 /**
 * @brief Funkcija koja vrsi enkripciju fajla
 * @param[in] ime_fajla Lokacija fajla koji treba da bude enkriptovan
 * @param[in] key_name Kljuc koji se koristi u enkripciji
 * @param[in] Nk Broj koji predstavlja duzinu bloka u bajtovima podeljeni sa 4
 * @param[in] path Nova lokacija enkriptovanog fajla dobijenog ovom funkcijom(moze biti NULL)
 *
 *Fajl na lokaciji <b>ime_fajla</b> pre citanja se trazi njegovo ime i velicina. Posto se nadju 
 *kreira se novi fajl koji od oblika imena file.extension pravi novi fajl file_aes.txt.
 *Proverava se ime kreiranog fajla u slucaju da vec postoji na lokaciji gde treba da se smesti i ako postoji menja svoje ime.
 *Potom se cita fajl na lokaciji <ime_fajla> u blokovima od po 8KB i onda se dele na delove od po 16 bajta koji se 
 *enkriptuju funkcijom <b>Aes_Cipher_Block</b> i upisuju u file_aes.txt.
 *Na kraju programa se pomocu funkcija iz <b>hash.h</b> dodaju 8 bajta na kraju fajla koji se koriste za identifikaciju kljuca.
* Funkcija  ako je path NULL enkriptovan fajl stavlja gde je i sam program, a ako path postoji
* onda ga postavlja na tu lokaciju. Funkcija takodje na pocetak fajla stavlja njegovo originalno ime bez lokacije i njegovu duzinu.
 */ 
int  Aes_Cipher_File(char *ime_fajla,char *key_name,int Nk,char*path);


#endif // TEST_H_INCLUDED

