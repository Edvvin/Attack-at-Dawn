#pragma once



/**
  * @file
  * @author Andrija Kolic
  * @brief Funkcije za enkripciju i dekripciju fajlova DES ili triple-DES algoritmom.
  *
  * Header fajl koji sadrzi sve funkcije koje korisnik moze da pozove koristeci interfejs ili konzolu.
  * Sadrzi funkcije za enkripciju i dekripciju fajla koristeci DES algoritam, kao i funkcije za
  * enkripciju i dekripciju fajla koristeci triple-DES algoritam.
  *
  */

/**
 * @brief Funkcija za enkripciju vise fajlova jednim kljucem
 * @param[in] pKeyHead Lista kljuceva koje ce funkcija koristiti
 * @param[in,out] pFileHead Fajlovi koje ce funkcija da enkriptuje
 *
 * Nakon izvrsenja funkcije, u strukturi Cvorf ce se nalaziti novo ime fajla kao
 * i status enkripcije.
 */


/**
  * @brief Funkcija za enkripciju fajla koristeci DES algoritam
  * @param[in] path Ime fajla koji se enkriptuje
  * @param[in] c Kljuc koji se koristi za enkripciju, prilozen u vidu niza char-ova duzine 8
  * @param[in] dest Destinacija enkriptovanog fajla, rezultujuci fajl ce biti kreiran na toj lokaciji
  *
  * Funkcija enkriptuje fajl sa imenom 'path' i ne menja dati string. Koristi kljuc koji ucitava iz stringa c,
  * ne menjajuci njegov sadrzaj. Rezultujuci fajl ce se nalaziti na lokaciji 'dest' i imace ime originala, mozda
  * neznatno izmenjeno. Funkcija vraca 0 ako je enkriptovanje uspesno izvrseno, a vraca -1 ako otvaranje fajla nije uspelo.
  *
  */
int DES_encrypt_file(char* path,char *c,char *dest);   //c je duzine 8

/**
  * @brief Funkcija za dekripciju fajla koristeci DES algoritam
  * @param[in] path Ime enkriptovanog fajla
  * @param[in] c Kljuc koji se koristi za dekripciju, prilozen u vidu niza char-ova duzine 8
  * @param[in] dest Destinacija dekriptovanog fajla, rezultujuci fajl ce biti kreiran na toj lokaciji
  *
  * Funkcija dekriptuje fajl sa imenom 'path' i ne menja dati string. Koristi kljuc koji ucitava iz stringa c,
  * ne menjajuci njegov sadrzaj. Rezultujuci fajl ce se nalaziti na lokaciji 'dest' i imace ime originala, koje je
  * upisano u enkriptovani fajl, mozda neznatno izmenjeno. Funkcija vraca 0 ako je enkriptovanje uspesno izvrseno,
  * vraca -1 ako otvaranje fajla nije uspelo, a vraca 1 ako prilozeni kljuc nije odgovarajuci.
  *
  */
int DES_decrypt_file(char* path,char *c,char *dest);   //c je duzine 8

/**
  * @brief Funkcija za enkripciju fajla koristeci tripe-DES algoritam
  * @param[in] path Ime fajla koji se enkriptuje
  * @param[in] c Kljuc koji se koristi za enkripciju, prilozen u vidu niza char-ova duzine 16
  * @param[in] dest Destinacija enkriptovanog fajla, rezultujuci fajl ce biti kreiran na toj lokaciji
  *
  * Funkcija enkriptuje fajl sa imenom 'path' i ne menja dati string. Koristi kljuc koji ucitava iz stringa c,
  * ne menjajuci njegov sadrzaj. Rezultujuci fajl ce se nalaziti na lokaciji 'dest' i imace ime originala, mozda
  * neznatno izmenjeno. Funkcija vraca 0 ako je enkriptovanje uspesno izvrseno, a vraca -1 ako otvaranje fajla nije uspelo.
  *
  */
int triple_DES_encrypt_file(char* path,char *c,char *dest);   //c je duzine 16

/**
  * @brief Funkcija za dekripciju fajla koristeci triple-DES algoritam
  * @param[in] path Ime enkriptovanog fajla
  * @param[in] c Kljuc koji se koristi za dekripciju, prilozen u vidu niza char-ova duzine 16
  * @param[in] dest Destinacija dekriptovanog fajla, rezultujuci fajl ce biti kreiran na toj lokaciji
  *
  * Funkcija dekriptuje fajl sa imenom 'path' i ne menja dati string. Koristi kljuc koji ucitava iz stringa c,
  * ne menjajuci njegov sadrzaj. Rezultujuci fajl ce se nalaziti na lokaciji 'dest' i imace ime originala, koje je
  * upisano u enkriptovani fajl, mozda neznatno izmenjeno. Funkcija vraca 0 ako je enkriptovanje uspesno izvrseno,
  * vraca -1 ako otvaranje fajla nije uspelo, a vraca 1 ako prilozeni kljuc nije odgovarajuci.
  *
  */
int triple_DES_decrypt_file(char* path,char *c,char *dest);   //c je duzine 16



