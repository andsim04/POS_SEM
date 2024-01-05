//
// Created by andre on 2. 1. 2024.
//

#include "bunka.h"
#include <stdlib.h>
#ifndef POS_SEM_MAPA_H
#define POS_SEM_MAPA_H

typedef struct vietor {
    int trvanie;
    int smer;
} VIETOR;

typedef struct mapa {
    int sirka;
    int vyska;
    bool je_inicializovana;
    BUNKA** mapa;
    VIETOR * vietor;

} MAPA;

void mapa_init(MAPA* mapa, int sirka, int vyska, VIETOR* vietor);
void vytvorenie_mapy_rucne(int x, int y);
void ulozenie_mapy(MAPA mapa, char* nazovSuboru);
void nacitanie_mapy(MAPA* mapa, char* nazovSuboru);
void mapa_vykresli(MAPA mapa);
void mapa_destroy(MAPA* mapa);
void mapa_rozsir_ohen(MAPA* mapa, int x, int y, int smerVetra);
void mapa_zmen_luka(MAPA* mapa, int x, int y);
void mapa_zmen_les(MAPA* mapa, int x, int y);
void vypis_vietor(int smerVetra);
void mapa_rucne(MAPA * mapa);

#endif //POS_SEM_MAPA_H