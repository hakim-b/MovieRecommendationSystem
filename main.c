#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "header_files//struct_types.h"

#define userFilename "/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_data.txt"
#define movieDatabase "/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/movie_database.txt"
#define ratingsMatrixFile "/home/hakim/CLionProjects/MovieRecommendationSystem/input_files/user_ratings.txt"

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
    FILE *file = fopen(movieDatabase, "r");

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

void updateMatrixEntry(const char *filename, int row, int col, float newValue) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int numRows, numCols;
    fscanf(file, "%d %d", &numRows, &numCols);

    // Check if row and col are valid
    if (row < 0 || row >= numRows || col < 0 || col >= numCols) {
        printf("Invalid row or column index!: %d, %d\n", row, col);
        fclose(file);
        return;
    }

    float matrix[numRows][numCols];

    // Read matrix entries
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            fscanf(file, "%f", &matrix[i][j]);
        }
    }

    // Update specified entry
    matrix[row][col] = newValue;

    // Rewind file to beginning and overwrite contents
    rewind(file);
    fprintf(file, "%d %d\n", numRows, numCols);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            fprintf(file, "%.1f ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

float **readMatrixFromFile(const char *filename, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        exit(EXIT_FAILURE);
    }

    // Read rows and columns from the file
    fscanf(file, "%d %d", rows, cols);

    // Allocate memory for the matrix
    float **matrix = (float **) malloc(*rows * sizeof(float *));
    if (matrix == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *rows; i++) {
        matrix[i] = (float *) malloc(*cols * sizeof(float));
        if (matrix[i] == NULL) {
            printf("Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Read matrix elements from the file
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%f", &matrix[i][j]);
        }
    }

    fclose(file);
    return matrix;
}

// Function to free memory allocated for the matrix
void freeMatrix(float **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int choice;

    do {
        printMenu();
        choice = getChoice();

        FILE *userFile = fopen(userFilename, "r");
        FILE *ratingsFile = fopen(ratingsMatrixFile, "r+");
        FILE *movieFile = fopen(movieDatabase, "r");

        struct User registeredUsers[100];
        struct Movie movies[100];
        char usernameForRating[40], usernameForRec[40];
        int inputRating;
        int movieNo;

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
                        userFile = fopen(userFilename, "a");
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

                int userRecordCount = 0;

                while (fscanf(userFile, "%49s %d", registeredUsers[userRecordCount].username,
                              &registeredUsers[userRecordCount].id) == 2) {
                    userRecordCount++;

                    if (numRecords >= 100) {
                        break;
                    }
                }

                fclose(userFile);

                if (movieFile == NULL) {
                    printf("Error opening file\n");
                    return 1;
                }

                int movieRecordCount = 0;

                while (fscanf(movieFile, "%99s %49s %f", movies[movieRecordCount].title, movies[movieRecordCount].genre,
                              &movies[movieRecordCount].rating) == 3) {
                    movieRecordCount++;

                    if (movieRecordCount >= 100) {
                        printf("Maximum number of records\n");
                        break;
                    }
                }

                fclose(movieFile);

                int userIndex;

                do {
                    printf("Enter your username: ");
                    scanf("%s", usernameForRating);
                    clearInputBuffer();

                    for (int i = 0; i < userRecordCount; i++) {
                        if (strcasecmp(usernameForRating, registeredUsers[i].username) == 0) {
                            userIndex = registeredUsers[i].id - 1;
                            break;
                        }
                    }

                    if (!usernameRegistered(usernameForRating, registeredUsers, userRecordCount)) {
                        printf("User not found. Please register first.\n\n");
                        break;
                    } else {
                        printf("***** Movie Database *****\n");
                        displayMovies();

                        do {
                            printf("Enter the number of the movie you want to rate: ");
                            scanf("%d", &movieNo);
                            clearInputBuffer();
                            if (movieNo >= 1 && movieNo <= movieRecordCount) {
                                do {
                                    printf("Enter your rating (1-5): ");
                                    scanf("%d", &inputRating);
                                    clearInputBuffer();

                                    if (inputRating < 1 || inputRating > 5) {
                                        printf("Invalid rating. Please enter a rating between 1 and 5\n");
                                    } else {
                                        updateMatrixEntry(ratingsMatrixFile, userIndex, movieNo - 1,
                                                          (float) inputRating);
                                        printf("Rating recorded successfully\n\n");
                                    }
                                } while (inputRating < 1 || inputRating > 5);
                            }
                        } while (movieNo < 1 || movieNo > movieRecordCount);
                    }
                } while (!usernameRegistered(usernameForRating, registeredUsers, userRecordCount));
                break;
            case 4:
                if (userFile == NULL) {
                    printf("Error opening file\n");
                    return 1;
                }

                int userCount = 0;

                while (fscanf(userFile, "%49s %d", registeredUsers[userCount].username,
                              &registeredUsers[userCount].id) == 2) {
                    userCount++;

                    if (userCount >= 100) {
                        break;
                    }
                }

                fclose(userFile);

                // Open the movie file and read the movie database
                if (movieFile == NULL) {
                    printf("Error opening file\n");
                    return 1;
                }

                int movieCount = 0;

                while (fscanf(movieFile, "%99s %49s %f", movies[movieCount].title, movies[movieCount].genre,
                              &movies[movieCount].rating) == 3) {
                    movieCount++;

                    if (movieCount >= 100) {
                        printf("Maximum number of records\n");
                        break;
                    }
                }

                fclose(movieFile);

                // Get the target user's index
                int targetUserIndex;

                do {
                    printf("Enter your username: ");
                    scanf("%s", usernameForRec);
                    clearInputBuffer();

                    for (int i = 0; i < userCount; i++) {
                        if (strcasecmp(usernameForRec, registeredUsers[i].username) == 0) {
                            targetUserIndex = registeredUsers[i].id - 1;
                            break;
                        }
                    }

                    if (!usernameRegistered(usernameForRec, registeredUsers, userCount)) {
                        printf("User not found. Please register first.\n\n");
                        break;
                    } else {
                        printf("\n***** Recommended Movies *****\n");
                        float **matrix = readMatrixFromFile(ratingsMatrixFile, &userCount, &movieCount);

//                        for (int i = 0; i < userCount; i++) {
//                            for (int j = 0; j < movieCount; ++j) {
//                                printf("%.1f\t", matrix[i][j]);
//                            }
//                            printf("\n");
//                        }

                        // Array to store the average ratings of unrated movies
                        float avgRatings[movieCount];
                        memset(&avgRatings, 0, sizeof(avgRatings));

                        // Iterate over each unrated movie and calculate its average rating
                        for (int j = 0; j < movieCount; j++) {
                            if (matrix[targetUserIndex][j] == 0.0) { // If the target user has not rated this movie
                                int numRatings = 0;
                                float totalRating = (float) 0.0;

                                // Iterate over all users to calculate the average rating for this movie
                                for (int i = 0; i < userCount; i++) {
                                    if (i != targetUserIndex &&
                                        matrix[i][j] != 0.0) { // Exclude the target user's rating
                                        totalRating += matrix[i][j];
                                        numRatings++;
                                    }
                                }

                                // Calculate the average rating for this movie
                                if (numRatings > 0) {
                                    avgRatings[j] = totalRating / (float) numRatings;
                                }
                            }
                        }

                        // Sort the movies based on their average ratings
                        // We'll use a simple bubble sort for demonstration purposes
                        for (int i = 0; i < movieCount - 1; i++) {
                            for (int j = 0; j < movieCount - i - 1; j++) {
                                if (avgRatings[j] < avgRatings[j + 1]) {
                                    // Swap average ratings
                                    float tempRating = avgRatings[j];
                                    avgRatings[j] = avgRatings[j + 1];
                                    avgRatings[j + 1] = tempRating;

                                    // Swap movie indices
                                    struct Movie tempMovie = movies[j];
                                    movies[j] = movies[j + 1];
                                    movies[j + 1] = tempMovie;
                                }
                            }
                        }

                        for (int i = 0; i < 5; i++) {
                            if (avgRatings[i] != 0) {
                                printf("%d. %s (%s) - Predicted Rating: %.1f\n", i + 1, movies[i].title,
                                       movies[i].genre, avgRatings[i]);
                            }
                        }

                        freeMatrix(matrix, userCount);
                    }
                } while (!usernameRegistered(usernameForRec, registeredUsers, userCount));
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