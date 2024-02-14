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
        if (strcasecmp(username, registeredUsers[i].username) == 0) {
            return 1;
        }
    }

    return 0;
}

void displayMovies() {
    FILE *file = fopen("/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/movie_database.txt", "r");

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

void updateUserRatingsFile(const char *username) {
    FILE *ratingsFile = fopen("/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_ratings.txt", "r+");
    if (ratingsFile == NULL) {
        printf("Error opening file\n");
        return;
    }

    // Determine the number of rows (users) in the ratings matrix
    int numRows;
    fscanf(ratingsFile, "%d", &numRows);

    // Move the file pointer to the end of the file
    fseek(ratingsFile, 0, SEEK_END);

    // Append a new row filled with 0.0s for the new user
    fprintf(ratingsFile, "\n");
    for (int i = 0; i < 10; i++) {
        fprintf(ratingsFile, "0.0 ");
    }

    // Move the file pointer back to update the number of rows
    fseek(ratingsFile, 0, SEEK_SET);
    fprintf(ratingsFile, "%d", numRows + 1);

    fclose(ratingsFile);
}


int main() {
    int choice;

    do {
        printMenu();
        choice = getChoice();

        FILE *userFile = fopen("/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_data.txt", "r");
        struct User registeredUsers[100];

        switch (choice) {
            case 1:
                if (userFile == NULL) {
                    printf("Error opening file\n");
                    return 1;
                }

                int numRecords = 0;
                int lastID;

                while (fscanf(userFile, "%49s %d", registeredUsers[numRecords].username,
                              &registeredUsers[numRecords].id) == 2) {
                    numRecords++;
                    lastID = registeredUsers[numRecords].id;

                    if (numRecords >= 100) {
                        break;
                    }
                }

                lastID = registeredUsers[numRecords - 1].id;
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
                        userFile = fopen("/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_data.txt", "a");
                        lastID++;
                        char newLine[70];
                        sprintf(newLine, "\n%s %d\n", nameToSearch, lastID);
                        fputs(newLine, userFile);
                        fclose(userFile); // Don't forget to close the file after writing
                        updateUserRatingsFile(nameToSearch);
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


