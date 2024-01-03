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
                    bunka_init(&mapa->mapa[i][j], 'F', true, false, false, i, j);
                    break;
                case 1:
                    //biotop luka
                    bunka_init(&mapa->mapa[i][j], 'P', true, false, false, j, i);
                    break;
                case 2:
                    //biotop hory
                    bunka_init(&mapa->mapa[i][j], 'M', false, false, false, j, i);
                    break;
                case 3:
                    //biotop voda
                    bunka_init(&mapa->mapa[i][j], 'W', false, false, false, j, i);
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

void mapa_zmen_luka(MAPA* mapa, int x, int y) {
    int count = 0;
    int rng = (rand() % 101) + 1;
    if (x != 0)
        if (mapa->mapa[x - 1][y].biotop == 'W' && rng <= 10)
            count++;

    if (x != mapa->vyska)
        if (mapa->mapa[x + 1][y].biotop == 'W' && rng <= 10)
            count++;

    if (y != 0)
        if (mapa->mapa[x][y - 1].biotop == 'W' && rng <= 10)
            count++;

    if (y != mapa->sirka)
        if (mapa->mapa[x][y + 1].biotop == 'W' && rng <= 10)
            count++;

    if (count > 0) {
        mapa->mapa[x][y].biotop = 'P';
        mapa->mapa[x][y].horlavy = true;
        mapa->mapa[x][y].zhorena = false;
    }
}

void mapa_zmen_les(MAPA* mapa, int x, int y) {
    int count = 0;
    int rng = (rand() % 101) + 1;
    if (x != 0)
        if (mapa->mapa[x - 1][y].biotop == 'F' && rng <= 2)
            count++;

    if (x != mapa->vyska)
        if (mapa->mapa[x + 1][y].biotop == 'F' && rng <= 2)
            count++;
    if (y != 0)
        if (mapa->mapa[x][y - 1].biotop == 'F' && rng <= 2)
            count++;

    if (y != mapa->sirka)
        if (mapa->mapa[x][y + 1].biotop == 'F' && rng <= 2)
            count++;

    if (count > 0) mapa->mapa[x][y].biotop = 'F';
}

void mapa_rozsir_ohen(MAPA* mapa, int x, int y, int smerVetra) {
    mapa->mapa[x][y].ohen = false;
    if (mapa->mapa[x][y].horlavy) {
        mapa->mapa[x][y].zhorena = true;
        mapa->mapa[x][y].biotop = 'X';
    }

    switch (smerVetra) {
        case 0:
            printf("Je bezvetrie\n");
            break;
        case 1:
            printf("Vietor duje na sever\n");
            break;
        case 2:
            printf("Vietor duje na východ\n");
            break;
        case 3:
            printf("Vietor duje na juh\n");
            break;
        case 4:
            printf("Vietor duje na západ\n");
            break;
    }

    if (x != 0) {
        if (mapa->mapa[x - 1][y].horlavy) {
            if (smerVetra == 1 && (rand()%101+1 < 90)) {
                mapa->mapa[x - 1][y].ohen = true;
                mapa->mapa[x - 1][y].horlavy = false;
                mapa->mapa[x - 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x-1, y);
            } else if ((smerVetra == 0 || smerVetra == 2 || smerVetra == 4) && (rand()%101+1 < 20)) {
                mapa->mapa[x - 1][y].ohen = true;
                mapa->mapa[x - 1][y].horlavy = false;
                mapa->mapa[x - 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x-1, y);
            } else if (smerVetra == 3 && rand()%101+1 < 2) {
                mapa->mapa[x - 1][y].ohen = true;
                mapa->mapa[x - 1][y].horlavy = false;
                mapa->mapa[x - 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x-1, y);
            }
        }
    }
    if (x != mapa->vyska) {
        if (mapa->mapa[x + 1][y].horlavy) {
            if (smerVetra == 3 && (rand()%101+1 < 90)) {
                mapa->mapa[x + 1][y].ohen = true;
                mapa->mapa[x + 1][y].horlavy = false;
                mapa->mapa[x + 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x+1, y);
            } else if ((smerVetra == 0 || smerVetra == 2 || smerVetra == 4) && (rand()%101+1 < 20)) {
                mapa->mapa[x + 1][y].ohen = true;
                mapa->mapa[x + 1][y].horlavy = false;
                mapa->mapa[x + 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x+1, y);
            } else if (smerVetra == 1 && rand()%101+1 < 2) {
                mapa->mapa[x + 1][y].ohen = true;
                mapa->mapa[x + 1][y].horlavy = false;
                mapa->mapa[x + 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x+1, y);
            }
        }
    }
    if (y != 0) {
        if (mapa->mapa[x][y - 1].horlavy) {
            if (smerVetra == 4 && (rand()%101+1 < 90)) {
                mapa->mapa[x][y - 1].ohen = true;
                mapa->mapa[x][y - 1].horlavy = false;
                mapa->mapa[x][y - 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y-1);
            } else if ((smerVetra == 0 || smerVetra == 3 || smerVetra == 1) && (rand()%101+1 < 20)) {
                mapa->mapa[x][y - 1].ohen = true;
                mapa->mapa[x][y - 1].horlavy = false;
                mapa->mapa[x][y - 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y-1);
            } else if (smerVetra == 2 && rand()%101+1 < 2) {
                mapa->mapa[x][y - 1].ohen = true;
                mapa->mapa[x][y - 1].horlavy = false;
                mapa->mapa[x][y - 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y-1);
            }
        }
    }
    if (y != mapa->sirka) {
        if (mapa->mapa[x][y + 1].horlavy) {
            if (smerVetra == 2 && (rand()%101+1 < 90)) {
                mapa->mapa[x][y + 1].ohen = true;
                mapa->mapa[x][y + 1].horlavy = false;
                mapa->mapa[x][y + 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y+1);
            } else if ((smerVetra == 0 || smerVetra == 3 || smerVetra == 1) && (rand()%101+1 < 20)) {
                mapa->mapa[x][y + 1].ohen = true;
                mapa->mapa[x][y + 1].horlavy = false;
                mapa->mapa[x][y + 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y+1);
            } else if (smerVetra == 4 && rand()%101+1 < 2) {
                mapa->mapa[x][y + 1].ohen = true;
                mapa->mapa[x][y + 1].horlavy = false;
                mapa->mapa[x][y + 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y+1);
            }
        }
    }
}