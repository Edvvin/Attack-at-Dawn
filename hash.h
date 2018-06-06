#pragma once


/**
  * @file
  * @author Andrija Kolic feat Jovan Spasojevic
  * @brief Funkcije za rad sa hashom koji sluzi za proveru validnosti kljuca, kao i funkcije za upis trazenih informacija u fajl
  *
  * ovo ce opet da se pojavi kod autora, ne?
  * Kada korisnik nije siguran koji kljuc/metod je koristio pri enkripciji, dolazi do problema. Da bismo resili ovaj problem svaki enkriptovani fajl
  * ima kod od 8 bajtova na svom kraju. Taj kod je hash funkcija koja prima enkriptovani text, kljuc, kao i metod enkripcije. Zbog prirode hash funkcija
  * iz ovog koda se ne moze direktno izracunati korisceni kljuc, ali korisnik moze proveriti da li koristi ispravan kljuc.
  * Pored funkcija vezanih za hash, imamo i dve funkcije koje upisuju/citaju ime i velicinu originalnog fajla u enkriptovani.
  *
  */


/**
  * @brief Funkcija koja racuna hash za enkriptovani text
  * @param[in] path Ime enkriptovanog fajla za koji se racuna hash njegovog texta
  * @param[in] fajlVecImaHash Ima vrednost 1 ako prilozeni fajl vec ima upisan kod na kraju, vrednost 0 u suprotnom
  *
  * Funkcija koja racuna int vrednost u zavisnosti od enkriptovanog texta datog fajla. Ova vrednosti se kasnije pridruzuju vrednost kljuca i metoda
  * enkripcija i tako se formira hash kod. Funkcija koristi svaki bajt datog fajla, zato moze biti spora za vece fajlove. Da bismo ovu manu sto vise ublazili
  * koriscene su samo bitske operacije.
  *
  */
int stringHash(char *path,char fajlVecImaHash);


/**
  * @brief Funkcija koja racuna hash kod u zavisnosti od fajla, koriscenog kljuca, i metoda enkripcije
  * @param[in] path Ime enkriptovanog fajla za koji se racuna hash kod
  * @param[in] fajlVecImaHash Ima vrednost 1 ako prilozeni fajl vec ima upisan kod na kraju, vrednost 0 u suprotnom
  * @param[in] kljuc Kljuc koriscen pri enkripciji, ili kljuc za koji se proverava ispravnost
  * @param[in] duzinaKLjuca duzina kljuca (ko bi reko)
  * @param[in] metod Metod koriscen za enkripciju, svakom metodu se pridruzuje odredjen broj
  *
  * Funkcija koja racuna hash kod za dati fajl, kljuc i metod. Koristi se pri kreaciji enkriptovanog fajla, kao i pri dekripciji, u svrhu provere ispravnosti
  * odabranog kljuca/metoda dekripcije. Promenljiva metod ima sledece predvidjene vrednosti:
  * 17 = DES, 71 = triple-DES, 23 = AES 128b, 47 = AES 192b, 61 = AES 256b.
  *
  */
long long mojHash(char *path,char fajlVecImaHash,char *kljuc,char duzinaKljuca,int metod);/*    metod:
                                                                                                    17 = DES
                                                                                                    71 = tripleDES
                                                                                                    Jovan:
                                                                                                    23 = 128 bita
                                                                                                    47 = 192 bita
                                                                                                    61 = 256 bita
                                                                                          */
/**
  * @brief Funkcija koja cita hash kod iz fajla
  * @param[in] path Ime fajla iz koga se cita hash kod
  *
  * Funkcija se poziva za fajlove u koje je vec upisan hash kod, kada ga zelimo procitati da bismo mogli proveriti validnost kljuca/metoda dekripcije.
  * Vraca poslednjih 8 bajtova.
  *
  */
long long procitajHash(char *path);

/**
  * @brief Upisuje hash kod u fajl
  * @param[in] path Ime fajla u koji se upisuje kod
  * @param[in] hes Vrednost koja se upisuje na kraj fajla
  *
  * Funkcija otvara fajl metodom "ab" i upisuje vrednost hes na kraj.
  *
  */
void upisiHash(char *path,long long hes);

/**
  * @brief Funkcija proverava da li su dati kljuc i metod odgovarajuci za dati fajl
  * @param[in] path Ime fajla ciji se hash kod proverava
  * @param[in] kljuc Vrednost kljuca za koji se generise hash kod
  * @param[in] duzinaKLjuca duzina kljuca u bajtovima
  * @param[in] metod Pridruzena vrednost metoda (pogledati dokumentaciju funkcije mojHash)
  *
  * Funkcija uporedjuje vrednost hash koda upisanog u fajl sa imenom path sa vrednoscu funkcije mojHash(path,1,kljuc,duzinaKljuca,metod).
  * Vraca 1 ako su vrednosti iste, 0 u suprotnom.
  *
  */
int isGood(char *path,char *kljuc,char duzinaKljuca,int metod);

/**
  * @brief Funkcija kreira fajl na lokaciji <b>ime_dest</b> i dodaje ime i velicinu fajla sa lokacije <b>ime_fajla</b>
  * @param[in] ime_fajla Lokacija fajla cije se cita ime i velicina
  * @param[in] ime_dest Lokacija fajla koji se stvara i gde se velicina i ime proslog fajla upisuju u njega
  *Funkcija prvo kerira fajl na lokaciji <b>ime_dest</b>, a onda upisuje u njega samo ime fajla na lokaciji <ime_fajla> kao i njegovu velicinu.
  */
int Dodaj_ime_i_velicinu(char *ime_fajla,char *ime_dest);

/**
  * @brief Funkcija cita ime i velicinu originala iz enkriptovanog fajla
  * @param[in] path Ime enkriptovanog fajla iz koga se cita
  * @param[out] ime Ime originala koje se cita iz datog fajla
  * @param[out] velicina Velicina originalnog fajla, cita se iz datog fajla
  * @param[out] pocetakFajla Duzina informacija upisanih u enkriptovani fajl, koje se trebaju ignorisati pri dekripciji
  *
  * Funkcija otvara fajl sa imenom path, i iz njega ucitava dva stringa. Prvi string se direktno smesta u ime, a drugi se prvo pretvara u int,
  * a zatim svesta u velicina. Duzine dva stringa se sabiraja, i nakon uracunavanja dodatnih karaktera, dobijena vrednost se smesta u pocetakFajla,
  * oznacavajuci broj bajtova koji se preskace pri dekripciji.
  *
  */
void procitajINFO(char *path,char *ime,int *velicina,int *pocetakFajla);

