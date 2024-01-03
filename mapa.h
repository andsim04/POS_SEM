//
// Created by andre on 2. 1. 2024.
//

#include "bunka.h"
#include <stdlib.h>
#ifndef POS_SEM_MAPA_H
#define POS_SEM_MAPA_H

typedef struct mapa {
    int sirka;
    int vyska;
    BUNKA** mapa;
    int vietor;

} MAPA;

void mapa_init(MAPA* mapa, int sirka, int vyska);
void mapa_vykresli(MAPA mapa);
void mapa_destroy(MAPA* mapa);
void mapa_rozsir_ohen(MAPA* mapa, int x, int y, int smerVetra);
void mapa_zmen_luka(MAPA* mapa, int x, int y);
void mapa_zmen_les(MAPA* mapa, int x, int y);

#endif //POS_SEM_MAPA_H