#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @file
 * @author Edvin Maid
 * @brief Sve funkcije za konvertovanje kljuca
 * Header file koji sadrzi sve funkcije vezane za konverzije kljuca iz jednog oblika u drugi
 */

/**
 * @breif Konvertuje niz heksadecimalnih karaktera u kljuc 
 * @param key Kljuc u obliku niza karaktera 0-9 ili A-F
 * @return Niz karaktera koji odgovaraju kljucu za AES I DES algoritme
 * Uzima niz karaktera koji predstavljaju heksadeicmalne cifre i pretvara ih
 * u niz karaktera koji odgovara kljucu
 */
unsigned char* hex2key(char* key);