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
void undo(char *command, char *prev_command);
void auto_indent(char *command);
void auto_enters(char *command);
void auto_tabs(char *command);
void compare(char *command);

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

void undo(char *command, char *prev_command){

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
        chosen_part[j] = command[i];
        i++;
        j++;
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
    int i = 0, j = 0, k= 0, i1= 0, j1=0, line1= 0, line2= 0, start;
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
    while (command[i] != ' ' && command[i + 1] != '-') {
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
        if(i1 == -1 && j1 == -1)return;
        if(strcmp(str1, str2) != 0){
            printf("====== #%d =====\n", line1);
            printf("%s\n", str1);
            printf("%s\n", str2);
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
                            if (ch2 == EOF) return;
                            printf("%c", ch2);
                            k++;
                        }
                    }
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
                            if (ch1 == EOF) return;
                            printf("%c", ch1);
                            k++;
                        }
                    }
                    return;
                }
            }
        }

        i= 0, j= 0, k= 0;
    }
    fclose(file1);
    fclose(file2);
}
