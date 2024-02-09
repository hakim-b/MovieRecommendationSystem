#include <stdio.h>
#include <string.h>

#include "header_files//struct_types.h"

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

    while (fscanf(file, "%99s %49s %f", movies[recordCount].title, movies[recordCount].genre, &movies[recordCount].rating) == 3) {
        recordCount++;

        if (recordCount >= 100) {
            printf("Maximum number of records\n");
            break;
        }
    }

    fclose(file);

    for (int i = 0; i < recordCount; i++) {
        printf("%s\t %s\t %.1f\n", movies[i].title, movies[i].genre,movies[i].rating);
    }
}

int main() {
    displayMovies();

//    FILE *file = fopen("/home/hakim/CLionProjects/MovieReccomendationSystem/input_files/user_data.txt", "r");
//
//    if (file == NULL) {
//        printf("Error opening file\n");
//        return 1;
//    }
//
//    const int MAX_RECORDS = 100;
//    struct User registeredUsers[MAX_RECORDS];
//    int numRecords = 0;
//
//    while (fscanf(file, "%49s %d", registeredUsers[numRecords].username, &registeredUsers[numRecords].id) == 2) {
//        numRecords++;
//
//        if (numRecords >= MAX_RECORDS) {
//            printf("Maximum number of records reached.\n");
//            break;
//        }
//    }
//
//    fclose(file);
//
//    printf("Read %d records:\n", numRecords);
//    for (int i = 0; i < numRecords; i++) {
//        printf("Name: %s, ID: %d\n", registeredUsers[i].username, registeredUsers[i].id);
//    }
//
//    const char *nameToSearch = "kay";
//    if (usernameRegistered(nameToSearch, registeredUsers, numRecords)) {
//        printf("Name '%s' already exists in the file.\n", nameToSearch);
//    } else {
//        printf("Name '%s' does not exist in the file.\n", nameToSearch);
//    }

    return 0;
}


