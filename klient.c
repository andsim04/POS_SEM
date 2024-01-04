//
// Created by andre on 2. 1. 2024.
//

#include "klient.h"


void *simulacia(void *thr_data) {
    SIMULACIA_THREAD_DATA *data = (SIMULACIA_THREAD_DATA *) thr_data;
    BUNKA horiace_bunky[data->mapa->sirka * data->mapa->vyska];
    BUNKA zhorene_bunky[data->mapa->sirka * data->mapa->vyska];
    BUNKA luky_bunky[data->mapa->sirka * data->mapa->vyska];
    int kolocount = 0;
    while (true) {
        kolocount++;
        pthread_mutex_lock(data->mapa_mutex);
        while (!data->je_pozastavena) {
            pthread_cond_wait(data->bezi, data->mapa_mutex);
        }


        printf("================KOLO %d================\n", kolocount);
        int rngVietor = (rand() % 101) + 1;
        if (data->vietor->trvanie == 0 && (rngVietor <= 10)) {
            data->vietor->smer = (rand() % 4) + 1;
            data->vietor->trvanie = (rand() % 3) + 3;
        } else if (data->vietor->trvanie == 0 && (rngVietor > 10)) {
            data->vietor->smer = 0;
        }
        vypis_vietor(data->vietor->smer);
        // krit sekcia

        int luky_bunky_velkost = 0;
        for (int i = 0; i < data->mapa->vyska; i++) {
            for (int j = 0; j < data->mapa->sirka; j++) {
                if (data->mapa->mapa[i][j].biotop == 'P') {
                    luky_bunky[luky_bunky_velkost].y = data->mapa->mapa[i][j].y;
                    luky_bunky[luky_bunky_velkost].x = data->mapa->mapa[i][j].x;
                    luky_bunky[luky_bunky_velkost].biotop = data->mapa->mapa[i][j].biotop;
                    luky_bunky[luky_bunky_velkost].horlavy = data->mapa->mapa[i][j].horlavy;
                    luky_bunky[luky_bunky_velkost].zhorena = data->mapa->mapa[i][j].zhorena;
                    luky_bunky[luky_bunky_velkost].ohen = data->mapa->mapa[i][j].ohen;

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
                if (data->mapa->mapa[i][j].biotop == 'O') {
                    horiace_bunky[horiace_bunky_velkost] = data->mapa->mapa[i][j];
                    horiace_bunky_velkost++;
                }
            }
        }
        for (int i = 0; i < horiace_bunky_velkost; ++i) {
            mapa_rozsir_ohen(data->mapa, horiace_bunky[i].x, horiace_bunky[i].y, data->vietor->smer);
        }

        if (data->vietor->trvanie > 0) data->vietor->trvanie--;
        mapa_vykresli(*data->mapa);
        pthread_mutex_unlock(data->mapa_mutex);
        //koniec krit sekcie

        printf("Pre pozastavenie simulácie zadajte 'H'\n");

        sleep(5);
    }
}

void je_horlavy(MAPA mapa, int x, int y) { //pracovna verzia
    if (mapa.mapa[x][y].horlavy) {
        //vlozit do zasobnika a vybrat na zaciatku kola
    } else {
        printf("Bunka na suradniciah %d %d nezacala horiet pretoze je to biotop %c \n", x, y, mapa.mapa[x][y].biotop);
    }
}


void *menu(void *thr_data) {
    MENU_THREAD_DATA *data = (MENU_THREAD_DATA *) thr_data;
    char akcia = ' ';
    bool pozastavenie = false;

    if (data->zaciatok) {
        printf("Zadaj akciu:\n");
        printf("\tS: Začni simulaciu\n");
        printf("\tN: Načítaj simulacia\n");
        printf("\tP: Pripojit sa na server\n");
        printf("\tU: Ukonči program\n");
        while(akcia == 'S' || akcia == 'N' || akcia == 'P' || akcia == 'U') {
            scanf("%c", &akcia);
        }
        switch (akcia) {
            case 'S':
                printf("Zadajte akciu:\n");
                printf("\t1: Vytvorit vlastnu mapu.\n");
                printf("\t2: Vygenerovat mapu.\n");
                while(akcia == '1' || akcia == '2') {
                    scanf("%c", &akcia);
                }
                if (akcia == '1') {
                    mapa_rucne(data->mapa);
                } else {
                    printf("Zadajte rozmery mapy:\n");
                    printf("\tSirka:\n");
                    int sirka;
                    scanf("%d", &sirka);
                    printf("\tVyska:\n");
                    int vyska;
                    scanf("%d", &vyska);
                    mapa_init(data->mapa, sirka, vyska, data->vietor);
                    mapa_vykresli(*data->mapa);
                }
                break;
            case 'N':

                break;
            case 'P':

                break;
            case 'U':

                break;
        }


        data->zaciatok = false;
    }

    switch (akcia) {
        case 'S':
            if (!pozastavenie) {
                *data->je_pozastavena = true;
                pozastavenie = true;
            } else {
                *data->je_pozastavena = false;
                pozastavenie = false;
            }
            break;
        case 'Z':
            //TODO: pozastav simulaciu
            printf("Zadajte súradnice bunky, ktorú chcete zapáliť:\n");
            printf("\tX:\n");
            int x;
            scanf("%d", &x);
            printf("\tY:\n");
            int y;
            scanf("%d", &y);

            mapa_rozsir_ohen(data->mapa, x, y, data->vietor->smer); // kontrolu ci je horlave

            break;
        case 'U':
            printf("Ukončujem program.\n");
            break;
        default:
            printf("Zadali ste zlú akciu\n");
            break;
    }

}

int main() {

    srand(time(NULL));
    //==========deklar cast=======
    MAPA mapa;
    VIETOR vietor;
    SIMULACIA_THREAD_DATA thread_data;
    pthread_mutex_t mapa_mutex;
    pthread_cond_t bezi;
    bool je_pozastavena = false;
    bool je_ukonceny = false;



    //==========init cast=========
    vietor.smer = 0;
    vietor.trvanie = 0;
    //TODO: toto pojde prec
    mapa_init(&mapa, 10, 6, &vietor);
    //
    pthread_mutex_init(&mapa_mutex, NULL);
    pthread_cond_init(&bezi, NULL);

    thread_data.je_pozastavena = &je_pozastavena;
    thread_data.mapa = &mapa;
    thread_data.mapa_mutex = &mapa_mutex;
    thread_data.bezi = &bezi;
    thread_data.vietor = &vietor;

    //========Logika cast========
    mapa_vykresli(mapa);

    for (int i = 0; i < 5; ++i) {
        int zapal2 = rand() % 10;
        int zapal1 = rand() % 6;
        if (mapa.mapa[zapal1][zapal2].horlavy) {
            mapa_rozsir_ohen(&mapa, zapal1, zapal2, 0);
            printf("Zapalene policko: [%d, %d]\n", zapal1, zapal2);
        }
    }

    //mapa_vykresli(mapa);

    simulacia(&thread_data);

    mapa_vykresli(mapa);

    //=========Destroy casti==========
    mapa_destroy(&mapa);
    pthread_mutex_destroy(&mapa_mutex);

    return 0;

}