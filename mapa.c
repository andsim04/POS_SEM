//
// Created by andre on 2. 1. 2024.
//

#include "mapa.h"

void mapa_init2(MAPA* mapa, int sirka, int vyska, VIETOR* vietor) {
    mapa->sirka = sirka;
    mapa->vyska = vyska;
    mapa->vietor = vietor;

    mapa->mapa = calloc(vyska, sizeof(BUNKA *));

    for (int i = 0; i < vyska; i++) {
        mapa->mapa[i] = calloc(sirka, sizeof(BUNKA));
    }
    mapa->je_inicializovana = true;
}

void mapa_init(MAPA* mapa, int sirka, int vyska, VIETOR * vietor) {
    mapa->sirka = sirka;
    mapa->vyska = vyska;
    mapa->vietor = vietor;

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
                    bunka_init(&mapa->mapa[i][j], 'P', true, false, false, i, j);
                    break;
                case 2:
                    //biotop hory
                    bunka_init(&mapa->mapa[i][j], 'M', false, false, false, i, j);
                    break;
                case 3:
                    //biotop voda
                    bunka_init(&mapa->mapa[i][j], 'W', false, false, false, i, j);
                    break;
            }
        }
    }
    mapa->je_inicializovana = true;
}

void mapa_vykresli(MAPA mapa) {
    printf("\n   ");

    for (int i = 0; i < mapa.sirka; ++i) {
        if ((i + 1) == mapa.sirka) {
            printf("%d", i);
        } else {
            printf("%d-", i);
        }

    }
    printf("\n");

    for (int i = 0; i < mapa.vyska; i++) {
        printf("%d |",i);
        for (int j = 0; j < mapa.sirka; j++) {
            if ((j + 1) == mapa.sirka) {
                bunka_vykresli(mapa.mapa[i][j]);
                printf("|");
            } else {
                bunka_vykresli(mapa.mapa[i][j]);
                printf("-");
            }

        }
        printf("\n");
    }
    printf("\n");
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
    mapa->je_inicializovana = false;
    mapa = NULL;

}

void mapa_zmen_luka(MAPA* mapa, int x, int y) {
    int count = 0;
    int rng = (rand() % 101) + 1;
    if (x != 0)
        if (mapa->mapa[x - 1][y].biotop == 'W' && rng <= 10)
            count++;

    if (x != mapa->vyska - 1)
        if (mapa->mapa[x + 1][y].biotop == 'W' && rng <= 10)
            count++;

    if (y != 0)
        if (mapa->mapa[x][y - 1].biotop == 'W' && rng <= 10)
            count++;

    if (y != mapa->sirka - 1)
        if (mapa->mapa[x][y + 1].biotop == 'W' && rng <= 10)
            count++;

    if (count > 0) {
        mapa->mapa[x][y].biotop = 'P';
        mapa->mapa[x][y].horlavy = true;
        mapa->mapa[x][y].zhorena = false;
        printf("Zmena lúku: [%d, %d]\n", x,y);
    }
}

void mapa_zmen_les(MAPA* mapa, int x, int y) {
    int count = 0;
    int rng = (rand() % 101) + 1;
    if (x != 0)
        if (mapa->mapa[x - 1][y].biotop == 'F' && rng <= 2)
            count++;

    if (x != mapa->vyska - 1)
        if (mapa->mapa[x + 1][y].biotop == 'F' && rng <= 2)
            count++;
    if (y != 0)
        if (mapa->mapa[x][y - 1].biotop == 'F' && rng <= 2)
            count++;

    if (y != mapa->sirka - 1)
        if (mapa->mapa[x][y + 1].biotop == 'F' && rng <= 2)
            count++;

    if (count > 0) {
        mapa->mapa[x][y].biotop = 'F';
        printf("Zmena les: [%d, %d]\n", x,y);
    }
}

void mapa_rozsir_ohen(MAPA* mapa, int x, int y, int smerVetra) {
    mapa->mapa[x][y].ohen = false;
    mapa->mapa[x][y].zhorena = true;
    mapa->mapa[x][y].biotop = 'X';
    printf("Zhorene policko: [%d, %d]\n", x,y);

    if (x != 0) {
        if (mapa->mapa[x - 1][y].horlavy) {
            if (smerVetra == 1 && ((rand()%101)+1 <= 90)) {
                mapa->mapa[x - 1][y].ohen = true;
                mapa->mapa[x - 1][y].horlavy = false;
                mapa->mapa[x - 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x-1, y);
            } else if ((smerVetra == 0 || smerVetra == 2 || smerVetra == 4) && (rand()%101+1 <= 20)) {
                mapa->mapa[x - 1][y].ohen = true;
                mapa->mapa[x - 1][y].horlavy = false;
                mapa->mapa[x - 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x-1, y);
            } else if (smerVetra == 3 && (rand()%101)+1 <= 2) {
                mapa->mapa[x - 1][y].ohen = true;
                mapa->mapa[x - 1][y].horlavy = false;
                mapa->mapa[x - 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x-1, y);
            }
        }
    }
    if (x != (mapa->vyska - 1)) {
        if (mapa->mapa[x + 1][y].horlavy) {
            if (smerVetra == 3 && ((rand()%101)+1 <= 90)) {
                mapa->mapa[x + 1][y].ohen = true;
                mapa->mapa[x + 1][y].horlavy = false;
                mapa->mapa[x + 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x+1, y);
            } else if ((smerVetra == 0 || smerVetra == 2 || smerVetra == 4) && (rand()%101+1 <= 20)) {
                mapa->mapa[x + 1][y].ohen = true;
                mapa->mapa[x + 1][y].horlavy = false;
                mapa->mapa[x + 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x+1, y);
            } else if (smerVetra == 1 && (rand()%101)+1 <= 2) {
                mapa->mapa[x + 1][y].ohen = true;
                mapa->mapa[x + 1][y].horlavy = false;
                mapa->mapa[x + 1][y].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x+1, y);
            }
        }
    }
    if (y != 0) {
        if (mapa->mapa[x][y - 1].horlavy) {
            if (smerVetra == 4 && (rand()%101+1 <= 90)) {
                mapa->mapa[x][y - 1].ohen = true;
                mapa->mapa[x][y - 1].horlavy = false;
                mapa->mapa[x][y - 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y-1);
            } else if ((smerVetra == 0 || smerVetra == 3 || smerVetra == 1) && (rand()%101+1 <= 20)) {
                mapa->mapa[x][y - 1].ohen = true;
                mapa->mapa[x][y - 1].horlavy = false;
                mapa->mapa[x][y - 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y-1);
            } else if (smerVetra == 2 && rand()%101+1 <= 2) {
                mapa->mapa[x][y - 1].ohen = true;
                mapa->mapa[x][y - 1].horlavy = false;
                mapa->mapa[x][y - 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y-1);
            }
        }
    }
    if (y != (mapa->sirka - 1)) {
        if (mapa->mapa[x][y + 1].horlavy) {
            if (smerVetra == 2 && (rand()%101+1 <= 90)) {
                mapa->mapa[x][y + 1].ohen = true;
                mapa->mapa[x][y + 1].horlavy = false;
                mapa->mapa[x][y + 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y+1);
            } else if ((smerVetra == 0 || smerVetra == 3 || smerVetra == 1) && (rand()%101+1 <= 20)) {
                mapa->mapa[x][y + 1].ohen = true;
                mapa->mapa[x][y + 1].horlavy = false;
                mapa->mapa[x][y + 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y+1);
            } else if (smerVetra == 4 && rand()%101+1 <= 2) {
                mapa->mapa[x][y + 1].ohen = true;
                mapa->mapa[x][y + 1].horlavy = false;
                mapa->mapa[x][y + 1].biotop = 'O';
                printf("Zacalo horiet policko [%d, %d]\n", x, y+1);
            }
        }
    }
}

void vypis_vietor(int smerVetra) {
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
}

void mapa_rucne(MAPA * mapa) {
    int vyska = 0;
    int sirka = 0;
    printf("Zadaj sirku:\n");
    scanf("%d", &sirka);
    printf("Zadaj vysku:\n");
    scanf("%d", &vyska);
    mapa_init(mapa, sirka, vyska, NULL);
    char biotop = ' ';
    for (int i = 0; i < vyska; ++i) {
        for (int j = 0; j < sirka; ++j) {
            while (true) {
                printf("Riadok %d\n", i);
                printf("Stlpec %d\n", j);
                printf("Zadajte biotop : ");
                scanf(" %c", &biotop);

                if (biotop == 'F' || biotop == 'P' || biotop == 'W' || biotop == 'M') {
                    mapa->mapa[i][j].biotop = biotop;
                    mapa->mapa[i][j].ohen  = false;
                    mapa->mapa[i][j].zhorena = false;
                    mapa->mapa[i][j].x = i;
                    mapa->mapa[i][j].y = j;
                    if (biotop == 'P' || biotop == 'F') {
                        mapa->mapa[i][j].horlavy = true;

                    } else {
                        mapa->mapa[i][j].horlavy = false;
                    }
                    break;
                } else {
                    printf("Zadaný biotop bol nespravny!\n");
                    printf("Riadok %d\n", i);
                    printf("Stlpec %d\n", j);
                    printf("Zadajte biotop : ");
                    scanf(" %c", &biotop);

                }
            }
        }
    }


}

void ulozenie_mapy(MAPA mapa, char* nazovSuboru) {
    FILE *f = fopen(nazovSuboru, "a");
    if (f == NULL) {
        perror("Problém s otvorením súboru!\n");
        return;
    }

    for (int i = 0; i < mapa.vyska; ++i) {
        for (int j = 0; j < mapa.sirka; ++j) {
            fprintf(f, "%c", mapa.mapa[i][j].biotop);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    printf("Mapa bola ulozena!\n");
    fclose(f);

}

void nacitanie_mapy(MAPA* mapa, char* nazovSuboru) {
    MAPA nacitane_mapy[100];
    int pocet_map = 0;
    int cislo_mapy = 0;
    FILE *f = fopen(nazovSuboru, "r");
    char znak = ' ';
    int sirka = 0;
    int vyska = 0;
    const unsigned int odpad = 1;
    char prazdnyRiadok[odpad];
    int sirka_mapy = 0;
    int vyska_mapy = 0;
    BUNKA bunky[50][50];
    while (!feof(f)) {
        znak = (char) fgetc(f);
        if (znak == 'P' || znak == 'F' || znak == 'M' || znak == 'W') {
            bunky[vyska][sirka].biotop = znak;
            bunky[vyska][sirka].zhorena = false;
            bunky[vyska][sirka].ohen = false;
            bunky[vyska][sirka].x = vyska;
            bunky[vyska][sirka].y = sirka;
            if (znak == 'F' || znak == 'P') {
                bunky[vyska][sirka].horlavy = true;
            } else {
                bunky[vyska][sirka].horlavy = false;
            }
            sirka++;

        } else if (znak == '\n'  && sirka != 0) {
            fgets(prazdnyRiadok, odpad, f);
            vyska++;

            if (pocet_map == cislo_mapy) {
                sirka_mapy = sirka;
                cislo_mapy++;
            }
            sirka = 0;
        } else if (znak == '\n'  && sirka == 0 && pocet_map != cislo_mapy) {
            vyska_mapy = vyska;
            mapa_init(&nacitane_mapy[pocet_map], sirka_mapy, vyska_mapy, NULL);
            for (int i = 0; i < vyska_mapy; i++) {
                for (int j = 0; j < sirka_mapy; ++j) {
                    nacitane_mapy[pocet_map].mapa[i][j].biotop = bunky[i][j].biotop;
                    nacitane_mapy[pocet_map].mapa[i][j].horlavy = bunky[i][j].horlavy;
                    nacitane_mapy[pocet_map].mapa[i][j].ohen = bunky[i][j].ohen;
                    nacitane_mapy[pocet_map].mapa[i][j].zhorena = bunky[i][j].zhorena;
                    nacitane_mapy[pocet_map].mapa[i][j].x = bunky[i][j].x;
                    nacitane_mapy[pocet_map].mapa[i][j].y = bunky[i][j].y;
                }
            }
            nacitane_mapy[pocet_map].sirka = sirka_mapy;
            nacitane_mapy[pocet_map].vyska = vyska_mapy;
            nacitane_mapy[pocet_map].vietor = NULL;
            vyska = 0;
            sirka = 0;
            vyska_mapy = 0;
            sirka_mapy = 0;
            pocet_map++;
            fgets(prazdnyRiadok, odpad, f);
        } else {
            fgets(prazdnyRiadok, odpad, f);
        }
    }
    fclose(f);
    for (int i = 0; i < pocet_map; ++i) {
        printf("===Cislo mapy %d===\n", i + 1);
        mapa_vykresli(nacitane_mapy[i]);
    }
    int vyber_mapa = 0;

    printf("Vyberte cislo mapy: \n");
    scanf("%d", &vyber_mapa);
    if (vyber_mapa > 0 && vyber_mapa <= pocet_map) {
        if (mapa->je_inicializovana) {
            mapa_destroy(mapa);
        }
        mapa_init2(mapa, nacitane_mapy[vyber_mapa - 1].sirka, nacitane_mapy[vyber_mapa - 1].vyska, NULL);
        for (int i = 0; i < mapa->vyska; ++i) {
            for (int j = 0; j < mapa->sirka; ++j) {
                mapa->mapa[i][j].biotop = nacitane_mapy[vyber_mapa - 1].mapa[i][j].biotop;
                mapa->mapa[i][j].horlavy = nacitane_mapy[vyber_mapa - 1].mapa[i][j].horlavy;
                mapa->mapa[i][j].ohen  = nacitane_mapy[vyber_mapa - 1].mapa[i][j].ohen;
                mapa->mapa[i][j].zhorena = nacitane_mapy[vyber_mapa - 1].mapa[i][j].zhorena;
                mapa->mapa[i][j].x = nacitane_mapy[vyber_mapa - 1].mapa[i][j].x;
                mapa->mapa[i][j].y = nacitane_mapy[vyber_mapa - 1].mapa[i][j].y;

            }
        }

        for (int i = 0; i < pocet_map; ++i) {
            mapa_destroy(&nacitane_mapy[i]);
        }
    } else {
        printf("Mapa s danym cislom neexistuje.\n");
    }

    printf("Mapa cislo %d bola nacitana!\n", vyber_mapa);

}