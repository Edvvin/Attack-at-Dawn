#include <curses.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>

#define MAX_MENUS 20
#define MAX_COLUMNS 20
#define MAX_NAME_LEN 70
#define MAX_FIELD_NAME_LEN 8
#define MAX_SELECT 2000
#define MAX_FIELD_STRING_LEN 70
#define MAX_ERROR_MESSAGE_LEN 100
#define MAX_INPUT_LEN 500
#define MENUS_ERR 0
#define MENUS_OK 1
#define HEADER_WIDTH 0
#define edvin endwin

/**
 * @file
 * @author Edvin Maid
 * @brief Sve funkcije vezane za iscrtavanje i odrzavanje menija
 * Header fajl koji sadrzi sve funkcije koje programer moze da koristi za kreiranje
 * menija i rasporedjivanje GUI elemenata kao i za unos od strane korisnika
 * Oslanja se na biblioteku PDCURSES
 */

typedef enum menus_error {
    M_ERR_NO_ERROR,M_ERR_NO_INITIALIZED_PROG, M_ERR_NULL_PONITER, M_ERR_OVERFLOW, M_ERR_UNDERFLOW, M_ERR_ALLOC, M_ERR_FIELD_POSITION_FILLED,M_ERR_PROG_ALREADY_RUNNING,M_ERR_PROG_ALREADY_ACTIVE,M_ERR_SCREEN_DIM
} MENUS_ERROR;

static MENUS_ERROR m_err;


/**
 * @brief Struktura koja definise sve potrebne informacije za polje
 */
typedef struct field {
    void *x, (*f)(void),(*free_func)(void*);
    char field_name[MAX_FIELD_NAME_LEN], field_string[MAX_FIELD_STRING_LEN];
    int is_selected,position;
    struct field *next, *prev,*next_select,*prev_select;
} FIELD;

/**
 * @brief Struktura koja definise sve potrebne informacije za kolonu
 * Kolona se sastoji od niza vertikalno rasporedjenih polja
 */
typedef struct column {
    int number_of_fields, number_of_selected,width,hasBorder,a_bor,b_bor,is_locked,lock_pos,isCompact,is_designated;
    WINDOW *win;
    char column_name[MAX_NAME_LEN],column_title[MAX_NAME_LEN];
    FIELD *first, *last, *first_visible,*first_selected,*last_selected;
} COLUMN;
/**
 * @brief Struktura koja definise sve potrebne informacije za Meni
 * Meni se sastoji iz horizontalno postavljenih sustednih kolona
 */
typedef struct menu {
    int number_of_columns,start_col;
    COLUMN* columns[MAX_COLUMNS];
    char *column_names[MAX_COLUMNS], menu_name[MAX_NAME_LEN];
    FIELD* cursor_field,*special_field;
    COLUMN* cursor_column;
} MENU;

/**
 * @brief Struktura koja definise sve potrebne informacije za sam program koji se pokrece
 */
typedef struct prog {
    void (*update)(void);
    int number_of_menus, top, is_stopped,column_height,max_width;
    MENU *menus[MAX_MENUS];
    char *menu_names[MAX_MENUS];
    MENU* menu_stack[MAX_MENUS];
} PROG;

static PROG* active_prog = NULL;

/**
 * @brief funkcija koja inicijalizuje program
 * Kreira novi program koji skladisti podatke vezane za menije
 * takodje i vrsi potrebnu inicijalizaciju za biblioteku pdcurses
 * @return 
 */
int init_prog();
/**
 * @brief postavlja funkciju koja ce da se poziva u svakoj iteraciji progamske petlje
 * @param update funkcija koju ce program pozivati
 * Kao argument se prosledjuje funkcija za koju programer zeli da se izvrsava
 * u svakoj iteraciji programske petlje
 */
void set_update(void(*update)(void));
/**
 * @brief Pokrece program koji je prethodno bio inicijalizovan
 * @param startMenu meni iz kojeg program pocinje
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 */
int run_prog(MENU* startMenu);
/**
 * @brief izbacuje program iz programske petlje
 */
void stop_prog();
/**
 * @brief Oslobadja memoriju
 * Oslobadja sve podatke vezane za menije
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 */
int end_prog();
/**
 * @brief Vraca trenutno aktivni meni
 * @return Pokazivac na strukturu MENU koja odgovara meniju koji se trenutno nalazi na ekranu
 */
MENU* get_active_menu();
/**
 * @brief Ne radi nista
 * bas nista
 * @param Pokazivac na nesto
 */
void nothing(void*);
/**
 * @brief Ne radi nista
 * bas bas nista 
 */
void nothing2(void);

//Menu related functions
/**
 * @brief vraca meni sa zadatim imenom
 * @param menuName niz karaktera koji odgovara meniju
 * @return pokazivac na meni sa zadatim imenom ili NULL ukoliko nije nadjen
 * Ova funkcija prolazi kroz niz menija ucitanih u program i pronalazi odgovarajuci
 */
MENU* get_menu(char* menuName);
/**
 * @brief kreira novi meni
 * @param menuName Naziv menija koji ce se posle koristiti da se njemu pristupi
 * @return Pokazivac na kreirani meni
 */
MENU* new_menu(char* menuName);
/**
 * @brief Funkcija koja se pokrece u svakoj iteraciji programskog ciklusa
 * @param M Meni koji ce se iscrtati i azurirati
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 * Iscrtava prosledjeni meni poziva funkciju update prosledjenu kao argument
 * u funkciji set_update i zaustavlja program i ceka unos od strane korisnika
 */
int update_menu(MENU* M);
/**
 * @brief Dodaje kolonu u odgovarajuci meni
 * @param M Meni u koji zelimo da dodamo kolonu
 * @param C Kolona koju zelimo da dodamu u odgovarajuci meni
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 */
int add_column(MENU* M, COLUMN* C);
/**
 * @brief Vraca aktivni meni unazad
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 * Uzima poslenji meni sa steka menija i postavlja ga kao aktivni meni 
 */
int prev_menu();
/**
 * @brief Postavlja sledeci meni na stek
 * @param M meni koji zelimo da postane aktivan
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 * Postavlja zadati meni na stek i proglasava ga aktivnim menijem
 */
int next_menu(MENU* M);
/**
 * @brief Iscrtava meni
 * @param M Meni koji zelimo da iscrtamo
 * Iscrtava sve elemente prosledjenog menija na ekran poziva se u update_menu
 */
void print_menu(MENU* M);

//Column related functions

/**
 * @brief Dohvata kolonu sa zadatim imenom
 * @param M Meni u kojoj se nalazi trazena kolona
 * @param columnName niz karaktera koji odgovara imenu kolone
 * @return Pokazivac na trazenu kolonu ili NULL ukoliko nije nadjena
 * Prolazi kroz niz svih kolona u zadatom meniju i trazi onu sa zadatim imenom
 */
COLUMN* get_column(MENU* M, char* columnName);
/**
 * @brief Kreira novu kolonu
 * @param columnName Ime koje zelimo da pripisemo novoj koloni
 * @param columnTitle Niz karaktera koji zelimo da se ispisuje u zacelju kolone
 * @param width Sirinu kolone zadatu u broju karaktera
 * @return Pokazivac na novu kolonu
 */
COLUMN* new_column(char* columnName,char* columnTitle,int width);
/**
 * @brief Dodaje polje u zadatu kolonu
 * @param C Kolona u koju dodaje polje
 * @param F Polje koje zelimo da dodamo
 * @param position Pozicija odnosno broj mesta od pocetka kolone
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 * Dodaje zadato polje u zadatu kolone na neku poziciju.
 * Ako na primer dodamo jedno polje na poziciju 1 a drugo na poziciju 4 na ekranu ce se ispisati:
 * 
 * polje1
 * 
 * 
 * polje2
 * 
 * Ako prosledimo position<0 onda ce samo dodati polje na kraj
 */
int add_field(COLUMN* C, FIELD* F,int position);
/**
 * @brief Ispisuje kolonu na ekran
 * @param C Kolona koju zelimo da ispisemo na ekran
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 * Poziva se u print_menu() i iscrtava sva polje i okvir zadate kolone na odgovarajucoj poziciji
 */
int print_column(COLUMN* C);
/**
 * @brief Postavlja okvir zadate kolone
 * @param C Kolone kojoj zelimo da dodamo okvir
 * @param hasBorder 1 ako zelimo okvir 0 ako zelimo bez okvira
 * @param a karakter okvira svuda sem coskova, 0 za crticu
 * @param b karakter u coskovima okvira, 0 za prave uglove
 * Postavlja kolonu tako da iscrtava svoj okvir u print_column()
 */
void set_border(COLUMN* C,int hasBorder,int a,int b);
/**
 * @brief Zakljucava poziciju kolone
 * @param C Kolona koju zelimo da zakljucamo
 * @param is_locked 1 ako zelimo da zakljucamo 0 ako zelimo da otkljucamo
 * @param lock_pos pozicija na kojoj zelimo da zakljucamo
 * Pomeranjem kurosora kada se naidje na ivice kolone ne vrsi se "scroll" 
 * a na vrhu kolone iscrtane na ekranu se nalazi polje sa pozicijom lock_pos
 */
void lockColumn(COLUMN* C,int is_locked,int lock_pos);
/**
 * @brief Sabija sva polja kolone tako da nema razmaka
 * @param C Kolona koju zelimo da sabijemo
 * Pozicije kolone postaju zbijene na primer ako kolona poseduje polje sa pozicijom
 * 1 i polje sa pozicijom 4 ispisivace se:
 * polje1
 * polje2
 * 
 * bez razmaka izmedju redova
 */
void compact(COLUMN* C);

//Field related functions
/**
 * @brief Pronalazi polje sa zadatim imenom
 * @param C Kolona u kojoj se nalazi polje
 * @param fieldName Ime trazenog polja
 * @return Pokazivac na polje sa zadatim imenom ili NULL ako nije nadjen
 * Prolazi kroz listu polja u kolonu C i poredi sa fieldName dok ne nadje odgovarajuce polje
 */
FIELD* get_field(COLUMN* C, char* fieldName);
/**
 * @brief Kreira novo polje
 * @param x Pokazivac na memoriju koju zelimo da asociramo sa zadatim poljem
 * @param f Pokazivac na funkciju akcije klika na kreirano polje
 * @param free_func funkcija koja oslobadja memoriju zadatu parametrom x
 * @param fieldName MAX_FIELD_NAME_LEN karaktera koji figurisu ime kreiranog polja
 * @param fieldString MAX_FIELD_STRING_LEN karaktera koje zelimo da ispisemo na polju
 * @return Pokazivac na kreirano polje
 * Kreira polje sa zadatim nizom karaktera za ispis, zadatim imenom, nekom memorijom sa kojom
 * se asocira i funkcijama za brisanje te memorije kao i za akcije prilikom klika na polje
 * Ukoliko je parametar f jednak NULL podrazumeva se funkcija select()
 * Ukoliko je parametar free_func jednak NULL podrazumeva se funkcija free()
 */
FIELD* new_field(void* x, void (*f)(void), void(*free_func)(void*), char* fieldName, char* fieldString);
/**
 * @brief Menja vrednost niza karaktera koji se ispisuje
 * @param F Polje ciji ispis zelimo da promenimo
 * @param fieldString niz karaktera koji ce se ispisivati
 * Kopira zadati niz karaktera u polje i ispisuje
 */
void set_field_string(FIELD* F,char* fieldString);
void execute(FIELD* F);
/**
 * @brief Obelezava polje na kojem je kursor
 * Funkcija koja obelezava polje nad kojim je kursor u aktivnom meniju i dodaje u listu
 * obelezenih polja.
 * Ova funkcija se uzima kao podrazumevana ako je drugi parametar funkcije new_field() jednak NULL
 */
void select();
/**
 * @brief Vraca polje sa zadatim indeksom u listi polja
 * @param C Kolona u kojoj tazimo polje
 * @param index indeks polja koje trazimo pocevsi od 0
 * @return pokazivac na polje sa zadatim indeksom
 * Prolazi kroz listu polja kolone C dok ne pronadje polje sa zadatim indeksom index
 */
FIELD* get_f(COLUMN* C, int index);
/**
 * @brief Uklanja zadato polje iz liste polja kolone C
 * @param C Kolona iz koje zelimo da uklonimo polje F
 * @param F Polje koje zelimo da uklonimo iz kolone C
 * Funkcija prolazi kroz listu polja u koloni C i pronalazi F.
 * Ukoliko ne pronadje odgovarajuce polje nista se ne menja.
 * Pronadjeno F je zatim izbaceno iz liste.
 */
void remove_field(COLUMN* C, FIELD* F);
/**
 * @brief Polje varca u neobelezeno stanje
 * @param C Kolona u kojoj se nalazi obelezeno polje
 * @param F Polje cije stanje zelimo da pretvorimo u neobelezeno
 * Funkcija ukoliko je F obelezeno ga pronalazi i pretvara u neobelezeno polje
 */
void remove_selected_field(COLUMN* C, FIELD* F);
/**
 * @brief Vraca poziciju polja F ukoloni C
 * @param C Kolona u kojoj se nalazi F
 * @param F Polje ciju poziciju trazimo
 * @return pozicija polja F
 * Vraca nam vrednost pozicije polja F
 */
int getPos(COLUMN* C,FIELD* F);
/**
 * @brief Iscrtava polje F
 * @param C Kolona u kojoj se nalazi polje F
 * @param F polje ciji field_string zelimo da ispisemo
 * @param pos poziciju od vrha iscrtane kolone na kojoj zelimo da iscrtamo polje
 * Iscrtava polje iz kolone C na poziciji pos od vrha kolone
 */
void print_field(COLUMN* C,FIELD* F,int pos);
/**
 * @brief Ispisuje naslov zadatke kolone
 * @param C Kolona ciji naslov ispisuje
 * @param s Niz karaktera koji zelimo da ispisemo kao naslov
 * Ispisuje niz karaktera s u zaglavlju kolone C
 */
void print_title(COLUMN* C, char* s);
/**
 * @brief Oslobadja memoriju vezanu za polje
 * @param F
 * Brise memoriju polja F kao i poziva zadatu funkciju za brisanje memorije asocirane sa poljem F
 */
void free_field(FIELD* F);

//cursor
/**
 * @brief Pomera kursor gore
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 * Pomera kursor na ekranu za jedno polje gore i ukoliko naidje na gornju granicu kolone vrsi "scroll"
 */
int cursor_up();
/**
 * @brief Pomera kursor dole
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske 
 * Pomera kursor na ekranu za jedno polje dole i ukoliko naidje na gornju granicu kolone vrsi "scroll"
 */
int cursor_down();
/**
 * @brief Pomera kursor u susednu levu kolonu
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske
 * Pomera kursor u prvo vidljivo polje kolone koja se nalazi levo od trenutno kolone,
 * a ukoliko ne postoji leva kolona koja sadrzi bar jedno polje onda kursor ostaje u istoj koloni
 */
int cursor_left();
/**
 * @brief Pomera kursor u susednu desnu kolonu
 * @return definiciju MENUS_OK ukoliko je uspesno izvrseno ili MENUS_ERR ukoliko je doslo do greske
 * Pomera kursor u prvo vidljivo polje kolone koja se nalazi desno od trenutno kolone,
 * a ukoliko ne postoji desna kolona koja sadrzi bar jedno polje onda kursor ostaje u istoj koloni
 */
int cursor_right();

/**
 * @brief Pokrece primitivni input-box pop up za unos niza karaktera
 * @param height Visina prozora pop up-a u broju karaktera
 * @param width Sirina prozora pop up-a u broju karaktera
 * @param title Niz karaktera koji ce biti ispisan na vrhu prozora
 * @param message Niz karaktera koji ce biti ispisan u prozoru
 * @param input niz karaktera u kojem ce se smestiti korisnicki unos
 * Funkcija pokrece prozor u kojem ce biti ispisana poruka zajedno sa naslovom
 * i jednim poljem za unos sa najvise MAX_INPUT_LEN karaktera
 */
void input_box(int height,int width,char* title,char* message,char* input);
/**
 * @brief Pokrece primitivni pop up za prikazivanje poruke
 * @param height Visina prozora pop up-a u broju karaktera
 * @param width Sirina prozora pop up-a u broju karaktera
 * @param title Niz karaktera koji ce biti ispisan na vrhu prozora
 * @param message Niz karaktera koji ce biti ispisan u prozoru
 * Funkcija pokrece prozor u kojem ce biti ispisana poruka zajedno sa naslovom
 */
void message_box(int height,int width,char* title,char* message);