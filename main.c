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
        printf("%d. %s\t %s\t %.1f\n", i + 1, movies[i].title, movies[i].genre, movies[i].rating);
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

int countLines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Return -1 to indicate an error
    }

    int lineCount = 0;
    int ch;

    // Count lines until EOF is reached
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lineCount++;
        }
    }

    fclose(file);
    return lineCount + 1; // Add 1 for the last line
}

int main() {
    int choice;

    do {
        printMenu();
        choice = getChoice();

        FILE *userFile = fopen("/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_data.txt", "r");
        FILE *ratingsFile = fopen("/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_ratings.txt",
                                  "r+");

        struct User registeredUsers[100];
        char usernameForRating[40];
        int inputRating;
        int moviePos;

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
                        userFile = fopen(
                                "/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_data.txt", "a");
                        lastID++;
                        char newLine[70];
                        sprintf(newLine, "\n%s %d\n", nameToSearch, lastID);
                        fputs(newLine, userFile);
                        fclose(userFile); // Don't forget to close the file after writing
                        if (ratingsFile == NULL) {
                            printf("Error opening file\n");
                            return 1;
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
                        break;
                    }
                } while (usernameRegistered(nameToSearch, registeredUsers, numRecords));

                break;
            case 2:
                displayMovies();
                break;
            case 3:
                if (userFile == NULL) {
                    printf("Error opening file\n");
                    return 1;
                }

                numRecords = 0;

                while (fscanf(userFile, "%49s %d", registeredUsers[numRecords].username,
                              &registeredUsers[numRecords].id) == 2) {
                    numRecords++;

                    if (numRecords >= 100) {
                        break;
                    }
                }

                fclose(userFile);

                int userIndex = -1; // Define userIndex outside the loop

                do {
                    printf("Enter your username: ");
                    scanf("%s", usernameForRating);
                    clearInputBuffer();

                    for (int i = 0; i < numRecords; i++) {
                        if (strcasecmp(usernameForRating, registeredUsers[i].username) == 0) {
                            userIndex = i;
                            break;
                        }
                    }

                    if (userIndex == -1) {
                        printf("User not found. Please register first.\n\n");
                        break;
                    } else {
                        printf("***** Movie Database *****\n");
                        displayMovies();

                        do {
                            printf("Enter the number of the movie you want to rate: ");
                            scanf("%d", &moviePos);
                            clearInputBuffer();
                            if (moviePos >= 1 && moviePos <= 10) {
                                do {
                                    printf("Enter your rating (1-5): ");
                                    scanf("%d", &inputRating);
                                    clearInputBuffer();

                                    if (inputRating < 1 || inputRating > 5) {
                                        printf("Invalid rating. Please enter a rating between 1 and 5\n");
                                    } else {
                                        // Update the rating in the matrix
                                        ratingsFile = fopen(
                                                "/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_ratings.txt",
                                                "r+");
                                        if (ratingsFile == NULL) {
                                            printf("Error opening file\n");
                                            return 1;
                                        }

                                        int numRows;
                                        fscanf(ratingsFile, "%d", &numRows);

// Move the file pointer to the beginning of the ratings data
                                        fseek(ratingsFile, 0, SEEK_SET);

// Skip the first line containing the number of rows
                                        char buffer[100];
                                        fgets(buffer, sizeof(buffer), ratingsFile);

// Move to the correct row
                                        for (int i = 0; i < userIndex; i++) {
                                            fgets(buffer, sizeof(buffer), ratingsFile);
                                        }

// Move to the correct position in the row
                                        int offset = (moviePos - 1) * 4 + (moviePos - 1);
                                        fseek(ratingsFile, offset,
                                              SEEK_CUR); // Move the file pointer to the correct position in the row
                                        float rating;
                                        fscanf(ratingsFile, "%f", &rating);
                                        fseek(ratingsFile, -4,
                                              SEEK_CUR); // Move the file pointer four positions back to overwrite the space after the rating
                                        fprintf(ratingsFile, " %.1f",
                                                (float) inputRating); // Write the new rating without space before it

                                        fclose(ratingsFile);

                                        printf("Rating recorded successfully\n\n");
                                    }
                                } while (inputRating < 1 || inputRating > 5);
                            }
                        } while (moviePos < 1 || moviePos > 10);
                    }
                } while (0);
                break;
            case 4:

            case 0:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice\n");
                break;
        }
    } while (1);
}


