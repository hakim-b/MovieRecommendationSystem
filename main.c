#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "header_files//struct_types.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printMenu() {
    printf("***** Movie Recommendation System *****\n");
    printf("1. Register User\n");
    printf("2. Display Movies\n");
    printf("3. Rate a Movie\n");
    printf("4. Get Movie Recommendations\n");
    printf("0. Exit\n");
}

int usernameRegistered(const char *username, struct User *registeredUsers, int recordCount) {
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(username, registeredUsers[i].username) == 0) {
            return 1;
        }
    }

    return 0;
}

void displayMovies() {
    FILE *file = fopen("/home/hakim/CLionProjects/MovieReccomendationSystem/input_files/movie_database.txt", "r");

    if (file == NULL) {
        printf("Error opening file\n");
    }

    struct Movie movies[100];
    int recordCount = 0;

    while (fscanf(file, "%99s %49s %f", movies[recordCount].title, movies[recordCount].genre,
                  &movies[recordCount].rating) == 3) {
        recordCount++;

        if (recordCount >= 100) {
            printf("Maximum number of records\n");
            break;
        }
    }

    fclose(file);

    for (int i = 0; i < recordCount; i++) {
        printf("%s\t %s\t %.1f\n", movies[i].title, movies[i].genre, movies[i].rating);
    }
}

int getChoice() {
    char input[100];
    int choice;

    printf("Enter your choice: ");
    fgets(input, sizeof(input), stdin);

    if (sscanf(input, "%d", &choice) != 1) {
        printf("Invalid input. Please enter an integer\n");
        return -1;
    }

    return choice;
}


int main() {
    int choice;

    do {
        printMenu();
        choice = getChoice();

        FILE *userFile = fopen("/home/hakim/CLionProjects/MovieReccomendationSystem/input_files/user_data.txt", "r");
        struct User registeredUsers[100];
        int lastID;

        switch (choice) {
            case 1:
                if (userFile == NULL) {
                    printf("Error opening file\n");
                    return 1;
                }

                int numRecords = 0;

                while (fscanf(userFile, "%49s %d", registeredUsers[numRecords].username,
                              &registeredUsers[numRecords].id) == 2) {
                    numRecords++;
                    lastID = registeredUsers[numRecords].id;

                    if (numRecords >= 100) {
                        break;
                    }
                }

                fclose(userFile);

                char nameToSearch[40];
                do {
                    printf("Enter username for registration: ");
                    scanf("%s", nameToSearch);
                    clearInputBuffer();

                    if (usernameRegistered(nameToSearch, registeredUsers, numRecords)) {
                        printf("Username already registered.\n");
                    } else {
                        printf("User %s is successfully registered\n\n", nameToSearch);
                        userFile = fopen("/home/hakim/CLionProjects/MovieReccomendationSystem/input_files/user_data.txt", "a");
                        char newLine[70];
                        sprintf(newLine, "\n%s %d\n", nameToSearch, lastID + 1);
                        fputs(newLine, userFile);
                        break;
                    }
                } while (usernameRegistered(nameToSearch, registeredUsers, numRecords));

                break;
            case 2:
                displayMovies();
                break;
            case 0:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice\n");
                break;
        }
    } while (1);
}


