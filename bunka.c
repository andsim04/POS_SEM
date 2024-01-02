//
// Created by andre on 2. 1. 2024.
//
#include "bunka.h"

void bunka_init(BUNKA* bunka, char biotop, _Bool horlavy, _Bool ohen, bool zhorena) {
    bunka->biotop = biotop;
    bunka->horlavy = horlavy;
    bunka->ohen = ohen;
    bunka->zhorena = zhorena;
}

void bunka_vykresli(BUNKA bunka) {
    if (bunka.ohen) printf("%c", 'O');
    else if (bunka.zhorena) printf("%c", 'X');
    else printf("%c", bunka.biotop);
}

void bunka_podpal(BUNKA* bunka) {
    bunka->ohen = true;
}

void bunka_zmen(BUNKA* bunka, char biotop) {
    bunka->biotop = biotop;
}

void bunka_zhor(BUNKA* bunka) {
    bunka->ohen = false;
    bunka->zhorena = true;
}
