//
// Created by andre on 2. 1. 2024.
//
#include <stdbool.h>
#include <stdio.h>
#ifndef POS_SEM_BUNKA_H
#define POS_SEM_BUNKA_H



typedef struct bunka {
    int x;
    int y;
    char biotop;
    bool horlavy;
    bool ohen;
    bool zhorena;
} BUNKA;

void bunka_init(BUNKA* bunka, char biotop, _Bool horlavy, _Bool ohen, _Bool zhorena, int x, int y);
void bunka_vykresli(BUNKA bunka);



#endif //POS_SEM_BUNKA_H
