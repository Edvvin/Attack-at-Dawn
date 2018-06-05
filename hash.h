#pragma once

long long mojHash(char *path,char fajlVecImaHash,char *kljuc,char duzinaKljuca,int metod);/*    metod:
                                                                                                    17 = DES
                                                                                                    71 = tripleDES
                                                                                                    Jovan:
                                                                                                    23 = 128 bita
                                                                                                    47 = 192 bita
                                                                                                    61 = 256 bita
                                                                                          */

long long procitajHash(char *path);
void upisiHash(char *path,long long hes);

int isGood(char *path,char *kljuc,char duzinaKljuca,int metod);


int Dodaj_ime_i_velicinu(char *,char *);
void procitajINFO(char *path,char *ime,int *velicina,int imaIV,char *iv,int *pocetakFajla);
