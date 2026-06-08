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

/* -------- UTILITIES -------- */

void clearScreen() {
    printf("\033[H\033[2J");
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause() {
    printf("\nPress [" GREEN "Enter" RESET "] to return to menu...");
    clearBuffer();
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

/* -------- CORE LOGIC FUNCTIONS -------- */

// Extracted View Available Logic - Colors only the title element
void viewAvailable() {
    printf("===== AVAILABLE MOVIES (TO WATCH) =====\n\n");
    for (int i = 0; i < availableCount; i++) {
        printf("%d. " RED "%s" RESET " (%d min)\n   %s\n\n", 
               i + 1, available[i].title, available[i].duration, available[i].description);
    }
}

// Extracted View Watched Logic - Colors only the title element
void viewWatched() {
    printf("===== WATCHED MOVIES =====\n\n");
    if (watchedCount == 0) {
        printf("Your list is empty.\n");
    } else {
        for (int i = 0; i < watchedCount; i++) {
            printf("%d. " GREEN "%s" RESET " (%d min)\n", 
                   i + 1, watched[i].title, watched[i].duration);
        }
    }
}

// Extracted Core Add Logic
void addMovieToWatched() {
    printf("===== ADD TO WATCHED =====\n\n");
    if (watchedCount >= MAX) {
        printf(RED "Error: " RESET "Watched list is full!\n");
        return;
    }
    
    for (int i = 0; i < availableCount; i++) {
        printf("%d. %s\n", i + 1, available[i].title);
    }
    
    // Validation: Explicitly stating expected numerical bounds
    printf("\nSelect movie number (1-%d): ", availableCount);
    int addIdx;
    if (scanf("%d", &addIdx) == 1 && addIdx > 0 && addIdx <= availableCount) {
        watched[watchedCount++] = available[addIdx - 1];
        saveToFile();
        printf("\nMovie updated status to " GREEN "Added successfully!" RESET "\n");
    } else {
        printf("\n" RED "Invalid selection!" RESET " Please use digits in the requested range.\n");
        clearBuffer(); // Keep buffer clean after failure
    }
}

// Extracted Core Delete Logic
void deleteMovieFromWatched() {
    printf("===== DELETE WATCHED =====\n\n");
    if (watchedCount == 0) {
        printf("Nothing to delete.\n");
        return;
    }
    
    for (int i = 0; i < watchedCount; i++) {
        printf("%d. %s\n", i + 1, watched[i].title);
    }
    
    // Validation: Explicitly stating expected numerical bounds
    printf("\nSelect number to delete (1-%d): ", watchedCount);
    int delIdx;
    if (scanf("%d", &delIdx) == 1 && delIdx > 0 && delIdx <= watchedCount) {
        for (int i = delIdx - 1; i < watchedCount - 1; i++) {
            watched[i] = watched[i + 1];
        }
        watchedCount--;
        saveToFile();
        printf("\nMovie updated status to " GREEN "Deleted successfully!" RESET "\n");
    } else {
        printf("\n" RED "Invalid selection!" RESET " Please use digits in the requested range.\n");
        clearBuffer();
    }
}

/* -------- MAIN PROGRAM -------- */

int main() {
    int choice;
    loadFromFile();

    while (1) {
        clearScreen();
        printf("===============================\n");
        printf("         MOVIE MANAGER         \n");
        printf("===============================\n");
        printf("1. View Available Movies\n");
        printf("2. View Watched Movies\n");
        printf("3. Add Movie to Watched\n");
        printf("4. Delete from Watched\n");
        printf("0. Save & Exit\n");
        printf("-------------------------------\n");
        
        // Validation: Explicit format constraint requested in prompt
        printf("Choice (0-4): ");

        if (scanf("%d", &choice) != 1) {
            printf("\n" RED "Invalid input!" RESET " Use integers matching options only.\n");
            clearBuffer();
            printf("\nPress [" GREEN "Enter" RESET "] to try again...");
            getchar();
            continue;
        }
        getchar(); 

        if (choice == 0) {
            saveToFile();
            printf("\nData updated. " GREEN "Goodbye!" RESET "\n");
            break;
        }

        clearScreen(); 

        switch (choice) {
            case 1: viewAvailable(); break;
            case 2: viewWatched(); break;
            case 3: addMovieToWatched(); break;
            case 4: deleteMovieFromWatched(); break;
            default:
                printf(RED "Option not found." RESET " Pick an available option inside range.\n");
                break;
        }

        pause(); 
    }

    return 0;
}
