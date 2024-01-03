//
// Created by andre on 2. 1. 2024.
//


#include "klient.h"

void* simulacia(void* thr_data) {
    SIMULACIA_THREAD_DATA* data = (SIMULACIA_THREAD_DATA*) thr_data;
    VIETOR vietor;
    BUNKA horiace_bunky[data->mapa->sirka*data->mapa->vyska];
    BUNKA zhorene_bunky[data->mapa->sirka*data->mapa->vyska];
    BUNKA luky_bunky[data->mapa->sirka * data->mapa->vyska];
    vietor.trvanie = 0;
    int kolocount = 0;
    while(true) {
        kolocount++;
        printf("================KOLO %d================\n", kolocount);
        int rngVietor = (rand()%101)+1;
        if (vietor.trvanie == 0 && (rngVietor <= 10)) {
            vietor.smer = (rand() % 4) + 1;
            vietor.trvanie = (rand() % 3) + 3;
        } else if (vietor.trvanie == 0 && (rngVietor > 10)){
            vietor.smer = 0;
        }
        vypis_vietor(vietor.smer);
        // krit sekcia
        int luky_bunky_velkost = 0;
        for (int i = 0; i < data->mapa->vyska; i++) {
            for (int j = 0; j < data->mapa->sirka; j++) {
                if (data->mapa->mapa[i][j].biotop == 'P') {
                    luky_bunky[luky_bunky_velkost] = data->mapa->mapa[i][j];
                    luky_bunky_velkost++;
                }
            }
        }

        for (int i = 0; i < luky_bunky_velkost; ++i) {
            mapa_zmen_les(data->mapa, luky_bunky[i].x, luky_bunky[i].y);
        }

        int zhorene_bunky_velkost = 0;
        for (int i = 0; i < data->mapa->vyska; i++) {
            for (int j = 0; j < data->mapa->sirka; j++) {
                if (data->mapa->mapa[i][j].biotop == 'X') {
                    zhorene_bunky[zhorene_bunky_velkost] = data->mapa->mapa[i][j];
                    zhorene_bunky_velkost++;
                }
            }
        }
        for (int i = 0; i < zhorene_bunky_velkost; ++i) {
            mapa_zmen_luka(data->mapa, zhorene_bunky[i].x, zhorene_bunky[i].y);
        }

        int horiace_bunky_velkost = 0;
        for (int i = 0; i < data->mapa->vyska; i++) {
            for (int j = 0; j < data->mapa->sirka; j++) {
                if (data->mapa->mapa[i][j].ohen) {
                    horiace_bunky[horiace_bunky_velkost] = data->mapa->mapa[i][j];
                    horiace_bunky_velkost++;
                }
            }
        }
        for (int i = 0; i < horiace_bunky_velkost; ++i) {
            mapa_rozsir_ohen(data->mapa, horiace_bunky[i].x, horiace_bunky[i].y, vietor.smer);
        }

        //koniec krit sekcie



        if (vietor.trvanie > 0) vietor.trvanie--;
        mapa_vykresli(*data->mapa);
        sleep(5);
    }
}

void je_horlavy(MAPA mapa, int x, int y) {
    if (mapa.mapa[x][y].horlavy) {
        //vlozit do zasobnika a vybrat na zaciatku kola
    } else {
        printf("Bunka na suradniciah %d %d nezacala horiet pretoze je to biotop %c \n", x, y, mapa.mapa[x][y].biotop);
    }
}

int main() {

    srand(time(NULL));
    MAPA mapa;
    mapa_init(&mapa, 10, 6);
    mapa_vykresli(mapa);

    SIMULACIA_THREAD_DATA thread_data;

    thread_data.mapa = &mapa;
    /*
    int zapal1 = rand()%6;
    int zapal2 = rand()%10;
    mapa_rozsir_ohen(&mapa, zapal1, zapal2, 0);
    printf("Zapalene policko: [%d, %d]\n", zapal1, zapal2);
     */
    int zapal1 = 0;
    int zapal2 = 0;
    for (int i = 0; i < 5; ++i) {
        zapal1 = rand()%6;
        zapal2 = rand()%10;
        if (mapa.mapa[zapal1][zapal2].horlavy) {
            mapa_rozsir_ohen(&mapa, zapal1, zapal2, 0);
        }
    }

    simulacia(&thread_data);


    mapa_vykresli(mapa);

    mapa_destroy(&mapa);
    return 0;

}