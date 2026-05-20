#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILE_NAME "movies.txt"

/* -------- COLORS -------- */
#define GREEN "\033[1;32m"
#define RED   "\033[1;31m"
#define RESET "\033[0m"

struct Movie {
    char title[100];
    char description[200];
    int duration;
};

struct Movie available[MAX] = {
    {"Inception", "A thief enters dreams to steal secrets.", 148},
    {"Interstellar", "A team travels through a wormhole to save humanity.", 169},
    {"The Matrix", "A hacker discovers reality is a simulation.", 136},
    {"The Godfather", "A mafia family struggles with power.", 175},
    {"Avatar", "A marine explores an alien world.", 162}
};
int availableCount = 5;
struct Movie watched[MAX];
int watchedCount = 0;

/* -------- THE FIXES -------- */

void clearScreen() {

    printf("\033[H\033[2J");
    
    
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    printf("\n" GREEN "Press [Enter] to return to menu..." RESET);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    getchar(); 
}

/* -------- DATA HANDLERS -------- */

void loadFromFile() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return;
    watchedCount = 0;
    while (watchedCount < MAX && fscanf(f, "%99[^|]|%199[^|]|%d\n",
                  watched[watchedCount].title,
                  watched[watchedCount].description,
                  &watched[watchedCount].duration) == 3) {
        watchedCount++;
    }
    fclose(f);
}

void saveToFile() {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) return;
    for (int i = 0; i < watchedCount; i++) {
        fprintf(f, "%s|%s|%d\n", watched[i].title, watched[i].description, watched[i].duration);
    }
    fclose(f);
}

/* -------- MAIN PROGRAM -------- */

int main() {
    int choice;
    loadFromFile();

    while (1) {
        clearScreen();
        printf("===============================\n");
        printf("        MOVIE MANAGER         \n");
        printf("===============================\n");
        printf("1. View Available Movies\n");
        printf("2. View Watched Movies\n");
        printf("3. Add Movie to Watched\n");
        printf("4. Delete from Watched\n");
        printf("0. Save & Exit\n");
        printf("-------------------------------\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf(RED "Invalid input! Use numbers only.\n" RESET);
            int c; while ((c = getchar()) != '\n' && c != EOF); 
            pause();
            continue;
        }

        if (choice == 0) {
            saveToFile();
            printf(GREEN "Data saved. Goodbye!\n" RESET);
            break;
        }

        clearScreen(); 

        switch (choice) {
            case 1:
                printf("===== AVAILABLE MOVIES =====\n\n");
                for (int i = 0; i < availableCount; i++) {
                    printf("%d. %s (%d min)\n   %s\n\n", i+1, available[i].title, available[i].duration, available[i].description);
                }
                break;

            case 2:
                printf("===== WATCHED MOVIES =====\n\n");
                if (watchedCount == 0) printf("Your list is empty.\n");
                for (int i = 0; i < watchedCount; i++) {
                    printf("%d. %s (%d min)\n", i+1, watched[i].title, watched[i].duration);
                }
                break;

            case 3:
                printf("===== ADD TO WATCHED =====\n\n");
                for (int i = 0; i < availableCount; i++) printf("%d. %s\n", i+1, available[i].title);
                printf("\nSelect movie number: ");
                int addIdx;
                if (scanf("%d", &addIdx) == 1 && addIdx > 0 && addIdx <= availableCount) {
                    watched[watchedCount++] = available[addIdx-1];
                    saveToFile();
                    printf(GREEN "\nAdded successfully!" RESET);
                } else {
                    printf(RED "\nInvalid selection!" RESET);
                }
                break;

            case 4:
                printf("===== DELETE WATCHED =====\n\n");
                if (watchedCount == 0) {
                    printf("Nothing to delete.\n");
                } else {
                    for (int i = 0; i < watchedCount; i++) printf("%d. %s\n", i+1, watched[i].title);
                    printf("\nSelect number to delete: ");
                    int delIdx;
                    if (scanf("%d", &delIdx) == 1 && delIdx > 0 && delIdx <= watchedCount) {
                        for (int i = delIdx-1; i < watchedCount-1; i++) watched[i] = watched[i+1];
                        watchedCount--;
                        saveToFile();
                        printf(GREEN "\nDeleted successfully!" RESET);
                    }
                }
                break;

            default:
                printf(RED "Option not found." RESET);
                break;
        }

        pause(); 
    }

    return 0;
}
