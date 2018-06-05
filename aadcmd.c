#include "aadcmd.h"
#include "DES.h"
#define MAX_MESSAGE_LEN 200
#define LOAD_BLOCK 20

void start_cmd_mode(int argc, char** argv) {
    FILE* log_file = NULL;
    interpret(argc, argv, 0, &log_file);
    if (log_file) {
        fclose(log_file);
    }
}

void interpret(int argc, char** argv, int is_batch, FILE** log_file) {
    char* cmd = argv[0];
    char msg[200];
    if (!strcmp(cmd, "batch") && !is_batch) {
        batch_mode(argc - 1, argv + 1, log_file);
    } else if (!strcmp(cmd, "3des")) {
        triple_des_cmd(argc - 1, argv + 1, is_batch, log_file);
    } else if (!strcmp(cmd, "des")) {
        des_cmd(argc - 1, argv + 1, is_batch, log_file);
    } else if (!strcmp(cmd, "aes")) {
        aes_cmd(argc - 1, argv + 1, is_batch, log_file);
    } else {
        sprintf(msg, "%s is not a recognised command", cmd);
        print_to_log(msg, is_batch, log_file);
    }
}

void aes_cmd(int argc, char** argv, int is_batch, FILE** log_file) {
    int is_key_loaded = 0;
    int len = 0;
    char* key;
    int decrypt = 0;
    int is_destination_loaded = 0;
    int files_start = 0;
    char* dest;
    char msg[200];
    for (int i = 0; i < argc; i++) {
        if (files_start) {
            FILE* f = fopen(argv[i], "r");
            if (!f) {
                sprintf(msg, "file with path: ");
                for (int j = 0; argv[i][j]; j++) {
                    sprintf(msg + strlen(msg), "%c", argv[i][j]);
                }
                sprintf(msg + strlen(msg), " not found");
                print_to_log(msg, is_batch, log_file);
            } else {
                fclose(f);
                if (decrypt) {
                    unsigned char* k = hex2key(key);
                    int er = Aes_Decipher_File(argv[i], k, len / 2);
                    free(k);
                    if (er) {
                        sprintf(msg, "file with path: ");
                        for (int j = 0; argv[i][j]; j++) {
                            sprintf(msg + strlen(msg), "%c", argv[i][j]);
                        }
                        sprintf(msg + strlen(msg), " could not be decrypted with the given key");
                        print_to_log(msg, is_batch, log_file);
                    }
                } else {
                    unsigned char* k;
                    k = hex2key(key);
                    Aes_Cipher_File(argv[i], k, len / 2);
                    free(k);
                }
            }
            continue;
        }
        if (!strcmp(argv[i], "-encrypt")) {
            decrypt = 0;
        } else if (!strcmp(argv[i], "-decrypt")) {
            decrypt = 1;
        } else if (!strcmp(argv[i], "-destination")) {
            if (!is_destination_loaded && i + 1 < argc) {
                struct stat sb;
                if (stat(argv[i + 1], &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    dest = argv[i + 1];
                    i++;
                    is_destination_loaded = 1;
                } else {
                    sprintf(msg, "Destination: ");
                    for (int j = 0; argv[i][j]; j++) {
                        sprintf(msg + strlen(msg), "%c", argv[i][j]);
                    }
                    sprintf(msg + strlen(msg), " not found");
                    print_to_log(msg, is_batch, log_file);
                    return;
                }
            } else if (i + 1 < argc) {
                sprintf(msg, "Missing argument for -destination");
                print_to_log(msg, is_batch, log_file);
                return;
            }
        } else if (!is_key_loaded) {
            len = strlen(argv[i]);
            int y;
            for(y = 0; y<len;y++){
                if(!strchr("0123456789ABCDEFabcdef",argv[i][y]))break;
            }
            if(y<len){
                sprintf(msg, "The key must contain only hexadecimal digits 0-F");
                print_to_log(msg, is_batch, log_file);
                return;
            }
            if (len != 32 && len != 48 && len != 64) {
                sprintf(msg, "The key ");
                for (int j = 0; argv[i][j]; j++) {
                    sprintf(msg + strlen(msg), "%c", argv[i][j]);
                }
                sprintf(msg + strlen(msg), " should be of length 32,48 or 64");
                print_to_log(msg, is_batch, log_file);
                return;
            } else {
                key = argv[i];
                is_key_loaded = 1;
            }
        } else {
            files_start = 1;
            i--;
        }
    }
}

void triple_des_cmd(int argc, char** argv, int is_batch, FILE** log_file) {
    int is_key_loaded = 0;
    int len = 0;
    char* key;
    int decrypt = 0;
    int is_destination_loaded = 0;
    int files_start = 0;
    char* dest;
    char msg[200];
    for (int i = 0; i < argc; i++) {
        if (files_start) {
            FILE* f = fopen(argv[i], "r");
            if (!f) {
                sprintf(msg, "file with path: ");
                for (int j = 0; argv[i][j]; j++) {
                    sprintf(msg + strlen(msg), "%c", argv[i][j]);
                }
                sprintf(msg + strlen(msg), " not found");
                print_to_log(msg, is_batch, log_file);
            } else {
                fclose(f);
                if (decrypt) {
                    char* k = hex2key(key);
                    int er = triple_DES_decrypt_file(argv[i], k);
                    free(k);
                    if (er) {
                        sprintf(msg, "file with path: ");
                        for (int j = 0; argv[i][j]; j++) {
                            sprintf(msg + strlen(msg), "%c", argv[i][j]);
                        }
                        sprintf(msg + strlen(msg), " could not be decrypted with the given key");
                        print_to_log(msg, is_batch, log_file);
                    }
                } else {
                    char* k = hex2key(key);
                    triple_DES_encrypt_file(argv[i], k);
                    free(k);
                }
            }
            continue;
        }
        if (!strcmp(argv[i], "-encrypt")) {
            decrypt = 0;
        } else if (!strcmp(argv[i], "-decrypt")) {
            decrypt = 1;
        } else if (!strcmp(argv[i], "-destination")) {
            if (!is_destination_loaded && i + 1 < argc) {
                struct stat sb;
                if (stat(argv[i + 1], &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    dest = argv[i + 1];
                    i++;
                    is_destination_loaded = 1;
                } else {
                    sprintf(msg, "Destination: ");
                    for (int j = 0; argv[i][j]; j++) {
                        sprintf(msg + strlen(msg), "%c", argv[i][j]);
                    }
                    sprintf(msg + strlen(msg), " not found");
                    print_to_log(msg, is_batch, log_file);
                    return;
                }
            } else if (i + 1 < argc) {
                sprintf(msg, "Missing argument for -destination");
                print_to_log(msg, is_batch, log_file);
                return;
            }
        } else if (!is_key_loaded) {
            len = strlen(argv[i]);
            int y;
            for(y = 0; y<len;y++){
                if(!strchr("0123456789ABCDEFabcdef",argv[i][y]))break;
            }
            if(y<len){
                sprintf(msg, "The key must contain only hexadecimal digits 0-F");
                print_to_log(msg, is_batch, log_file);
                return;
            }
            if (len != 32 ) {
                sprintf(msg, "The key ");
                for (int j = 0; argv[i][j]; j++) {
                    sprintf(msg + strlen(msg), "%c", argv[i][j]);
                }
                sprintf(msg + strlen(msg), " should be of length 32");
                print_to_log(msg, is_batch, log_file);
                return;
            } else {
                key = argv[i];
                is_key_loaded = 1;
            }
        } else {
            files_start = 1;
            i--;
        }
    }
}

void des_cmd(int argc, char** argv, int is_batch, FILE** log_file) {
    int is_key_loaded = 0;
    int len = 0;
    char* key;
    int decrypt = 0;
    int is_destination_loaded = 0;
    int files_start = 0;
    char* dest;
    char msg[200];
    for (int i = 0; i < argc; i++) {
        if (files_start) {
            FILE* f = fopen(argv[i], "r");
            if (!f) {
                sprintf(msg, "file with path: ");
                for (int j = 0; argv[i][j]; j++) {
                    sprintf(msg + strlen(msg), "%c", argv[i][j]);
                }
                sprintf(msg + strlen(msg), " not found");
                print_to_log(msg, is_batch, log_file);
            } else {
                fclose(f);
                if (decrypt) {
                    char* k = hex2key(key);
                    int er = DES_decrypt_file(argv[i], k);
                    free(k);
                    if (er) {
                        sprintf(msg, "file with path: ");
                        for (int j = 0; argv[i][j]; j++) {
                            sprintf(msg + strlen(msg), "%c", argv[i][j]);
                        }
                        sprintf(msg + strlen(msg), " could not be decrypted with the given key");
                        print_to_log(msg, is_batch, log_file);
                    }
                } else {
                    char* k = hex2key(key);
                    DES_encrypt_file(argv[i], k);
                    free(k);
                }
            }
            continue;
        }
        if (!strcmp(argv[i], "-encrypt")) {
            decrypt = 0;
        } else if (!strcmp(argv[i], "-decrypt")) {
            decrypt = 1;
        } else if (!strcmp(argv[i], "-destination")) {
            if (!is_destination_loaded && i + 1 < argc) {
                struct stat sb;
                if (stat(argv[i + 1], &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    dest = argv[i + 1];
                    i++;
                    is_destination_loaded = 1;
                } else {
                    sprintf(msg, "Destination: ");
                    for (int j = 0; argv[i][j]; j++) {
                        sprintf(msg + strlen(msg), "%c", argv[i][j]);
                    }
                    sprintf(msg + strlen(msg), " not found");
                    print_to_log(msg, is_batch, log_file);
                    return;
                }
            } else if (i + 1 < argc) {
                sprintf(msg, "Missing argument for -destination");
                print_to_log(msg, is_batch, log_file);
                return;
            }
        } else if (!is_key_loaded) {
            len = strlen(argv[i]);
            int y;
            for(y = 0; y<len;y++){
                if(!strchr("0123456789ABCDEFabcdef",argv[i][y]))break;
            }
            if(y<len){
                sprintf(msg, "The key must contain only hexadecimal digits 0-F");
                print_to_log(msg, is_batch, log_file);
                return;
            }
            if (len != 16 ) {
                sprintf(msg, "The key ");
                for (int j = 0; argv[i][j]; j++) {
                    sprintf(msg + strlen(msg), "%c", argv[i][j]);
                }
                sprintf(msg + strlen(msg), " should be of length 16");
                print_to_log(msg, is_batch, log_file);
                return;
            } else {
                key = argv[i];
                is_key_loaded = 1;
            }
        } else {
            files_start = 1;
            i--;
        }
    }
}

FILE* open_log() {
    char str[100];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(str, "aad_log_file_%d_%d_%d_%d_%d_%d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    FILE* log = fopen(str, "w");
    return log;
}

void print_to_log(char* outstr, int is_batch, FILE** log_file) {
    if (is_batch) {
        if (!*log_file)
            *log_file = open_log();
        fprintf(*log_file, "%s\n", outstr);
    } else {
        printf("%s\n", outstr);
    }
}


void batch_mode(int argc, char** argv, FILE** log_file) {
    char msg[200];
    for (int i = 0; i < argc; i++) {
        FILE* f = fopen(argv[i], "r");
        if (!f) {
            sprintf(msg, "Could not find file %s", argv[i]);
            print_to_log(msg, 1, log_file);
        } else {
            close(f);
            do_batch(argv[i], log_file);
        }
    }
}

void do_batch(char* batch_file, FILE** log_file) {
    FILE* f = fopen(batch_file, "r");
    int cmd_count = 0;
    int done = 0;
    int* argc = malloc(LOAD_BLOCK * sizeof (int));
    char ***args = malloc(LOAD_BLOCK * sizeof (char**));
    do {
        char* str = malloc(LOAD_BLOCK * sizeof (char));
        int n = 0;
        int started = 0;
        while (1) {
            char c;
            int check = fscanf(f, "%c", &c);
            if (((c == '\n' || c == '\r') && started)|| check == EOF) {
                if (check == EOF)
                    done = 1;
                str[n] = '\0';
                str = realloc(str, sizeof (char)*(n + 1));
                started = 0;
                break;
            } else if (started || (!isblank(c) && c != '\n' && c != '\r')) {
                str[n++] = c;
                started = 1;
                if (!(n % LOAD_BLOCK))
                    str = realloc(str, sizeof (int)*(n + LOAD_BLOCK));
            }
        }
        if (n) {
            args[cmd_count] = malloc(sizeof (char*)*LOAD_BLOCK);
            int count = 0;

            char* s = str, *nxt;

            do {
                nxt = strchr(s, ' ');
                if (!nxt) {
                    int m = strlen(s);
                    args[cmd_count][count] = malloc(sizeof (char)*(m + 1));
                    for (int i = 0; i < m; i++) {
                        args[cmd_count][count][i] = s[i];
                    }
                    args[cmd_count][count][m] = '\0';
                    count++;
                    break;
                }
                int m = nxt - s;
                args[cmd_count][count] = malloc(sizeof (char)*(m + 1));
                for (int i = 0; i < m; i++) {
                    args[cmd_count][count][i] = s[i];
                }
                args[cmd_count][count][m] = '\0';
                count++;
                if (!(count % LOAD_BLOCK))
                    args[cmd_count] = realloc(args[cmd_count], sizeof (char*)*(LOAD_BLOCK + count));
                s = nxt + 1;
            } while (*s != '\0');
            argc[cmd_count++] = count;
            if (!(cmd_count % LOAD_BLOCK))
                args = realloc(args, sizeof (char**)*(cmd_count + LOAD_BLOCK));
        }
        free(str);
    } while (!done);
    for (int i = 0; i < cmd_count; i++) {
        interpret(argc[i], args[i], 1, log_file);
        for (int j = 0; j < argc[i]; j++) {
            free(args[i][j]);
        }
        free(args[i]);
    }
    free(argc);
    free(args);
}