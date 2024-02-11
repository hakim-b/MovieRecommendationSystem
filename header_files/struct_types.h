//
// Created by hakim on 2/8/24.
//

#ifndef MOVIERECCOMENDATIONSYSTEM_STRUCT_TYPES_H
#define MOVIERECCOMENDATIONSYSTEM_STRUCT_TYPES_H

struct User {
    char username[40];
    int id;
};

struct Movie {
    char title[50];
    char genre[20];
    float rating;
};

#endif //MOVIERECCOMENDATIONSYSTEM_STRUCT_TYPES_H
