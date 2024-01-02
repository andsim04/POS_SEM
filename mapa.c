//
// Created by andre on 2. 1. 2024.
//

#include "mapa.h"

void mapa_init(MAPA* mapa, int sirka, int vyska) {
    mapa->sirka = sirka;
    mapa->vyska = vyska;

    mapa->mapa = calloc(vyska, sizeof(BUNKA*));

    for (int i = 0; i < vyska; i++) {
        mapa->mapa[i] = calloc(sirka, sizeof(BUNKA));
    }

    for (int i = 0; i < vyska; i++) {
        for (int j = 0; j < sirka; j++) {
            int rngBiotop = rand() % 4;
            switch (rngBiotop) {
                case 0:
                    //biotop les
                    bunka_init(&mapa->mapa[i][j], 'F', true, false, false);
                    break;
                case 1:
                    //biotop luka
                    bunka_init(&mapa->mapa[i][j], 'P', true, false, false);
                    break;
                case 2:
                    //biotop hory
                    bunka_init(&mapa->mapa[i][j], 'M', false, false, false);
                    break;
                case 3:
                    //biotop voda
                    bunka_init(&mapa->mapa[i][j], 'W', false, false, false);
                    break;
            }
        }
    }
}

void mapa_vykresli(MAPA mapa) {
    printf("\n");
    for (int i = 0; i < mapa.vyska; i++) {
        for (int j = 0; j < mapa.sirka; j++) {
            bunka_vykresli(mapa.mapa[i][j]);
        }
        printf("\n");
    }
}

void mapa_destroy(MAPA* mapa) {
    for (int i = 0; i < mapa->vyska; i++) {
        free(mapa->mapa[i]);
        mapa->mapa[i] = NULL;
    }
    free(mapa->mapa);
    mapa->mapa = NULL;

    mapa->sirka = 0;
    mapa->vyska = 0;

}