#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_SIZE 1000

void createfile(char *command);
int getcommand(char *clipboard);
void direct(char *chosen_part);
void insertstr(char *command);
void cat(char *command);
void removestr(char *command);
void copystr(char *command, char *clipboard);
void cutstr(char *command, char *clipboard);
void pastestr(char *command, char *clipboard);

int main() {
    char *clipboard= (char *)malloc(MAX_SIZE * sizeof (char));
    while (1) if (!getcommand(clipboard)) printf("invalid command\n");
}

void createfile(char *command) {
    int i = 0, j = 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    memset(chosen_part, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (i != strlen(command)) {
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    direct(chosen_part);
    FILE *fPtr;
    fPtr = fopen(chosen_part, "w");
    if (fPtr == NULL) {
        printf("Unable to create file.\n");
        return;
    }
    printf("Success!\n");
}

int getcommand(char *clipboard) {
    int i = 0;
    char *command = (char *) malloc(MAX_SIZE * sizeof(char));
    char *special_part = (char *) malloc(MAX_SIZE * sizeof(char));
    memset(command, 0, MAX_SIZE);
    memset(special_part, 0, MAX_SIZE);
    gets(command);
    while (command[i] != ' ' && command[i + 1] != '-') {
        special_part[i] = command[i];
        i++;
    }
    if (strcmp(special_part, "createfile") == 0) {
        createfile(command);
        return 1;
    } else if (strcmp(special_part, "insertstr") == 0) {
        insertstr(command);
        return 1;
    } else if (strcmp(special_part, "cat") == 0) {
        cat(command);
        return 1;
    } else if (strcmp(special_part, "removestr") == 0) {
        removestr(command);
        return 1;
    }else if(strcmp(special_part, "copystr") == 0){
        copystr(command, clipboard);
        return 1;
    }else if(strcmp(special_part, "cutstr") == 0){
        cutstr(command, clipboard);
        return 1;
    }else if(strcmp(special_part, "pastestr") == 0){
        pastestr(command, clipboard);
        return 1;
    }
    return 0;
}

void direct(char *chosen_part) {
    int i = 0, slashe = 0;
    while (chosen_part[i] != '\0') {
        if (chosen_part[i] == '/') slashe++;
        i++;
    }
    if (slashe != 0) {
        char *path = (char *) calloc(MAX_SIZE, sizeof(char));
        memset(path, 0, MAX_SIZE);
        for (int j = 0; j < MAX_SIZE; j++) {
            if (chosen_part[j] == '/') {
                mkdir(path);
                slashe--;
                if (slashe == 0) break;
            }
            path[j] = chosen_part[j];
        }
        memset(path, 0, MAX_SIZE);
    }
}

void insertstr(char *command) {
    int i = 0, j = 0, k = 0, line = 0, pos = 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy = (char *) malloc(MAX_SIZE * sizeof(char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (command[i] != ' ' && command[i + 1] != '-') {
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    j = 0;
    if (access(chosen_part, F_OK) == 0) {
        i = 0, j = 0;
        memset(chosen_part, 0, MAX_SIZE);
        memset(chosen_part_cpy, 0, MAX_SIZE);
        while (command[i] != '/') i++;
        i++;
        while (command[i] != ' ' && command[i + 1] != '-') i++;
        i += 7;
        while (command[i] != ' ' && command[i + 1] != '-') i++;
        i += 7;
        while (command[i] != ':') {
            line = line * 10 + command[i] - 48;
            i++;
        }
        i++;
        while (command[i] != '\0') {
            pos = pos * 10 + command[i] - 48;
            i++;
        }
        i = 0;
        memset(chosen_part, 0, MAX_SIZE);
        while (command[i] != '/') i++;
        i++;
        while (command[i] != ' ' && command[i + 1] != '-') {
            chosen_part[j] = command[i];
            i++;
            j++;
        }
        j = 0;
        strcpy(chosen_part_cpy, chosen_part);
        memset(chosen_part, 0, MAX_SIZE);
        i += 7;
        while (command[i] != ' ' && command[i + 1] != '-') {
            chosen_part[j] = command[i];
            i++;
            j++;
        }
        i = 0, j = 0, k = 0;
        //*********************************************************************************************
        FILE *file = fopen("root/kiri_project.txt", "a");
        FILE *fPtr = fopen(chosen_part_cpy, "r");
        char str[MAX_SIZE], c;
        c = '0';
        while (c != EOF) {
            if (c == '\n') i++;
            c = fgetc(fPtr);
            memset(str, 0, MAX_SIZE);
            if (i == line) {
                if (j < pos) {
                    fputc(c, file);
                    j++;
                } else if (j == pos) {
                    while (k != strlen(chosen_part)) {
                        if (chosen_part[k] == '\\' && chosen_part[k + 1] == 'n') {
                            fputc('\n', file);
                            k += 2;
                        } else if (chosen_part[k] == '\\' && chosen_part[k + 1] == '\\') {
                            fputc('\\', file);
                            k += 2;
                        } else {
                            fputc(chosen_part[k], file);
                            k++;
                        }
                    }
                    fputc(c, file);
                    j++;
                } else if (j > pos) {
                    fputc(c, file);
                    j++;
                }
            } else {
                fputc(c, file);
            }
        }
        printf("Success!\n");
        fclose(file);
        fclose(fPtr);
        file = fopen("root/kiri_project.txt", "r");
        fPtr = fopen(chosen_part_cpy, "w");
        i = 0;
        c = fgetc(file);
        while (c != EOF) {
            fputc(c, fPtr);
            c = fgetc(file);
        }
        fclose(file);
        fclose(fPtr);
        remove("root/kiri_project.txt");
    } else printf("file doesn't exist!\n");
}

void cat(char *command) {
    int i = 0, j = 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    memset(chosen_part, 0, MAX_SIZE);
    while (command[i] != ' ' && command[i + 1] != '-') i++;
    i += 9;
    while (command[i] != '\0') {
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    FILE *fPtr;
    char c;
    fPtr = fopen(chosen_part, "r");
    if (fPtr == NULL) {
        printf("Cannot open file \n");
        return;
    }
    c = fgetc(fPtr);
    while (c != EOF) {
        printf("%c", c);
        c = fgetc(fPtr);
    }
    printf("\n");
    fclose(fPtr);
}

void removestr(char *command) {
    int i = 0, j = 0, k, line = 0, pos = 0, length= 0, max_length = 0, kiram_too_net= 0;
    long long length_of_line[MAX_SIZE];
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy = (char *) malloc(MAX_SIZE * sizeof(char));
    char *all_char = (char *) malloc(MAX_SIZE * sizeof (char));
    char *new_all_char = (char *) malloc (MAX_SIZE * sizeof (char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    memset(all_char, 0, MAX_SIZE);
    memset(new_all_char, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (command[i] != ' ' && command[i + 1] != '-') {
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    if (access(chosen_part, F_OK) == 0) {
        i = 0;
        memset(chosen_part_cpy, 0, MAX_SIZE);
        while (command[i] != '/') i++;
        i++;
        while (command[i] != ' ' && command[i + 1] != '-') i++;
        i += 7;
        while (command[i] != ':') {
            line = line * 10 + command[i] - 48;
            i++;
        }
        i++;
        while (command[i] != ' ' && command[i + 1] != '-') {
            pos = pos * 10 + command[i] - 48;
            i++;
        }
        while (command[i] != ' ' && command[i + 1] != '-') i++;
        i += 8;
        while (command[i] != ' ' && command[i + 1] != '-') {
            length = length * 10 + command[i] - 48;
            i++;
        }
        i += 3;
        strcpy(chosen_part_cpy, chosen_part);
        memset(chosen_part, 0, MAX_SIZE);
        chosen_part[0] = command[i];
        i = 0, j = 0, k = 0;
        //*********************************************************************************************
        FILE *fPtr = fopen(chosen_part_cpy, "r");
        char c = '0';
        while (c != EOF) {
            c = fgetc(fPtr);
            if (c == '\n') {
                length_of_line[i]= j + 1;
                all_char[k] = c;
                j = 0;
                i++;
                k++;
            }else {
                all_char[k] = c;
                k++;
                j++;
            }
        }
        for(int l= 0; l < i; l++) max_length = max_length + length_of_line[l];
        for(int l= 0; l< line - 1; l++){
            kiram_too_net = kiram_too_net + length_of_line[l];
        }
        pos= pos + kiram_too_net;
        if(chosen_part[0] == 'b'){
            for(i = 0; i< pos - length; i++){
                new_all_char[i]= all_char[i];
            }
            for(i= 0; i< max_length - pos;i++){
                new_all_char[pos - length + i]= all_char[pos + i];
            }
            fclose(fPtr);
            fopen(chosen_part_cpy, "w");
            fputs(new_all_char, fPtr);
            fclose(fPtr);
            printf("Success!\n");
        }
        if(chosen_part[0] == 'f'){
            for(i = 0; i< pos; i++){
                new_all_char[i]= all_char[i];
            }
            for(i= 0; i< max_length - pos - length;i++){
                new_all_char[pos + i]= all_char[pos + length + i];
            }
            fclose(fPtr);
            fopen(chosen_part_cpy, "w");
            fputs(new_all_char, fPtr);
            fclose(fPtr);
            printf("Success!\n");
        }
    } else printf("file doesn't exist!\n");
}

void copystr(char *command, char *clipboard){
    memset(clipboard, 0, MAX_SIZE);
    int i = 0, j = 0, k, line = 0, pos = 0, length= 0, max_length = 0, kiram_too_net= 0;
    long long length_of_line[MAX_SIZE];
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy = (char *) malloc(MAX_SIZE * sizeof(char));
    char *all_char = (char *) malloc(MAX_SIZE * sizeof (char));
    char *new_all_char = (char *) malloc (MAX_SIZE * sizeof (char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    memset(all_char, 0, MAX_SIZE);
    memset(new_all_char, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (command[i] != ' ' && command[i + 1] != '-') {
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    if (access(chosen_part, F_OK) == 0) {
        i = 0;
        memset(chosen_part_cpy, 0, MAX_SIZE);
        while (command[i] != '/') i++;
        i++;
        while (command[i] != ' ' && command[i + 1] != '-') i++;
        i += 7;
        while (command[i] != ':') {
            line = line * 10 + command[i] - 48;
            i++;
        }
        i++;
        while (command[i] != ' ' && command[i + 1] != '-') {
            pos = pos * 10 + command[i] - 48;
            i++;
        }
        while (command[i] != ' ' && command[i + 1] != '-') i++;
        i += 8;
        while (command[i] != ' ' && command[i + 1] != '-') {
            length = length * 10 + command[i] - 48;
            i++;
        }
        i += 3;
        strcpy(chosen_part_cpy, chosen_part);
        memset(chosen_part, 0, MAX_SIZE);
        chosen_part[0] = command[i];
        i = 0, j = 0, k = 0;
        //*********************************************************************************************
        FILE *fPtr = fopen(chosen_part_cpy, "r");
        char c = '0';
        while (c != EOF) {
            c = fgetc(fPtr);
            if (c == '\n') {
                length_of_line[i]= j + 1;
                all_char[k] = c;
                j = 0;
                i++;
                k++;
            }else {
                all_char[k] = c;
                k++;
                j++;
            }
        }
        for(int l= 0; l < i; l++) max_length = max_length + length_of_line[l];
        for(int l= 0; l< line - 1; l++){
            kiram_too_net = kiram_too_net + length_of_line[l];
        }
        pos= pos + kiram_too_net;
        if(chosen_part[0] == 'b'){
            for(i = 0; i<length; i++){
                clipboard[i]= all_char[pos - length + i];
            }
            fclose(fPtr);
            printf("Success!\n");
        }
        if(chosen_part[0] == 'f'){
            for(i = 0; i< length; i++){
                clipboard[i]= all_char[pos + i];
            }
            fclose(fPtr);
            printf("Success!\n");
        }
    } else printf("file doesn't exist!\n");
}

void cutstr(char *command, char *clipboard){
    copystr(command, clipboard);
    removestr(command);
}

void pastestr(char *command, char *clipboard){
    int i = 0, j = 0, k = 0, line = 0, pos = 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy = (char *) malloc(MAX_SIZE * sizeof(char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (command[i] != ' ' && command[i + 1] != '-') {
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    j = 0;
    if (access(chosen_part, F_OK) == 0) {
        i = 0, j = 0;
        memset(chosen_part, 0, MAX_SIZE);
        memset(chosen_part_cpy, 0, MAX_SIZE);
        while (command[i] != '/') i++;
        i++;
        while (command[i] != ' ' && command[i + 1] != '-') i++;
        i += 7;
        while (command[i] != ':') {
            line = line * 10 + command[i] - 48;
            i++;
        }
        i++;
        while (command[i] != '\0') {
            pos = pos * 10 + command[i] - 48;
            i++;
        }
        i = 0;
        memset(chosen_part, 0, MAX_SIZE);
        while (command[i] != '/') i++;
        i++;
        while (command[i] != ' ' && command[i + 1] != '-') {
            chosen_part[j] = command[i];
            i++;
            j++;
        }
        i = 0, j = 0, k = 0;
        //*********************************************************************************************
        FILE *file = fopen("root/kiri_project.txt", "a");
        FILE *fPtr = fopen(chosen_part, "r");
        char str[MAX_SIZE], c;
        c = '0';
        while (c != EOF) {
            if (c == '\n') i++;
            c = fgetc(fPtr);
            memset(str, 0, MAX_SIZE);
            if (i == line) {
                if (j < pos) {
                    fputc(c, file);
                    j++;
                } else if (j == pos) {
                    while (k != strlen(clipboard)) {
                        if (clipboard[k] == '\\' && clipboard[k + 1] == 'n') {
                            fputc('\n', file);
                            k += 2;
                        } else if (clipboard[k] == '\\' && clipboard[k + 1] == '\\') {
                            fputc('\\', file);
                            k += 2;
                        } else {
                            fputc(clipboard[k], file);
                            k++;
                        }
                    }
                    fputc(c, file);
                    j++;
                } else if (j > pos) {
                    fputc(c, file);
                    j++;
                }
            } else {
                fputc(c, file);
            }
        }
        printf("Success!\n");
        fclose(file);
        fclose(fPtr);
        file = fopen("root/kiri_project.txt", "r");
        fPtr = fopen(chosen_part, "w");
        i = 0;
        c = fgetc(file);
        while (c != EOF) {
            fputc(c, fPtr);
            c = fgetc(file);
        }
        fclose(file);
        fclose(fPtr);
        remove("root/kiri_project.txt");
    } else printf("file doesn't exist!\n");

}
