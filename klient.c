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

void* horenie(void* thr_data) {
    HORENIE_THREAD_DATA* data = (HORENIE_THREAD_DATA*) thr_data;
    VIETOR vietor;
    BUNKA horiace_bunky[data->mapa->sirka*data->mapa->vyska];
    while(true) {
        int rngVietor = rand()%101+1;
        if (vietor.trvanie == 0 && (rngVietor <= 10)) {
            vietor.smer = (rand() % 4) + 1;
        } else if (vietor.trvanie == 0 && (rngVietor > 10)){
            vietor.smer = 0;
        }

        // krit sekcia
        int velkost = 0;
        for (int i = 0; i < data->mapa->vyska; i++) {
            for (int j = 0; j < data->mapa->sirka; j++) {
                if (data->mapa->mapa[i][j].ohen) {
                    horiace_bunky[velkost] = data->mapa->mapa[i][j];
                    velkost++;
                }
            }
        }
        for (int i = 0; i < velkost; ++i) {
            mapa_rozsir_ohen(data->mapa, horiace_bunky[i].x, horiace_bunky[i].y, vietor.smer);
        }

        //koniec krit sekcie



        if (vietor.trvanie > 0) vietor.trvanie--;
    }
}
