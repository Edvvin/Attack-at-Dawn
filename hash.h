#pragma once

long long mojHash(char *path,char fajlVecImaHash,char *kljuc,char duzinaKljuca,int metod);/*    metod:
                                                                                                    17 = DES
                                                                                                    71 = tripleDES
                                                                                                    Jovan:  (treba proveriti sa njim)
                                                                                                    23 = nesto
                                                                                                    47 = nesto drugo
                                                                                                    61 = nesto trece
                                                                                          */

long long procitajHash(char *path);
void upisiHash(char *path,long long hes);

int isGood(char *path,char *kljuc,char duzinaKljuca,int metod);
