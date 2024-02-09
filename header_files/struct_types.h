//
// Created by hakim on 2/8/24.
//

#ifndef MOVIERECCOMENDATIONSYSTEM_STRUCT_TYPES_H
#define MOVIERECCOMENDATIONSYSTEM_STRUCT_TYPES_H

struct User {
    char username[100];
    int id;
};

struct Movie {
    char title[100];
    char genre[50];
    float rating;
};

#endif //MOVIERECCOMENDATIONSYSTEM_STRUCT_TYPES_H
