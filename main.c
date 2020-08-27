#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

typedef struct score{
    char PlayerName[30];
    int parameters[3];
} score;

typedef struct save{
    char PlayerName[30];
    int lost;
    int parameters[3];
    int Q_number;
    int NumConst;
    char EndQuestions[100];
    struct questions* Q_list;
} save;

typedef struct questions{
    int count;
    char Question[200];
    char Answer1[200];
    int result1[3];
    char Answer2[200];
    int result2[3];
    int time;
    struct questions* next;
} questions;

int NameSearch(char* PlayerName){
    FILE* NAMES = fopen(".\\NAMES.txt", "r");
    char tmp[30];
    while(1){
        fscanf(NAMES, "%s", tmp);
        if(strcmp(tmp, PlayerName) == 0){
            return 1;
        }
        char c = (char)fgetc(NAMES);
        if(c == EOF){
            break;
        }
        fseek(NAMES, -1, SEEK_CUR);
    }
    return 0;
}

int compare(const void* b,const void* a){
    if(((score*)a)->parameters[0] > ((score*)b)->parameters[0]){
        return 1;
    } else if (((score*)a)->parameters[0] < ((score*)b)->parameters[0]){
        return -1;
    } else {
        if(((score*)a)->parameters[1] > ((score*)b)->parameters[1]){
            return 1;
        } else if(((score*)a)->parameters[1] < ((score*)b)->parameters[1]){
            return -1;
        } else {
            if(((score*)a)->parameters[2] > ((score*)b)->parameters[2]){
                return 1;
            } else if(((score*)a)->parameters[2] < ((score*)b)->parameters[2]){
                return -1;
            } else {
                return 0;
            }
        }
    }
}

void PrintString(char* a){
    for (int i = 0; a[i] != '\0'; ++i) {
        printf("%c", a[i]);
        Sleep(35);
    }
}

void show_Q(questions* Q){
    PrintString(Q->Question);
    printf("\n[1] ");
    PrintString(Q->Answer1);
    puts("");
    printf("[2] ");
    PrintString(Q->Answer2);
    puts("");
    printf(">");
}

save* start_save(save* sv, char PlayerName[], int Q_number, questions* Q_list){
    sv = (save*)malloc(sizeof(save));
    strcpy(sv->PlayerName, PlayerName);
    for (int i = 0; i < 3; ++i) {
        sv->parameters[i] = 50;
    }
    sv->lost = 0;
    sv->Q_number = sv->NumConst = Q_number;
    int k;
    for (k = 0; k < Q_number; ++k) {
        sv->EndQuestions[k] = '3';
    }
    sv->EndQuestions[k] = '\0';
    sv->Q_list = Q_list;
    questions* current = sv->Q_list;
    for (int j = 0; current != NULL; ++j) {
        current->time = 3;
        current = current->next;
    }
    return sv;
}

void save_game(save* sv, int lost){
    if(NameSearch(sv->PlayerName) == 0) {
        FILE *SaveNames = fopen(".\\NAMES.txt", "a");
        fprintf(SaveNames, "%s\n", sv->PlayerName);
        fclose(SaveNames);
    }
    char address[200] = {".\\"};
    strcat(address, sv->PlayerName);
    sv->lost = lost;
    FILE* SaveGame = fopen(address, "wb");
    fwrite(sv, sizeof(save), 1, SaveGame);
    fclose(SaveGame);
}

int deleteNode(questions** head_ref, questions* current){
    if (*head_ref == NULL) {
        return 0;
    }
    questions* temp = *head_ref;
    if (current == *head_ref)
    {
        *head_ref = temp->next;
        free(temp);
        return 1;
    }
    for (int i=0; temp!=NULL && (temp->next != current); i++) {
        temp = temp->next;
    }
    if (temp == NULL || temp->next == NULL) {
        return 0;
    }
    questions* next = temp->next->next;
    free(temp->next);
    temp->next = next;
    return 1;
}

void print_para(int parameters[]){
    printf("People: %d Court: %d Treasury: %d\n", parameters[0], parameters[1], parameters[2]);
}

int menu(){
    printf("\t\t1)New Game   2)Continue   3)ScoreBoard   4)Question Maker\n\n>");
    char c = (char)getch();
    if(c == '1'){
        return 1;
    } else if(c == '2'){
        return 2;
    } else if(c == '3'){
        return 3;
    } else if(c == '4'){
        return 4;
    } else {
        return 0;
    }
}

questions* create_list(char address[200], int p){
    FILE* Q = fopen(address, "r");
    questions* nn = NULL;
    if(Q == NULL){
        printf("Can't open this file!!!\n");
        return nn;
    }
    nn = (questions*)malloc(sizeof(questions));
    nn->count = p;
    fgets(nn->Question, 200, Q);
    fgets(nn->Answer1, 200, Q);
    for (int i = 0; i < 3; ++i) {
        fscanf(Q, "%d", &nn->result1[i]);
    }
    fgetc(Q);
    fgets(nn->Answer2, 200, Q);
    for (int i = 0; i < 3; ++i) {
        fscanf(Q, "%d", &nn->result2[i]);
    }
    nn->time = 3;
    nn->next = NULL;
    return nn;
}

questions* Q_to_List(){
    questions* start = NULL;
    FILE* NAMES = fopen(".\\Project-Files\\CHOICES.txt", "r");
    if(NAMES == NULL){
        printf("Can't open this file!!!!!\n");
        return start;
    }
    int p = 0;
    char temp1[20];
    char address1[100] = {".\\Project-Files\\"};
    fgets(temp1, 20, NAMES);
    temp1[strlen(temp1)-1] = '\0';
    strcat(address1, temp1);
    start = create_list(address1, p);
    questions* current = start;
    p++;
    while (1){
        char temp[20];
        char address[100] = {".\\Project-Files\\"};
        fgets(temp, 20, NAMES);
        if(temp[strlen(temp)-1] == '\n') {
            temp[strlen(temp) - 1] = '\0';
        }
        strcat(address, temp);
        current->next = create_list(address, p);
        char c = (char)fgetc(NAMES);
        current = current->next;
        if(c == EOF){
            break;
        }
        fseek(NAMES, -1, SEEK_CUR);
        p++;
    }
    return start;
}

void game(save* sv){
    srand(time(NULL));
    while(1) {
        questions *current;
        int which, i = 0;
        while(1) {
            which = rand() % sv->NumConst;
            if(sv->EndQuestions[which] == '0'){
                continue;
            }
            current = sv->Q_list;
            while(current->count != which){
                i++;
                current = current->next;
            }
            break;
        }
        system("cls");
        /*questions* showtime = sv->Q_list;
        printf("%d\n", which);
        for (; showtime != NULL; showtime = showtime->next) {
            printf("%d ", showtime->time);
        }
        puts("");
        puts(sv->EndQuestions);
        printf("%d", sv->Q_number);
        puts("");*/
        print_para(sv->parameters);
        show_Q(current);
        char ans = (char) getchar();
        getchar();
        if (ans == '1') {
            for (int k = 0; k < 3; ++k) {
                sv->parameters[k] += current->result1[k];
            }
            for (int j = 0; j < 3; ++j) {
                if (sv->parameters[j] >= 100){
                    sv->parameters[j] = 100;
                }
            }
            current->time--;
            sv->EndQuestions[which]--;
            if(current->time == 0){
                deleteNode(&sv->Q_list, current);
                sv->Q_number--;
            }
            system("cls");
            print_para(sv->parameters);
        } else if (ans == '2') {
            for (int k = 0; k < 3; ++k) {
                sv->parameters[k] += current->result2[k];
            }
            for (int j = 0; j < 3; ++j) {
                if (sv->parameters[j] >= 100){
                    sv->parameters[j] = 100;
                }
            }
            current->time--;
            sv->EndQuestions[which]--;
            if(current->time == 0){
                deleteNode(&sv->Q_list, current);
                sv->Q_number--;
            }
            system("cls");
            print_para(sv->parameters);
        } else {
            printf("\nAre you want to save your progress? [y/n]");
            char key = (char) getch();
            if (key == 'y') {
                save_game(sv, 0);
            }
            break;
        }
        if(sv->parameters[0] < 15 || sv->parameters[1] < 15 || sv->parameters[2] < 15){
            system("color 4f");
        } else {
            system("color Af");
        }
        double avg = 0;
        for (int j = 0; j < 3; ++j) {
            avg += sv->parameters[j];
        }
        avg /= 3;
        if((sv->parameters[0] <= 0 || sv->parameters[1] <= 0 || sv->parameters[2] <= 0) || (avg < 10)){
            printf("GAME OVER\n");
            printf("\nAre you want to save your progress? [y/n]");
            char key = (char) getch();
            if (key == 'y') {
                save_game(sv, 1);
            }
            break;
        }
        if(sv->Q_number == 0){
            sv->Q_number = sv->NumConst;
            sv->Q_list = Q_to_List();
            int j;
            for (j = 0; j < sv->NumConst; ++j) {
                sv->EndQuestions[j] = '3';
            }
            sv->EndQuestions[j] = '\0';
            questions* refind = sv->Q_list;
            for (; refind != NULL; refind = refind->next) {
                refind->time = 3;
            }
        }
    }
}

save* load_game(char PlayerName[]){
    save* sv;
    sv = (save*)malloc(sizeof(save));
    char address[200] = {".\\"};
    strcat(address, PlayerName);
    FILE* ld = fopen(address, "rb");
    if (ld == NULL){
        printf("There is no save with that name!!!\n");
        sv = NULL;
        return sv;
    }
    fread(sv, sizeof(save), 1, ld);
    sv->Q_list = Q_to_List();
    questions* current = sv->Q_list;
    if(sv->lost == 1){
        sv->Q_number = sv->NumConst;
        for (int j = 0; j < sv->NumConst; ++j) {
            current->time = 3;
        }
        for (int k = 0; k < sv->NumConst; ++k) {
            sv->EndQuestions[k] = '3';
        }
        for (int i = 0; i < 3; ++i) {
            sv->parameters[i] = 50;
        }
    } else {
        for (int i = 0; current != NULL; ++i) {
            current->time = sv->EndQuestions[i] - 48;
            current = current->next;
        }
        int k = 0;
        for (int l = 0; l < sv->NumConst; ++l) {
            if (sv->EndQuestions[l] == '0'){
                k++;
            }
        }
        for (int j = 0; j < k; ++j) {
            questions* tmp = sv->Q_list;
            while(tmp != NULL){
                if(tmp->time == 0){
                    deleteNode(&sv->Q_list, tmp);
                    break;
                }
                tmp = tmp->next;
            }
        }
    }
    return sv;
}

void scoreboard(){
    FILE* NAMES = fopen(".\\NAMES.txt", "r");
    score* LIST = (score*)malloc(sizeof(score));
    int i = 0;
    while(1){
        char PlayerName[30];
        char address[200] = {".\\"};
        fscanf(NAMES, "%s", PlayerName);
        int x = strlen(PlayerName);
        strcat(address, PlayerName);
        FILE* player = fopen(address, "rb");
        save* sv;
        sv = (save*)malloc(sizeof(save));
        fread(sv, sizeof(save), 1, player);
        strcpy(LIST[i].PlayerName, sv->PlayerName);
        for (int j = 0; j < 3; ++j) {
            LIST[i].parameters[j] = sv->parameters[j];
        }
        char c = (char)fgetc(NAMES);
        if(c == EOF){
            break;
        }
        i++;
        LIST = (score*)realloc(LIST, (i+1)*sizeof(score));
        fseek(NAMES, -1, SEEK_CUR);
    }
    qsort(LIST, i, sizeof(score), compare);
    printf("-----------------------------\n");
    for (int k = 0; k < i && k < 10; ++k) {
        printf("|%d) ", (k+1));
        printf("%s", LIST[k].PlayerName);
        for (int j = 0; j < (10-strlen(LIST[k].PlayerName)); ++j) {
            printf(" ");
        }
        for (int l = 0; l < 3; ++l) {
            printf("%2d ", LIST[k].parameters[l]);
        }
        puts("");
    }
    printf("-----------------------------\n");
    printf("Press any key...");
    char c = (char)getch();
}

void QMaker(){
    FILE* NAMES = fopen(".\\Project-Files\\CHOICES.txt", "a+");
    char address[50] = {".\\Project-Files\\"};
    printf("Please enter your Question name: ");
    fseek(NAMES, 0, SEEK_END);
    char Name[50];
    gets(Name);
    strcat(address, Name);
    strcat(address, ".txt");
    fprintf(NAMES, "\n%s.txt", Name);
    fclose(NAMES);
    FILE* new_Q = fopen(address, "w");
    char tmp[100];
    printf("Enter your question: ");
    gets(tmp);
    fprintf(new_Q, "%s\n", tmp);
    for (int i = 0; i < 2; ++i) {
        printf("Enter your %d answer: ", i+1);
        gets(tmp);
        fprintf(new_Q, "%s\n", tmp);
        printf("Enter your People parameter impact: ");
        int para;
        scanf("%d", &para);
        fprintf(new_Q, "%d\n", para);
        printf("Enter your Court parameter impact: ");
        scanf("%d", &para);
        fprintf(new_Q, "%d\n", para);
        printf("Enter your Treasury parameter impact: ");
        scanf("%d", &para);
        fprintf(new_Q, "%d\n", para);
        getchar();
    }
    fseek(NAMES, 0, SEEK_END);
    fclose(new_Q);
    printf("Your Question is added!!!\nPress any key...");
    getch();
}

int main() {
    system("color Af");
    char PlayerName[30];
    PrintString("\t\t\t***Welcome to EMPEROR game***\n\n");
    Sleep(1000);
    while(1) {
        system("cls");
        int key = menu();
        system("cls");
        if (key == 1) {
            questions* Q_list = Q_to_List();
            int Q_number = 0;
            for(questions* current = Q_list; current != NULL; Q_number++){
                current = current->next;
            }
            printf("\nPlease enter your name:\n>");
            gets(PlayerName);
            save *first_game = NULL;
            first_game = start_save(first_game, PlayerName, Q_number, Q_list);
            game(first_game);
            break;
        } else if(key == 2){
            printf("\nPlease enter your name:\n>");
            gets(PlayerName);
            save *old_game = load_game(PlayerName);
            if (old_game != NULL) {
                game(old_game);
                break;
            }
        } else if(key == 3){
            scoreboard();
        } else if(key == 4){
            QMaker();
        }
    }
    return 0;
}
