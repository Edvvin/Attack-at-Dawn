#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include "aes.h"
#include "DES.h"
#include "keydecode.h"
#include <time.h>

/**
 * @file
 * @author Edvin Maid
 * @brief Header sa svim funkcijama za konzolni mod
 * Header fajl sa svim funkcijama koji su potrebni za pokretanje instrukcija 
 * pozvanih pomocu konzole.
 */

/**
 * @brief Zapocinje konzolni mod rada
 * @param argc broj argumenata
 * @param argv niz argumenata koje program obradjuje bez imena programa
 * Zapocinje konzolni mod rada i prosledjuje zadate instrukcije i parametre i izvrsava ih
 */
void start_cmd_mode(int argc, char** argv);
/**
 * @brief Interpretira instrukciju
 * @param argc broj argumenata
 * @param argv niz argumenata bez imena programa
 * @param is_batch 1 ako je batch rezim rada ukljucen 0 ako je iskljucen
 * @param log_file fajl u koji ce se upisati poruke ukoliko dodje do greske u obradi
 */
void interpret(int argc, char** argv, int is_batch, FILE** log_file);
/**
 * @brief Zapocinje log fajl
 * @return Pokazivac na log fajl
 * Otvara novi fajl u rezimu "w" u kojem ce se upisivati sve poruke za greske
 * koje budu nastale u batch rezimu rada
 */
FILE* open_log();
/**
 * @brief Ispisuje zadatu poruku
 * @param outstr Poruka koju ispisuje
 * @param is_batch 1 ako je batch rezim rada ukljucen 0 ako je iskljucen
 * @param log_file fajl u koji ce se upisati poruke ukoliko dodje do greske u obradi
 * Ispisuje zadatu poruku na standardni izlaz ukoliko je is_batch = 0
 * Ispisuje zadatu poruku u log fajl ukoliko je is_batch = 1
 * Ukoliko fajl nije kreiran poziva open_log
 */
void print_to_log(char* outstr,int is_batch,FILE** log_file);

/**
 * @brief Zapocinje batch rezim rada
 * @param argc broj fajlova koje treba izvrsiti u batch rezimu
 * @param argv destinacije fajlova koje treba izvrsiti u batch rezimu
 * @param log_file fajl u koji ce se upisati poruke ukoliko dodje do greske u obradi
 * Funkcija zapocinje batch rezim rada i za svaki fajl pokrece do_batch
 */
void batch_mode(int argc,char** argv, FILE** log_file);
/**
 * @brief Izvrsava instrukcije u batch rezimu
 * @param batch_file putanja do fajla koji izvrsava
 * @param log_file fajl u koji ce se upisati poruke ukoliko dodje do greske u obradi
 * Parsira zadati fajl i deli ga u niz instrukcija koje prosledjuje interpret funkciji
 */
void do_batch(char* batch_file, FILE** log_file);
/**
 * @brief parsira komandu 3des i pokrece triple des
 * @param argc broj argumenata za komandu 3des
 * @param argv argumenti za komandu 3des
 * @param is_batch 1 ako je batch rezim rada ukljucen 0 ako je iskljucen
 * @param log_file fajl u koji ce se upisati poruke ukoliko dodje do greske u obradi
 * Parsira komandu i njene argumente, proverava ispravnost, ukoliko dodje do greske ispise je
 * koriscenjem funkcije print_to_log()
 * Uspesno parsirane komande pokrece
 */
void triple_des_cmd(int argc, char** argv, int is_batch, FILE** log_file);
/**
 * @brief parsira komandu 3des i pokrece des
 * @param argc broj argumenata za komandu des
 * @param argv argumenti za komandu des
 * @param is_batch 1 ako je batch rezim rada ukljucen 0 ako je iskljucen
 * @param log_file fajl u koji ce se upisati poruke ukoliko dodje do greske u obradi
 * Parsira komandu i njene argumente, proverava ispravnost, ukoliko dodje do greske ispise je
 * koriscenjem funkcije print_to_log()
 * Uspesno parsirane komande pokrece
 */
void des_cmd(int argc, char** argv, int is_batch, FILE** log_file);
/**
 * @brief parsira komandu 3des i pokrece triple aes
 * @param argc broj argumenata za komandu aes
 * @param argv argumenti za komandu aes
 * @param is_batch 1 ako je batch rezim rada ukljucen 0 ako je iskljucen
 * @param log_file fajl u koji ce se upisati poruke ukoliko dodje do greske u obradi
 * Parsira komandu i njene argumente, proverava ispravnost, ukoliko dodje do greske ispise je
 * koriscenjem funkcije print_to_log()
 * Uspesno parsirane komande pokrece
 */
void aes_cmd(int argc, char** argv, int is_batch, FILE** log_file);