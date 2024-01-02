//
// Created by andre on 2. 1. 2024.
//
#include <stdbool.h>
#include <stdio.h>
#ifndef POS_SEM_BUNKA_H
#define POS_SEM_BUNKA_H

typedef struct bunka {
    char biotop;
    bool horlavy;
    bool ohen;
    bool zhorena;
} BUNKA;

void bunka_init(BUNKA* bunka, char biotop, _Bool horlavy, _Bool ohen, _Bool zhorena);
void bunka_vykresli(BUNKA bunka);
void bunka_podpal(BUNKA* bunka);
void bunka_zmen(BUNKA* bunka, char biotop);
void bunka_zhor(BUNKA* bunka);


#endif //POS_SEM_BUNKA_H
