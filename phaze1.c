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
#include <windows.h>

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
void undo(char *command, char *prev_command);
void auto_indent(char *command);
void auto_enters(char *command);
void auto_tabs(char *command);
void compare(char *command);
int one_difference(char str1[MAX_SIZE], char str2[MAX_SIZE]);
void tree(char *command);
void arman(char *command);
void find(char *command);
void replace(char *command);
void grep(char *command);

int main() {
    FILE  *f= fopen("root", "w");
    fclose(f);
    char *clipboard= (char *)malloc(MAX_SIZE * sizeof (char));
    while (1) {
        if (getcommand(clipboard) == 0) printf("invalid command\n");
        else if(getcommand(clipboard) == 2) return 0;
    }
}

void createfile(char *command) {
    int i = 0, j = 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    memset(chosen_part, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (i != strlen(command)) {
        if(command[i]== '\"'){
            i++;
            continue;
        }
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
    char *perv_command = (char *) malloc(MAX_SIZE * sizeof (char));
    memset(perv_command, 0, MAX_SIZE);
    strcpy(perv_command, command);
    memset(command, 0, MAX_SIZE);
    memset(special_part, 0, MAX_SIZE);
    gets(command);
    while (1) {
        if(command[i] == ' ' && command[i + 1] == '-') break;
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
    }else if(strcmp(special_part, "undo") == 0){
        undo(command, perv_command);
        return 1;
    }else if(strcmp(special_part, "auto-indent") == 0){
        auto_indent(command);
        return 1;
    }else if(strcmp(special_part , "compare") == 0){
        compare(command);
        return 1;
    }else if(strcmp(special_part, "tree") == 0){
        tree(command);
        return 1;
    }else if(strcmp(special_part, "arman") == 0){
        arman(command);
        return 1;
    }else if(strcmp(special_part, "find") == 0){
        find(command);
        return 1;
    }else if(strcmp(special_part, "replace") == 0){
        replace(command);
        return 1;
    }else if(strcmp(special_part, "gret") == 0){
        grep(command);
        return 1;
    }else if(strcmp(special_part, "exit") == 0) return 2;
    return 0;
}

void direct(char *chosen_part) {
    int i = 0, slash = 0;
    while (chosen_part[i] != '\0') {
        if (chosen_part[i] == '/') slash++;
        i++;
    }
    if (slash != 0) {
        char *path = (char *) calloc(MAX_SIZE, sizeof(char));
        memset(path, 0, MAX_SIZE);
        for (int j = 0; j < MAX_SIZE; j++) {
            if (chosen_part[j] == '/') {
                mkdir(path);
                slash--;
                if (slash == 0) break;
            }
            path[j] = chosen_part[j];
        }
        memset(path, 0, MAX_SIZE);
    }
}

void insertstr(char *command) {
    int i = 0, j = 0, k = 0, line = 0, pos = 0, quote = 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy = (char *) malloc(MAX_SIZE * sizeof(char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (command[i] != ' ' || command[i + 1] != '-' || (quote%2) == 1) {
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
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
        while (command[i] != ' ' || command[i + 1] != '-') i++;
        i += 7;
        while (command[i] != ' ' || command[i + 1] != '-') i++;
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
        while (command[i] != ' ' && command[i + 1] != '-' || (quote%2) == 1) {
            if(command[i] == '\"'&& command[i- 1]!='\\') {
                i++;
                quote++;
                continue;
            }
            chosen_part[j] = command[i];
            i++;
            j++;
        }
        i = 0, j = 0, k = 0;
        //*********************************************************************************************
        FILE *fhide= fopen("kir_to_insert.txt", "w");
        FILE *fh = fopen(chosen_part_cpy, "r");
        char hide= '0';
        while(hide != EOF){
            hide=fgetc(fh);
            fputc(hide, fhide);
        }
        fclose(fhide);
        fclose(fh);
        int attr = GetFileAttributes("kir_to_insert.txt");
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
        }
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
                        if (chosen_part[k] == '\\' && chosen_part[k + 1] == 'n' && chosen_part[k - 1] != '\\') {
                            fputc('\n', file);
                            k += 2;
                        } else if (chosen_part[k] == '\\' && chosen_part[k + 1] == '\\' && chosen_part[k + 2] != 'n') {
                            fputc('\\', file);
                            k += 2;
                        }else if (chosen_part[k] == '\\' && chosen_part[k + 1] == 'n' && chosen_part[k - 1] == '\\') {
                            fputc('\\', file);
                            fputc('n', file);
                            k += 3;
                        }else {
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
        if(command[i]== '\"'){
            i++;
            continue;
        }
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
    int i = 0, j = 0, k, line = 0, pos = 0, length= 0, max_length = 0, kiram_too_net= 0, quote= 0;
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
    while (command[i] != ' ' && command[i + 1] != '-' || (quote%2) == 1) {
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
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
        FILE *fhide= fopen("kir_to_insert.txt", "w");
        FILE *fh = fopen(chosen_part_cpy, "r");
        char hide= '0';
        while(hide != EOF){
            hide=fgetc(fh);
            fputc(hide, fhide);
        }
        fclose(fhide);
        fclose(fh);
        int attr = GetFileAttributes("kir_to_insert.txt");
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
        }
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
    int i = 0, j = 0, k, line = 0, pos = 0, length= 0, max_length = 0, kiram_too_net= 0, quote= 0;
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
    while (command[i] != ' ' && command[i + 1] != '-' || (quote%2) == 1) {
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
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
    int i = 0, j = 0, k = 0, line = 0, pos = 0, quote= 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy = (char *) malloc(MAX_SIZE * sizeof(char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (command[i] != ' ' && command[i + 1] != '-' || (quote%2)== 1) {
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
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
        FILE *fhide= fopen("kir_to_insert.txt", "w");
        FILE *fh = fopen(chosen_part, "r");
        char hide= '0';
        while(hide != EOF){
            hide=fgetc(fh);
            fputc(hide, fhide);
        }
        fclose(fhide);
        fclose(fh);
        int attr = GetFileAttributes("kir_to_insert.txt");
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
        }
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

void undo(char *command, char *prev_command){
    int attr = GetFileAttributes("kir_to_insert.txt");
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) {
        SetFileAttributes("kir_to_insert.txt", attr & ~FILE_ATTRIBUTE_HIDDEN);
    }
}

void auto_indent(char *command){
    int i = 0, j = 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy= (char *) malloc(MAX_SIZE * sizeof (char ));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (i != strlen(command)) {
        if(command[i]== '\"'){
            i++;
            continue;
        }
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    FILE *fhide= fopen("kir_to_insert.txt", "w");
    FILE *fh = fopen(chosen_part, "r");
    char hide= '0';
    while(hide != EOF){
        hide=fgetc(fh);
        fputc(hide, fhide);
    }
    fclose(fhide);
    fclose(fh);
    int attr = GetFileAttributes("kir_to_insert.txt");
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
    }
    if (access(chosen_part, F_OK) == 0) {
        auto_enters(command);
        auto_tabs(command);
        printf("Success!\n");
    } else printf("file doesn't exist!\n");
}

void auto_enters(char *command){
    int i = 0, j = 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy= (char *) malloc(MAX_SIZE * sizeof (char ));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (i != strlen(command)) {
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    strcpy(chosen_part_cpy, chosen_part);
    memset(chosen_part, 0, MAX_SIZE);
    FILE *fPtr = fopen(chosen_part_cpy, "r");
    i= 0, j= 0;
    char c = fgetc(fPtr);
    while(c != EOF){
        chosen_part[i] = c;
        i++;
        c = fgetc(fPtr);
    }
    int size = strlen(chosen_part);
    for(i= 0; i < strlen(chosen_part); i++){
        if(chosen_part[i] == ' ' && chosen_part[i + 1] == '{') {
            for (int k = i; k < strlen(chosen_part); k++) chosen_part[k] = chosen_part[k + 1];
        }else if(chosen_part[i] == '}' && chosen_part[i + 1] == ' '){
            for(int k=i + 1; k<strlen(chosen_part); k++) chosen_part[k] = chosen_part[k+1];
        }
    }
    for(i=0; i<size; i++){
        if(chosen_part[i - 1] == '{' && chosen_part[i] != '\n'){
            for(j=size; j>i; j--) chosen_part[j] = chosen_part[j-1];
            chosen_part[i] = '\n';
            size++;
        }
        if(chosen_part[i - 1] == '}' && chosen_part[i] != '\n'){
            for(j=size; j>i; j--) chosen_part[j] = chosen_part[j-1];
            chosen_part[i] = '\n';
            size++;
        }
        if(chosen_part[i - 1] == '{' && chosen_part[i] == '}'){
            for(j=size; j>i; j--) chosen_part[j] = chosen_part[j-1];
            chosen_part[i] = '\n';
            size++;
        }
    }
    for(i=1; i<size; i++){
        if(chosen_part[i] == '}' && chosen_part[i-1] != '\n'){
            for(j=size; j>i-1; j--) chosen_part[j] = chosen_part[j-1];
            chosen_part[i] = '\n';
            size++;
        }
        if(chosen_part[i] == '{' && chosen_part[i-1] != ' ' && chosen_part[i-1] != '\n'){
            for(j=size; j>i-1; j--) chosen_part[j] = chosen_part[j-1];
            chosen_part[i] = ' ';
            size++;
        }
    }
    fclose(fPtr);
    fPtr = fopen(chosen_part_cpy, "w");
    fputs(chosen_part, fPtr);
    fclose(fPtr);
}

void auto_tabs(char *command){
    int i = 0, j = 0, counter= 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy= (char *) malloc(MAX_SIZE * sizeof (char ));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (i != strlen(command)) {
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    strcpy(chosen_part_cpy, chosen_part);
    memset(chosen_part, 0, MAX_SIZE);
    FILE *fPtr = fopen(chosen_part_cpy, "r");
    i= 0;
    char c = fgetc(fPtr);
    while(c != EOF){
        chosen_part[i] = c;
        i++;
        c = fgetc(fPtr);
    }
    int length= strlen(chosen_part);
    for(i=0; i< length; i++){
        if(chosen_part[i] == '{') counter += 4;
        if(chosen_part[i - 1] == '\n' && chosen_part[i] == '}') counter -=4;
        if(counter < 0) break;
        if(chosen_part[i - 1] == '\n'){
            j = i;
            while(j < length && chosen_part[j] == ' '){
                for(int k=j; k<strlen(chosen_part); k++) chosen_part[k] = chosen_part[k+1];
                length --;
            }
            length += counter;
            for(j=length; j> i + counter - 1; j--) chosen_part[j] = chosen_part[j - counter];
            for(j=i; j< i + counter; j++) chosen_part[j] = ' ';
        }
    }
    fclose(fPtr);
    fPtr = fopen(chosen_part_cpy, "w");
    fputs(chosen_part, fPtr);
    fclose(fPtr);
}

void compare(char *command){
    int i = 0, j = 0, k= 0, i1= 0, j1=0, line1= 0, line2= 0, start, quote= 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy = (char *) malloc(MAX_SIZE * sizeof(char));
    char *str1= (char *) malloc (MAX_SIZE * sizeof (char));
    char *str2= (char *) malloc (MAX_SIZE * sizeof (char));
    char *all= (char *) malloc (MAX_SIZE *sizeof (char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    memset(str1, 0, MAX_SIZE);
    memset(str2, 0, MAX_SIZE);
    memset(all, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    while (command[i] != ' ' && command[i + 1] != '-' || (quote%2)==1) {
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    strcpy(chosen_part_cpy, chosen_part);
    memset(chosen_part, 0, MAX_SIZE);
    while (command[i] != '/') i++;
    i++;
    j= 0;
    while(i <= strlen(command)){
        if(command[i]== '\"'){
            i++;
            continue;
        }
        chosen_part[j] = command[i];
        j++;
        i++;
    }
    i = 0, j = 0;
    FILE *file1= fopen(chosen_part_cpy, "r");
    FILE *file2= fopen(chosen_part, "r");
    char ch1, ch2;
    /*------------------------------------------------------------*/
    while(1){
        memset(str1, 0, MAX_SIZE);
        memset(str2, 0, MAX_SIZE);
        str1[i]= fgetc(file1);
        while(1){
            if(str1[i] == '\n' || str1[i] == EOF){
                if(str1[i] == EOF) i1= -1;
                str1[i]= '\0';
                line1++;
                break;
            }
            i++;
            str1[i]= fgetc(file1);
        }
        str2[j]= fgetc(file2);
        while(1){
            if(str2[j] == '\n' || str2[j] == EOF){
                if(str2[j] == EOF)j1= -1;
                str2[j]= '\0';
                line2++;
                break;
            }
            j++;
            str2[j]= fgetc(file2);
        }
        if(i1 == -1 && j1 == -1){
            fclose(file1);
            fclose(file2);
            return;
        }
        if(strcmp(str1, str2) != 0){
            printf("====== #%d =====\n", line1);
            int num= one_difference(str1, str2);
            if(num != -1){
                int counter= 0;
                char word1[MAX_SIZE], word2[MAX_SIZE];
                int l= 0, m= 0, n= 0, p= 0, number= 0, check= 0;
                while(1) {
                    memset(word1, 0, MAX_SIZE);
                    while (str1[l] != ' ') {
                        if(str1[l] == '\n' || str1[l] == '\0'){
                            check =1;
                            break;
                        }
                        word1[n] = str1[l];
                        n++;
                        l++;
                    }
                    l++;
                    n= 0;
                    counter ++;
                    if (counter== num)
                        printf("<<%s>> " ,word1);
                    else
                        printf("%s ", word1);
                    if(check == 1)break;
                }
                printf("\n");
                counter= 0;
                check= 0;
                while(1) {
                    memset(word2, 0, MAX_SIZE);
                    while (str2[m] != ' ') {
                        if(str2[m] == '\n' || str2[m] == '\0'){
                            check =1;
                            break;
                        }
                        word2[p] = str2[m];
                        p++;
                        m++;
                    }
                    m++;
                    p= 0;
                    counter ++;
                    if (counter== num)
                        printf("<<%s>> " ,word2);
                    else
                        printf("%s ", word2);
                    if(check == 1)break;
                }
                printf("\n");
            }else {
                printf("%s\n", str1);
                printf("%s\n", str2);
            }
        }
        j= 0;
        i= 0;
        if(i1 == -1){
            start= line2 + 1;
            while(1) {
                str2[j] = fgetc(file2);
                while (1) {
                    if (str2[j] == '\n' || str2[j] == EOF) {
                        if (str2[j] == EOF) j1 = -1;
                        all[k] = str2[j];
                        k++;
                        j++;
                        line2++;
                        break;
                    }
                    all[k] = str2[j];
                    k++;
                    j++;
                    str2[j] = fgetc(file2);
                }
                if (j1 == -1) {
                    k = 0;
                    printf("<<<<< #%d - #%d <<<<<\n", start, line2);
                    for (int l = start; l < line2 + 1; l++) {
                        while (1) {
                            ch2 = all[k];
                            if (ch2 == '\n') {
                                printf("\n");
                                k++;
                                break;
                            }
                            if (ch2 == EOF) {
                                printf("\n");
                                fclose(file1);
                                fclose(file2);
                                return;
                            }
                            printf("%c", ch2);
                            k++;
                        }
                    }
                    printf("\n");
                    fclose(file1);
                    fclose(file2);
                    return;
                }
            }
        }
        j= 0;
        i= 0;
        if(j1 == -1){
            start= line1 + 1;
            while(1) {
                str1[i] = fgetc(file1);
                while (1) {
                    if (str1[i] == '\n' || str1[i] == EOF) {
                        if (str1[i] == EOF) i1 = -1;
                        all[k] = str1[i];
                        k++;
                        i ++;
                        line1++;
                        break;
                    }
                    all[k] = str1[i];
                    k++;
                    i++;
                    str1[i] = fgetc(file1);
                }
                if (i1 == -1) {
                    k = 0;
                    printf(">>>>> #%d - #%d >>>>>\n", start, line1);
                    for (int l = start; l < line1 + 1; l++) {
                        while (1) {
                            ch1 = all[k];
                            if (ch1 == '\n') {
                                printf("\n");
                                k++;
                                break;
                            }
                            if (ch1 == EOF) {
                                printf("\n");
                                fclose(file1);
                                fclose(file2);
                                return;
                            }
                            printf("%c", ch1);
                            k++;
                        }
                    }
                    printf("\n");
                    fclose(file1);
                    fclose(file2);
                    return;
                }
            }
        }

        i= 0, j= 0, k= 0;
    }
}

int one_difference(char str1[MAX_SIZE], char str2[MAX_SIZE]){
    char word1[MAX_SIZE], word2[MAX_SIZE];
    int i= 0, j= 0, k= 0, l= 0, number= 0, check= 0, counter=0;
    while(1) {
        while (str1[i] != ' ') {
            if(str1[i] == '\n' || str1[i] == '\0'){
                check =1;
                break;
            }
            word1[k] = str1[i];
            k++;
            i++;
        }
        i++;
        k= 0;
        while (str2[j] != ' ') {
            if (str2[j] == '\n' || str2[j] == '\0') {
                check = 1;
                break;
            }
            word2[l] = str2[j];
            l++;
            j++;
        }
        j++;
        l = 0;
        if (strcmp(word1, word2) == 0)number++;
        counter ++;
        if(check == 1)break;
    }
    if(number == counter - 1) return counter;
    else return -1;
}

void tree(char *command){
    int i = 0, number= 0;
    i+=8;
    while (command[i] != '\0') {
        number = number * 10 + command[i] - 48;
        i++;
    }
    if(number <-1)printf("invalid depth\n");
    else if(number == 0){
        printf("root:\n");
    }
    else if(number == 1){
        DIR *d;
        int j= 0;
        struct dirent *dir;
        d = opendir("root");
        printf("root:\n");
        if (d){
            while ((dir = readdir(d)) != NULL){
                if(j > 1) printf("    %s\n", dir->d_name);
                j++;
            }
            closedir(d);
        }
    }
}

void arman(char *command){

}

void find(char *command){
    int i = 0, j = 0, k = 0, line = 0, pos = 0, quote= 0, type= 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy= (char *)malloc(MAX_SIZE* sizeof (char));
    char *new_part= (char *)malloc (MAX_SIZE * sizeof (char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    memset(new_part, 0, MAX_SIZE);
    while (command[i] != ' ' && command[i + 1] != '-' || (quote%2)== 1) {
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
        i++;
    }
    i += 7;
    while (command[i] != ' ' && command[i+ 1] != '-' || (quote %2) == 1){
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
        if(command[i] == '\\' && command[i+1] == '*'){
            chosen_part[j]= command[i+1];
            i+=2;
        }else if(command[i- 1] != '\\' && command[i] == '*'){
            if(command[i-1] == ' ' || command[i-1] == '\n' || i==0)type=1;
            else if(command[i+1] == ' ' || command[i+1] == '\n' || command[i+1]== '\0')type= 2;
        }else {
            chosen_part[j] = command[i];
            i++;
        }
        j++;
    }
    strcpy(chosen_part_cpy, chosen_part);
    memset(chosen_part, 0, MAX_SIZE);
    while(command[i] != '/') i++;
    i++;
    j =0;
    while(command[i] != '\0' && (command[i] != ' ' || command[i + 1] != '-')){
        chosen_part[j] = command[i];
        i++;
        j++;
    }if(type==0) {
        if (command[i] == '\0') {
            i = 0;
            j = 0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[i] = getc(fPtr);
                while (str[i] != EOF) {
                    i++;
                    str[i] = getc(fPtr);
                }
                int index = -1;
                for (i = 0; str[i] != '\0'; i++) {
                    index = -1;
                    for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                        if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                            index = -1;
                            break;
                        }
                        if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                            str[i + strlen(chosen_part_cpy)] != '\0') {
                            index = -1;
                            break;
                        }
                        if (str[i + j] != chosen_part_cpy[j]) {
                            index = -1;
                            break;
                        }
                        index = i;
                    }
                    if (index != -1) {
                        break;
                    }
                }
                printf("%d\n", index);
            } else printf("file doesn't exist!\n");
        } else {
            i += 3;
            j = 0;
            while (command[i] != '\0' && command[i] != ' ') {
                new_part[j] = command[i];
                i++;
                j++;
            }
            i++;
            if (strcmp(new_part, "count") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            counter++;
                        }
                    }
                    printf("%d\n", counter);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "at") == 0) {
                int number = 0;
                while (command[i] != '\0') {
                    number = number * 10 + command[i] - 48;
                    i++;
                }
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) counter++;
                        if (counter == number) break;
                    }
                    printf("%d\n", index);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "byword") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) break;
                    }
                    printf("%d\n", word + 1);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "all") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            printf("%d\n", word + 1);
                        }
                    }
                } else printf("file doesn't exist!\n");
            } else {
                printf("Invalid input\n");
            }
        }
    }else if(type==1) {
        if (command[i] == '\0') {
            i = 0;
            j = 0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[i] = getc(fPtr);
                while (str[i] != EOF) {
                    i++;
                    str[i] = getc(fPtr);
                }
                int index = -1;
                for (i = 0; str[i] != '\0'; i++) {
                    index = -1;
                    for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                        if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                            str[i + strlen(chosen_part_cpy)] != '\0') {
                            index = -1;
                            break;
                        }
                        if (str[i + j] != chosen_part_cpy[j]) {
                            index = -1;
                            break;
                        }
                        index = i;
                    }
                    if (index != -1) {
                        break;
                    }
                }
                printf("%d\n", index);
            } else printf("file doesn't exist!\n");
        } else {
            i += 3;
            j = 0;
            while (command[i] != '\0' && command[i] != ' ') {
                new_part[j] = command[i];
                i++;
                j++;
            }
            i++;
            if (strcmp(new_part, "count") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            counter++;
                        }
                    }
                    printf("%d\n", counter);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "at") == 0) {
                int number = 0;
                while (command[i] != '\0') {
                    number = number * 10 + command[i] - 48;
                    i++;
                }
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) counter++;
                        if (counter == number) break;
                    }
                    printf("%d\n", index);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "byword") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) break;
                    }
                    printf("%d\n", word + 1);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "all") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            printf("%d\n", word + 1);
                        }
                    }
                } else printf("file doesn't exist!\n");
            } else {
                printf("Invalid input\n");
            }
        }
    }else if(type == 2){
        if (command[i] == '\0') {
            i = 0;
            j = 0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[i] = getc(fPtr);
                while (str[i] != EOF) {
                    i++;
                    str[i] = getc(fPtr);
                }
                int index = -1;
                for (i = 0; str[i] != '\0'; i++) {
                    index = -1;
                    for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                        if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                            index = -1;
                            break;
                        }
                        if (str[i + j] != chosen_part_cpy[j]) {
                            index = -1;
                            break;
                        }
                        index = i;
                    }
                    if (index != -1) {
                        break;
                    }
                }
                printf("%d\n", index);
            } else printf("file doesn't exist!\n");
        } else {
            i += 3;
            j = 0;
            while (command[i] != '\0' && command[i] != ' ') {
                new_part[j] = command[i];
                i++;
                j++;
            }
            i++;
            if (strcmp(new_part, "count") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            counter++;
                        }
                    }
                    printf("%d\n", counter);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "at") == 0) {
                int number = 0;
                while (command[i] != '\0') {
                    number = number * 10 + command[i] - 48;
                    i++;
                }
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) counter++;
                        if (counter == number) break;
                    }
                    printf("%d\n", index);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "byword") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) break;
                    }
                    printf("%d\n", word + 1);
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "all") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            printf("%d\n", word + 1);
                        }
                    }
                } else printf("file doesn't exist!\n");
            } else {
                printf("Invalid input\n");
            }
        }
    }
}

void replace(char *command){
    int i = 0, j = 0, k = 0, line = 0, pos = 0, quote= 0, type= 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy= (char *)malloc(MAX_SIZE* sizeof (char));
    char *chosen_part_cpy_cpy= (char *)malloc(MAX_SIZE* sizeof (char));
    char *new_part= (char *)malloc (MAX_SIZE * sizeof (char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    memset(chosen_part_cpy_cpy, 0, MAX_SIZE);
    memset(new_part, 0, MAX_SIZE);
    while (command[i] != ' ' && command[i + 1] != '-') {
        i++;
    }
    i += 7;
    while (command[i] != ' ' && command[i+ 1] != '-' || (quote%2)==1){
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
        if(command[i] == '\\' && command[i+1] == '*'){
            chosen_part_cpy[j]= command[i+1];
            i+=2;
        }else if(command[i- 1] != '\\' && command[i] == '*'){
            if(command[i-1] == ' ' || command[i-1] == '\n' || i==0)type=1;
            else if(command[i+1] == ' ' || command[i+1] == '\n' || command[i+1]== '\0')type= 2;
        }else {
            chosen_part_cpy[j] = command[i];
            i++;
        }
        j++;
    }
    i += 7;
    j= 0;
    while (command[i] != ' ' && command[i+ 1] != '-' || (quote % 2) == 1){
        if(command[i] == '\"'&& command[i- 1]!='\\') {
            i++;
            quote++;
            continue;
        }
        if(command[i] == '\\' && command[i+1] == '*'){
            chosen_part_cpy_cpy[j]= command[i+1];
            i+=2;
        }else {
            chosen_part_cpy_cpy[j] = command[i];
            i++;
        }
        j++;
    }
    while(command[i] != '/') i++;
    i++;
    j =0;
    while(command[i] != '\0' && (command[i] != ' ' || command[i + 1] != '-')){
        if(command[i]== '\"'){
            i++;
            continue;
        }
        chosen_part[j] = command[i];
        i++;
        j++;
    }
    if(type== 0) {
        if (command[i] == '\0') {
            i = 0;
            j = 0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fhide= fopen("kir_to_insert.txt", "w");
                FILE *fh = fopen(chosen_part, "r");
                char hide= '0';
                while(hide != EOF){
                    hide=fgetc(fh);
                    fputc(hide, fhide);
                }
                fclose(fhide);
                fclose(fh);
                int attr = GetFileAttributes("kir_to_insert.txt");
                if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                    SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                }
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[0] = getc(fPtr);
                while (str[i] != EOF) {
                    i++;
                    str[i] = getc(fPtr);
                }
                int index = -1;
                for (i = 0; str[i] != '\0'; i++) {
                    index = -1;
                    for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                        if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                            index = -1;
                            break;
                        }
                        if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                            str[i + strlen(chosen_part_cpy)] != '\0') {
                            index = -1;
                            break;
                        }
                        if (str[i + j] != chosen_part_cpy[j]) {
                            index = -1;
                            break;
                        }
                        index = i;
                    }
                    if (index != -1) {
                        break;
                    }
                }
                if (index == -1) {
                    printf("str doesn't exist\n");
                } else {
                    for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                        str[l] = str[l + strlen(chosen_part_cpy)];
                    }
                    char str_cpy[MAX_SIZE];
                    strcpy(str_cpy, str);
                    for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                        str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                    }
                    for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                        str[l + index] = chosen_part_cpy_cpy[l];
                    }
                    FILE *file = fopen(chosen_part, "w");
                    for (int l = 0; l < MAX_SIZE; l++) {
                        if (str[l] == EOF) break;
                        fputc(str[l], file);
                    }
                    fclose(file);
                    printf("Success!\n");
                }
            } else printf("file doesn't exist!\n");
        } else {
            i += 3;
            j = 0;
            while (command[i] != '\0' && command[i] != ' ') {
                new_part[j] = command[i];
                i++;
                j++;
            }
            i++;
            if (strcmp(new_part, "at") == 0) {
                int number = 0;
                while (command[i] != '\0') {
                    number = number * 10 + command[i] - 48;
                    i++;
                }
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fhide= fopen("kir_to_insert.txt", "w");
                    FILE *fh = fopen(chosen_part, "r");
                    char hide= '0';
                    while(hide != EOF){
                        hide=fgetc(fh);
                        fputc(hide, fhide);
                    }
                    fclose(fhide);
                    fclose(fh);
                    int attr = GetFileAttributes("kir_to_insert.txt");
                    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                        SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                    }
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) counter++;
                        if (counter == number) break;
                    }
                    if (index == -1) {
                        printf("str doesn't exist\n");
                    } else {
                        for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                            str[l] = str[l + strlen(chosen_part_cpy)];
                        }
                        char str_cpy[MAX_SIZE];
                        strcpy(str_cpy, str);
                        for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                            str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                        }
                        for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                            str[l + index] = chosen_part_cpy_cpy[l];
                        }
                        FILE *file = fopen(chosen_part, "w");
                        for (int l = 0; l < MAX_SIZE; l++) {
                            if (str[l] == EOF) break;
                            fputc(str[l], file);
                        }
                        fclose(file);
                        printf("Success!\n");
                    }
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "all") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fhide= fopen("kir_to_insert.txt", "w");
                    FILE *fh = fopen(chosen_part, "r");
                    char hide= '0';
                    while(hide != EOF){
                        hide=fgetc(fh);
                        fputc(hide, fhide);
                    }
                    fclose(fhide);
                    fclose(fh);
                    int attr = GetFileAttributes("kir_to_insert.txt");
                    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                        SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                    }
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            if (index == -1) {
                                printf("str doesn't exist\n");
                            } else {
                                for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                                    str[l] = str[l + strlen(chosen_part_cpy)];
                                }
                                char str_cpy[MAX_SIZE];
                                strcpy(str_cpy, str);
                                for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                                    str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                                }
                                for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                                    str[l + index] = chosen_part_cpy_cpy[l];
                                }
                                FILE *file = fopen(chosen_part, "w");
                                for (int l = 0; l < MAX_SIZE; l++) {
                                    if (str[l] == EOF) break;
                                    fputc(str[l], file);
                                }
                                fclose(file);
                                printf("Success!\n");
                            }
                        }
                    }
                } else printf("file doesn't exist!\n");
            } else {
                printf("Invalid input\n");
            }
        }
    }else if(type == 1){
        if (command[i] == '\0') {
            i = 0;
            j = 0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fhide= fopen("kir_to_insert.txt", "w");
                FILE *fh = fopen(chosen_part, "r");
                char hide= '0';
                while(hide != EOF){
                    hide=fgetc(fh);
                    fputc(hide, fhide);
                }
                fclose(fhide);
                fclose(fh);
                int attr = GetFileAttributes("kir_to_insert.txt");
                if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                    SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                }
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[0] = getc(fPtr);
                while (str[i] != EOF) {
                    i++;
                    str[i] = getc(fPtr);
                }
                int index = -1;
                for (i = 0; str[i] != '\0'; i++) {
                    index = -1;
                    for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                        if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                            str[i + strlen(chosen_part_cpy)] != '\0') {
                            index = -1;
                            break;
                        }
                        if (str[i + j] != chosen_part_cpy[j]) {
                            index = -1;
                            break;
                        }
                        index = i;
                    }
                    if (index != -1) {
                        break;
                    }
                }
                if (index == -1) {
                    printf("str doesn't exist\n");
                } else {
                    for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                        str[l] = str[l + strlen(chosen_part_cpy)];
                    }
                    char str_cpy[MAX_SIZE];
                    strcpy(str_cpy, str);
                    for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                        str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                    }
                    for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                        str[l + index] = chosen_part_cpy_cpy[l];
                    }
                    FILE *file = fopen(chosen_part, "w");
                    for (int l = 0; l < MAX_SIZE; l++) {
                        if (str[l] == EOF) break;
                        fputc(str[l], file);
                    }
                    fclose(file);
                    printf("Success!\n");
                }
            } else printf("file doesn't exist!\n");
        } else {
            i += 3;
            j = 0;
            while (command[i] != '\0' && command[i] != ' ') {
                new_part[j] = command[i];
                i++;
                j++;
            }
            i++;
            if (strcmp(new_part, "at") == 0) {
                int number = 0;
                while (command[i] != '\0') {
                    number = number * 10 + command[i] - 48;
                    i++;
                }
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fhide= fopen("kir_to_insert.txt", "w");
                    FILE *fh = fopen(chosen_part, "r");
                    char hide= '0';
                    while(hide != EOF){
                        hide=fgetc(fh);
                        fputc(hide, fhide);
                    }
                    fclose(fhide);
                    fclose(fh);
                    int attr = GetFileAttributes("kir_to_insert.txt");
                    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                        SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                    }
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) counter++;
                        if (counter == number) break;
                    }
                    if (index == -1) {
                        printf("str doesn't exist\n");
                    } else {
                        for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                            str[l] = str[l + strlen(chosen_part_cpy)];
                        }
                        char str_cpy[MAX_SIZE];
                        strcpy(str_cpy, str);
                        for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                            str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                        }
                        for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                            str[l + index] = chosen_part_cpy_cpy[l];
                        }
                        FILE *file = fopen(chosen_part, "w");
                        for (int l = 0; l < MAX_SIZE; l++) {
                            if (str[l] == EOF) break;
                            fputc(str[l], file);
                        }
                        fclose(file);
                        printf("Success!\n");
                    }
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "all") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fhide= fopen("kir_to_insert.txt", "w");
                    FILE *fh = fopen(chosen_part, "r");
                    char hide= '0';
                    while(hide != EOF){
                        hide=fgetc(fh);
                        fputc(hide, fhide);
                    }
                    fclose(fhide);
                    fclose(fh);
                    int attr = GetFileAttributes("kir_to_insert.txt");
                    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                        SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                    }
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i + strlen(chosen_part_cpy)] != '\n' && str[i + strlen(chosen_part_cpy)] != ' ' &&
                                str[i + strlen(chosen_part_cpy)] != '\0') {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            if (index == -1) {
                                printf("str doesn't exist\n");
                            } else {
                                for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                                    str[l] = str[l + strlen(chosen_part_cpy)];
                                }
                                char str_cpy[MAX_SIZE];
                                strcpy(str_cpy, str);
                                for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                                    str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                                }
                                for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                                    str[l + index] = chosen_part_cpy_cpy[l];
                                }
                                FILE *file = fopen(chosen_part, "w");
                                for (int l = 0; l < MAX_SIZE; l++) {
                                    if (str[l] == EOF) break;
                                    fputc(str[l], file);
                                }
                                fclose(file);
                                printf("Success!\n");
                            }
                        }
                    }
                } else printf("file doesn't exist!\n");
            } else {
                printf("Invalid input\n");
            }
        }
    }else if(type == 2){
        if (command[i] == '\0') {
            i = 0;
            j = 0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fhide= fopen("kir_to_insert.txt", "w");
                FILE *fh = fopen(chosen_part, "r");
                char hide= '0';
                while(hide != EOF){
                    hide=fgetc(fh);
                    fputc(hide, fhide);
                }
                fclose(fhide);
                fclose(fh);
                int attr = GetFileAttributes("kir_to_insert.txt");
                if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                    SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                }
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[0] = getc(fPtr);
                while (str[i] != EOF) {
                    i++;
                    str[i] = getc(fPtr);
                }
                int index = -1;
                for (i = 0; str[i] != '\0'; i++) {
                    index = -1;
                    for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                        if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                            index = -1;
                            break;
                        }
                        if (str[i + j] != chosen_part_cpy[j]) {
                            index = -1;
                            break;
                        }
                        index = i;
                    }
                    if (index != -1) {
                        break;
                    }
                }
                if (index == -1) {
                    printf("str doesn't exist\n");
                } else {
                    for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                        str[l] = str[l + strlen(chosen_part_cpy)];
                    }
                    char str_cpy[MAX_SIZE];
                    strcpy(str_cpy, str);
                    for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                        str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                    }
                    for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                        str[l + index] = chosen_part_cpy_cpy[l];
                    }
                    FILE *file = fopen(chosen_part, "w");
                    for (int l = 0; l < MAX_SIZE; l++) {
                        if (str[l] == EOF) break;
                        fputc(str[l], file);
                    }
                    fclose(file);
                    printf("Success!\n");
                }
            } else printf("file doesn't exist!\n");
        } else {
            i += 3;
            j = 0;
            while (command[i] != '\0' && command[i] != ' ') {
                new_part[j] = command[i];
                i++;
                j++;
            }
            i++;
            if (strcmp(new_part, "at") == 0) {
                int number = 0;
                while (command[i] != '\0') {
                    number = number * 10 + command[i] - 48;
                    i++;
                }
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fhide= fopen("kir_to_insert.txt", "w");
                    FILE *fh = fopen(chosen_part, "r");
                    char hide= '0';
                    while(hide != EOF){
                        hide=fgetc(fh);
                        fputc(hide, fhide);
                    }
                    fclose(fhide);
                    fclose(fh);
                    int attr = GetFileAttributes("kir_to_insert.txt");
                    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                        SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                    }
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) counter++;
                        if (counter == number) break;
                    }
                    if (index == -1) {
                        printf("str doesn't exist\n");
                    } else {
                        for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                            str[l] = str[l + strlen(chosen_part_cpy)];
                        }
                        char str_cpy[MAX_SIZE];
                        strcpy(str_cpy, str);
                        for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                            str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                        }
                        for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                            str[l + index] = chosen_part_cpy_cpy[l];
                        }
                        FILE *file = fopen(chosen_part, "w");
                        for (int l = 0; l < MAX_SIZE; l++) {
                            if (str[l] == EOF) break;
                            fputc(str[l], file);
                        }
                        fclose(file);
                        printf("Success!\n");
                    }
                } else printf("file doesn't exist!\n");
            } else if (strcmp(new_part, "all") == 0) {
                i = 0, j = 0;
                if (access(chosen_part, F_OK) == 0) {
                    FILE *fhide= fopen("kir_to_insert.txt", "w");
                    FILE *fh = fopen(chosen_part, "r");
                    char hide= '0';
                    while(hide != EOF){
                        hide=fgetc(fh);
                        fputc(hide, fhide);
                    }
                    fclose(fhide);
                    fclose(fh);
                    int attr = GetFileAttributes("kir_to_insert.txt");
                    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                        SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                    }
                    FILE *fPtr = fopen(chosen_part, "r");
                    char str[MAX_SIZE];
                    str[0] = getc(fPtr);
                    while (str[i] != EOF) {
                        i++;
                        str[i] = getc(fPtr);
                    }
                    i = 0;
                    int index = -1, counter = 0, word = 0;
                    for (i = 0; str[i] != '\0'; i++) {
                        if (str[i] == ' ' || str[i] == '\n') word++;
                        index = -1;
                        for (j = 0; chosen_part_cpy[j] != '\0'; j++) {
                            if (str[i - 1] != ' ' && str[i - 1] != '\n' && i != 0) {
                                index = -1;
                                break;
                            }
                            if (str[i + j] != chosen_part_cpy[j]) {
                                index = -1;
                                break;
                            }
                            index = i;
                        }
                        if (index != -1) {
                            if (index == -1) {
                                printf("str doesn't exist\n");
                            } else {
                                for (int l = index; l < strlen(str) - strlen(chosen_part_cpy); l++) {
                                    str[l] = str[l + strlen(chosen_part_cpy)];
                                }
                                char str_cpy[MAX_SIZE];
                                strcpy(str_cpy, str);
                                for (int l = index; l < strlen(str) - strlen(chosen_part_cpy_cpy); l++) {
                                    str[l + strlen(chosen_part_cpy_cpy)] = str_cpy[l];
                                }
                                for (int l = 0; l < strlen(chosen_part_cpy_cpy); l++) {
                                    str[l + index] = chosen_part_cpy_cpy[l];
                                }
                                FILE *file = fopen(chosen_part, "w");
                                for (int l = 0; l < MAX_SIZE; l++) {
                                    if (str[l] == EOF) break;
                                    fputc(str[l], file);
                                }
                                fclose(file);
                                printf("Success!\n");
                            }
                        }
                    }
                } else printf("file doesn't exist!\n");
            } else {
                printf("Invalid input\n");
            }
        }
    }
}

void grep(char *command){
    int i = 0, j = 0, k = 0, quote= 0, type= 0;
    char *chosen_part = (char *) malloc(MAX_SIZE * sizeof(char));
    char *chosen_part_cpy= (char *)malloc(MAX_SIZE* sizeof (char));
    char *new_part= (char *)malloc (MAX_SIZE * sizeof (char));
    memset(chosen_part, 0, MAX_SIZE);
    memset(chosen_part_cpy, 0, MAX_SIZE);
    memset(new_part, 0, MAX_SIZE);
    while (command[i] != ' ' && command[i + 1] != '-') {
        i++;
    }
    i += 3;
    if(command[i]== 's'){
        i+=4;
        while (command[i] != ' ' && command[i+ 1] != '-' || (quote % 2) == 1){
            if(command[i] == '\"'&& command[i- 1]!='\\') {
                i++;
                quote++;
                continue;
            }
            if(command[i] == '\\' && command[i+1] == '*'){
                chosen_part_cpy[j]= command[i+1];
                i+=2;
            }else if(command[i- 1] != '\\' && command[i] == '*'){
                if(command[i-1] == ' ' || command[i-1] == '\n' || i==0)type=1;
                else if(command[i+1] == ' ' || command[i+1] == '\n' || command[i+1]== '\0')type= 2;
            }else {
                chosen_part_cpy[j] = command[i];
                i++;
            }
            j++;
        }
        while(command[i] != '/') i++;
        i++;
        j= 0;
        while(command[i - 1] != '\0') {
            j= 0;
            while (command[i] != ' ' && command[i] != '\0' || (quote%2)== 1) {
                if(command[i] == '\"'&& command[i- 1]!='\\') {
                    i++;
                    quote++;
                    continue;
                }
                chosen_part[j] = command[i];
                i++;
                j++;
            }
            i+=2;
            j=0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fhide= fopen("kir_to_insert.txt", "w");
                FILE *fh = fopen(chosen_part, "r");
                char hide= '0';
                while(hide != EOF){
                    hide=fgetc(fh);
                    fputc(hide, fhide);
                }
                fclose(fhide);
                fclose(fh);
                int attr = GetFileAttributes("kir_to_insert.txt");
                if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                    SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                }
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[j]= getc(fPtr);
                while (str[j] != EOF){
                    j++;
                    str[j]= getc(fPtr);
                }
                int index = -1, line = 0, count_line= 0;
                for (j = 0; str[j] != '\0'; j++) {
                    if(str[j] == '\n') line++;
                    index = -1;
                    for (k = 0; chosen_part_cpy[k] != '\0'; k++) {
                        if(str[k- 1]!=' ' && str[k- 1] != '\n' && k !=0){
                            index = -1;
                            break;
                        }
                        if(str[k + strlen(chosen_part_cpy) ] != '\n' && str[k + strlen(chosen_part_cpy) ] != ' ' && str[k + strlen(chosen_part_cpy) ] != '\0'){
                            index = -1;
                            break;
                        }
                        if (str[j + k] != chosen_part_cpy[k]) {
                            index = -1;
                            break;
                        }
                        index = j;
                    }
                    if (index != -1) {
                        count_line= 0;
                        int l= 0;
                        printf("%s:  ", chosen_part);
                        while (count_line != line) {
                            for (l = 0; str[l] != '\0'; l++) {
                                if (str[l] == '\n') count_line++;
                                if(count_line == line) break;
                            }
                        }
                        if(line != 0) l++;
                        while (str[l] != '\n' && str[l] != '\0') {
                            printf("%c", str[l]);
                            l++;
                        }
                        printf("\n");
                    }
                }
            } else printf("file doesn't exist!\n");
        }
    }else if(command[i]== 'C') {
        int counter= 0;
        i += 8;
        while (command[i] != ' ' && command[i + 1] != '-' || (quote % 2) == 1) {
            if(command[i] == '\"'&& command[i- 1]!='\\') {
                i++;
                quote++;
                continue;
            }
            if (command[i] == '\\' && command[i + 1] == '*') {
                chosen_part_cpy[j] = command[i + 1];
                i += 2;
            } else {
                chosen_part_cpy[j] = command[i];
                i++;
            }
            j++;
        }
        while (command[i] != '/') i++;
        i++;
        j = 0;
        while (command[i - 1] != '\0') {
            j = 0;
            while (command[i] != ' ' && command[i] != '\0' || (quote%2)==1) {
                if(command[i] == '\"'&& command[i- 1]!='\\') {
                    i++;
                    quote++;
                    continue;
                }
                chosen_part[j] = command[i];
                i++;
                j++;
            }
            i += 2;
            j = 0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fhide= fopen("kir_to_insert.txt", "w");
                FILE *fh = fopen(chosen_part, "r");
                char hide= '0';
                while(hide != EOF){
                    hide=fgetc(fh);
                    fputc(hide, fhide);
                }
                fclose(fhide);
                fclose(fh);
                int attr = GetFileAttributes("kir_to_insert.txt");
                if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                    SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                }
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[j] = getc(fPtr);
                while (str[j] != EOF) {
                    j++;
                    str[j] = getc(fPtr);
                }
                int index = -1, line = 0;
                for (j = 0; str[j] != '\0'; j++) {
                    if (str[j] == '\n') line++;
                    index = -1;
                    for (k = 0; chosen_part_cpy[k] != '\0'; k++) {
                        if (str[j + k] != chosen_part_cpy[k]) {
                            index = -1;
                            break;
                        }
                        index = j;
                    }
                    if (index != -1) counter ++;
                }
            } else printf("file doesn't exist!\n");
        }
        printf("%d\n", counter);
    }else if(command[i]== 'I'){
        i+=8;
        while (command[i] != ' ' && command[i+ 1] != '-' || (quote%2)== 1){
            if(command[i] == '\"'&& command[i- 1]!='\\') {
                i++;
                quote++;
                continue;
            }
            if(command[i] == '\\' && command[i+1] == '*'){
                chosen_part_cpy[j]= command[i+1];
                i+=2;
            }else {
                chosen_part_cpy[j] = command[i];
                i++;
            }
            j++;
        }
        while(command[i] != '/') i++;
        i++;
        j= 0;
        while(command[i - 1] != '\0') {
            j= 0;
            while (command[i] != ' ' && command[i] != '\0' || (quote%2) == 1) {
                if(command[i] == '\"'&& command[i- 1]!='\\') {
                    i++;
                    quote++;
                    continue;
                }
                chosen_part[j] = command[i];
                i++;
                j++;
            }
            i+=2;
            j=0;
            if (access(chosen_part, F_OK) == 0) {
                FILE *fhide= fopen("kir_to_insert.txt", "w");
                FILE *fh = fopen(chosen_part, "r");
                char hide= '0';
                while(hide != EOF){
                    hide=fgetc(fh);
                    fputc(hide, fhide);
                }
                fclose(fhide);
                fclose(fh);
                int attr = GetFileAttributes("kir_to_insert.txt");
                if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                    SetFileAttributes("kir_to_insert.txt", attr | FILE_ATTRIBUTE_HIDDEN);
                }
                FILE *fPtr = fopen(chosen_part, "r");
                char str[MAX_SIZE];
                str[j]= getc(fPtr);
                while (str[j] != EOF){
                    j++;
                    str[j]= getc(fPtr);
                }
                int index = -1, line = 0, count_line= 0;
                for (j = 0; str[j] != '\0'; j++) {
                    if(str[j] == '\n') line++;
                    index = -1;
                    for (k = 0; chosen_part_cpy[k] != '\0'; k++) {
                        if(str[k- 1]!=' ' && str[- 1] != '\n' && k !=0){
                            index = -1;
                            break;
                        }
                        if(str[k + strlen(chosen_part_cpy) ] != '\n' && str[k + strlen(chosen_part_cpy) ] != ' ' && str[k + strlen(chosen_part_cpy) ] != '\0'){
                            index = -1;
                            break;
                        }
                        if (str[j + k] != chosen_part_cpy[k]) {
                            index = -1;
                            break;
                        }
                        index = j;
                    }
                    if (index != -1) {
                        printf("%s\n", chosen_part);
                        break;
                    }
                }
            } else printf("file doesn't exist!\n");
        }
    }
}
