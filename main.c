#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printmap(int m, int n, char **map){
    for (int i = 0; i < 2*n+1; ++i) {
        printf("=");
    }
    puts("");
    for (int k = 0; k < m; ++k) {
        for (int j = 0; j < n; ++j) {
            printf("|");
            if(map[k][j] == '*') {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("|");
        puts("");
    }
    for (int i = 0; i < 2*n+1; ++i) {
        printf("=");
    }
    puts("");
}

void clear(int m, int n, char **map){
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            map[i][j] = '@';
        }
    }
}

void copyarr(char **map, char **maptemp, int m, int n){
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            map[i][j] = maptemp[i][j];
        }
    }
}

int boundchecky(char ***map, char ***maptemp, int m, int n){
    for (int i = 0; i < n; ++i) {
        if((*map)[m-1][i] == '*'){
            *map = (char **)realloc(*map, (m+1)* sizeof(char *));
            (*map)[m] = (char *)malloc(n* sizeof(char));
            *maptemp = (char **)realloc(*maptemp, (m+1)* sizeof(char *));
            (*maptemp)[m] = (char *)malloc(n* sizeof(char));
            for(int i=0 ; i<n ; i++)
                (*map)[m][i] = (*maptemp)[m][i] = '@';
            m = m + 1;
            break;
        }
    }
    for (int i = 0; i < n; ++i) {
        if((*map)[0][i] == '*'){
            *map = (char **)realloc(*map, (m+1)* sizeof(char *));
            (*map)[m] = (char *)malloc(n* sizeof(char));
            *maptemp = (char **)realloc(*maptemp, (m+1)* sizeof(char *));
            (*maptemp)[m] = (char *)malloc(n* sizeof(char));
            for(int i=0 ; i<n ; i++)
                (*map)[m][i] = (*maptemp)[m][i] = '@';
            m = m + 1;
            for (int k = 0; k < m; ++k) {
                for (int j = 0; j < n; ++j) {
                    if((*map)[k][j] == '*'){
                        (*maptemp)[k+1][j] = (*map)[k][j];
                    }
                }
            }
            copyarr(*map, *maptemp, m, n);
            clear(m, n, *maptemp);
            break;
        }
    }
    return m;
}

int boundcheckx(char **map, char **maptemp, int m, int n){
    for (int i = 0; i < m; ++i) {
        if(map[i][n-1] == '*'){
            for (int j = 0; j < m; ++j) {
                map[j] = (char *)realloc(map[j], (n+1)*sizeof(char));
                map[j][n] = '@';
            }
            for (int j = 0; j < m; ++j) {
                maptemp[j] = (char *)realloc(maptemp[j], (n+1)*sizeof(char));
                maptemp[j][n] = '@';
            }
            n = n + 1;
            break;
        }
    }
    for (int i = 0; i < m; ++i) {
        if (map[i][0] == '*') {
            for (int j = 0; j < m; ++j) {
                map[j] = (char *) realloc(map[j], (n + 1) * sizeof(char));
                map[j][n] = '@';
            }
            for (int j = 0; j < m; ++j) {
                maptemp[j] = (char *) realloc(maptemp[j], (n + 1) * sizeof(char));
                maptemp[j][n] = '@';
            }
            n = n + 1;
            for (int k = 0; k < m; ++k) {
                for (int j = 0; j < n; ++j) {
                    if (map[k][j] == '*') {
                        maptemp[k][j + 1] = map[k][j];
                    }
                }
            }
            copyarr(map, maptemp, m, n);
            clear(m, n, maptemp);
            break;
        }
    }
    return n;
}

int neigh(int x, int y, char **map, int m, int n){
    int i = 0;
    if (x != n-1 && y != m-1) {
        if (map[y + 1][x + 1] == '*') {
            i++;
        }
    }
    if (x != n-1) {
        if (map[y][x + 1] == '*') {
            i++;
        }
    }
    if(y != m-1) {
        if (map[y + 1][x] == '*') {
            i++;
        }
    }
    if(y != 0 && x != n-1) {
        if (map[y - 1][x + 1] == '*') {
            i++;
        }
    }
    if (y != 0) {
        if (map[y-1][x] == '*'){
            i++;
        }
    }
    if(y != m-1 && x != 0) {
        if (map[y + 1][x - 1] == '*') {
            i++;
        }
    }
    if (x != 0) {
        if (map[y][x-1] == '*'){
            i++;
        }
    }
    if(x != 0 && y != 0) {
        if (map[y - 1][x - 1] == '*') {
            i++;
        }
    }
    return i;
}

int main() {
    int n;
    int t;
    printf("Please enter your screen size: ");
    scanf("%d", &n);
    int m = n;
    int k = m;
    printf("Please enter times that you want to simulate: ");
    scanf("%d", &t);
    char **map;
    map = (char **)malloc(m*sizeof(char *));
    for (int i = 0; i < m; ++i) {
        map[i] = (char *)malloc(n*sizeof(char));
    }
    char **maptemp;
    maptemp = (char **)malloc(m*sizeof(char *));
    for (int i = 0; i < m; ++i) {
        maptemp[i] = (char *)malloc(n*sizeof(char));
    }
    clear(m, n, map);
    clear(m, n, maptemp);
    int i = 1;
    int x, y;
    printf("Please enter your %dth cell cordinate: ", i);
    scanf("%d %d", &x, &y);
    while (x != -1 && y != -1){
        i++;
        map[y][x] = '*';
        printf("Please enter your %dth cell cordinate (Enter -1 for one of inputs to end): ", i);
        scanf("%d %d", &x, &y);
    }
    n = boundcheckx(map, maptemp, m, n);
    m = boundchecky(&map, &maptemp, m, n);

    printmap(m, n, map);
    int j = 0;
    while (j != t){
        for (int k = 0; k < m; ++k) {
            for (int l = 0; l < n; ++l) {
                if(map[k][l] == '*'){
                    if(neigh(l, k, map, m, n) < 2){
                        maptemp[k][l] = '@';
                    } else if(neigh(l, k, map, m, n) > 3){
                        maptemp[k][l] = '@';
                    } else {
                        maptemp[k][l] = '*';
                    }
                } else {
                    if(neigh(l, k, map, m, n) == 3){
                        maptemp[k][l] = '*';
                    }
                }
            }
        }
        copyarr(map, maptemp, m, n);
        clear(m, n, maptemp);
        n = boundcheckx(map, maptemp, m, n);
        m = boundchecky(&map, &maptemp, m, n);
        if (n >= 2*k || m >= 2*k){
            printf("You are out of range!!!!\n");
            exit(0);
        }
        sleep(1);
        system("@cls||clear");
        printmap(m, n, map);
        j++;
    }
    return 0;
}