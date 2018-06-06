#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include "aes.h"
#include "DES.h"
#include "keydecode.h"
#include <time.h>

void start_cmd_mode(int argc, char** argv);
void interpret(int argc, char** argv, int is_batch, FILE** log_file);
FILE* open_log();
void print_to_log(char* outstr,int is_batch,FILE** log_file);


void batch_mode(int argc,char** argv, FILE** log_file);
void do_batch(char* batch_file, FILE** log_file);
void triple_des_cmd(int argc, char** argv, int is_batch, FILE** log_file);
void des_cmd(int argc, char** argv, int is_batch, FILE** log_file);
void aes_cmd(int argc, char** argv, int is_batch, FILE** log_file);