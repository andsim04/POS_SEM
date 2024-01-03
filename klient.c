//
// Created by andre on 2. 1. 2024.
//

#include "klient.h"
#include "mapa.h"

typedef struct vietor {
    int trvanie;
    int smer;
} VIETOR;

typedef struct horenie_thread_data {
    MAPA* mapa;
} HORENIE_THREAD_DATA;

void* simulacia(void* thr_data) {
    HORENIE_THREAD_DATA* data = (HORENIE_THREAD_DATA*) thr_data;
    VIETOR vietor;
    BUNKA horiace_bunky[data->mapa->sirka*data->mapa->vyska];
    BUNKA zhorene_bunky[data->mapa->sirka*data->mapa->vyska];
    BUNKA luky_bunky[data->mapa->sirka * data->mapa->vyska];
    vietor.trvanie = 0;
    while(true) {
        int rngVietor = (rand()%101)+1;
        if (vietor.trvanie == 0 && (rngVietor <= 10)) {
            vietor.smer = (rand() % 4) + 1;
            vietor.trvanie = (rand() % 3) + 3;
        } else if (vietor.trvanie == 0 && (rngVietor > 10)){
            vietor.smer = 0;
        }

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
    }
}
