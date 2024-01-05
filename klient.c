//
// Created by andre on 2. 1. 2024.
//

#include "klient.h"


void *simulacia(void *thr_data) {
    SIMULACIA_THREAD_DATA *data = (SIMULACIA_THREAD_DATA *) thr_data;
    //TODO resetovat kolocount po pridani novej mapy
    int kolocount = 0;
    while (*data->ukonci) {
        kolocount++;
        pthread_mutex_lock(data->mapa_mutex);
        if (*data->nova_mapa) {
            kolocount = 1;
            *data->nova_mapa = false;
        }
        while (*data->je_pozastavena) {
            pthread_cond_signal(data->pozastavena);
            pthread_cond_wait(data->bezi, data->mapa_mutex);
        }
        if (!*data->ukonci) {
            break;
        }
        BUNKA horiace_bunky[data->mapa->sirka * data->mapa->vyska];
        BUNKA zhorene_bunky[data->mapa->sirka * data->mapa->vyska];
        BUNKA luky_bunky[data->mapa->sirka * data->mapa->vyska];

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



void zaciatocne_menu(char akcia, MENU_THREAD_DATA* data) {
    if (akcia == ' ') {
        printf("Zadaj akciu:\n");
        printf("\tZ: Začni simulaciu\n");
        printf("\tL: Načítaj simulacia\n");
        printf("\tC: Pripojit sa na server\n");
        printf("\tX: Ukonči program\n");
    }
    while (akcia != 'Z' && akcia != 'L' && akcia != 'C' && akcia != 'X') {
        scanf("%c", &akcia);
    }
    switch (akcia) {
        case 'Z':
            printf("Zadajte akciu:\n");
            printf("\t1: Vytvorit vlastnu mapu.\n");
            printf("\t2: Vygenerovat mapu.\n");
            while (akcia != '1' && akcia != '2') {
                scanf("%c", &akcia);
            }
            if (akcia == '1') {
                mapa_rucne(data->mapa);
                mapa_vykresli(*data->mapa);
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
            *data->je_pozastavena = false;
            *data->nova_mapa = true;
            pthread_mutex_unlock(data->mapa_mutex);
            pthread_cond_signal(data->bezi);

            break;
        case 'L':
            *data->je_pozastavena = true;
            *data->nova_mapa = true;
            nacitanie_mapy(data->mapa, "../UlozeneMapy/UlozeneMapy.txt" );
            pthread_mutex_unlock(data->mapa_mutex);
            break;
        case 'C':

            break;
        case 'X':
            *data->ukonci = false;
            *data->je_pozastavena = false;
            pthread_mutex_unlock(data->mapa_mutex);
            pthread_cond_signal(data->bezi);
            break;
        default:
            printf("Zadany zlý parameter!\n");
            break;
    }
}

void zapal_bunky(void* thr_data) {
    MENU_THREAD_DATA*data = (MENU_THREAD_DATA *) thr_data;
    int riadok = 0;
    int stlpec = 0;
    while (true) {
        printf("Zadajte riadok: \n");
        scanf("%d", &riadok);
        printf("Zadajte stlpec: \n");
        scanf("%d", &stlpec);
        if ((riadok > 0 && riadok <= data->mapa->vyska) && (stlpec > 0 && stlpec <= data->mapa->sirka)) {
            if(data->mapa->mapa[riadok][stlpec].horlavy) {

            }
        }
    }
}



void *menu(void *thr_data) {
    MENU_THREAD_DATA*data = (MENU_THREAD_DATA *) thr_data;
    bool menu_prerusenie = false;
    while (*data->ukonci) {
        char akcia = ' ';

        pthread_mutex_lock(data->mapa_mutex);
        if (data->zaciatok) {
            zaciatocne_menu(akcia, data);
            data->zaciatok = false;
            if (!*data->ukonci) {
                break;
            }
        }
        pthread_mutex_unlock(data->mapa_mutex);
        if (!menu_prerusenie) {
            while (akcia != 'H') {
                scanf(" %c", &akcia);
            }
        }
        if (!menu_prerusenie) {
            pthread_mutex_lock(data->mapa_mutex);
            *data->je_pozastavena = true;
            pthread_cond_wait(data->pozastavena, data->mapa_mutex);
            pthread_mutex_unlock(data->mapa_mutex);
        } else {
            menu_prerusenie = false;
        }


        printf("Simulácia bola pozastavená:\n");
        printf("\tZadaj akciu:\n");
        printf("\t\tP: Pokračuj v simulácii\n");
        printf("\t\tZ: Zapal bunku\n");
        printf("\t\tN: Nová simulacia\n");
        printf("\t\tU: Ulož aktuálnu simulaciu\n");
        printf("\t\tL: Načítaj novú simulaciu\n");
        printf("\t\tC: Pripojit sa na server\n");
        printf("\t\tX: Ukonči program\n");

        while (akcia != 'P' && akcia != 'N' && akcia != 'U' && akcia != 'L' && akcia != 'C' && akcia != 'X') {
            scanf("%c", &akcia);
        }
        pthread_mutex_lock(data->mapa_mutex);
        switch (akcia) {
            case 'P':
                *data->je_pozastavena = false;
                pthread_mutex_unlock(data->mapa_mutex);
                pthread_cond_signal(data->bezi);
                break;
            case 'Z':
                //TODO: Zapalenie bunky, pauznut + zadat suradnice zapalenej bunky + osetrit horlavost
                zapal_bunky(data);
                *data->je_pozastavena = true;
                *data->nova_mapa = true;
                menu_prerusenie = true;

                break;
            case 'N':
                zaciatocne_menu('Z', data);
                *data->nova_mapa = true;
                break;
            case 'U':
                ulozenie_mapy(*data->mapa, "../UlozeneMapy/UlozeneMapy.txt");
                pthread_mutex_unlock(data->mapa_mutex);
                menu_prerusenie = true;
                break;
            case 'L':
                *data->je_pozastavena = true;
                *data->nova_mapa = true;
                nacitanie_mapy(data->mapa, "../UlozeneMapy/UlozeneMapy.txt" );
                pthread_mutex_unlock(data->mapa_mutex);
                menu_prerusenie = true;
                break;
            case 'C':
                //TODO: Pripojenie na server
                break;
            case 'X':
                *data->ukonci = false;
                *data->je_pozastavena = false;
                pthread_mutex_unlock(data->mapa_mutex);
                pthread_cond_signal(data->bezi);
                break;
            default:
                printf("Zadany zlý parameter!\n");
                break;
        }
    }
    pthread_mutex_unlock(data->mapa_mutex);
}

int main() {
    srand(time(NULL));
    //==========deklar cast=======
    int PORT = 99887;
    int BUFFER_SIZE = 1024;
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    pthread_t thread_menu, thread_simulacia;
    MAPA mapa;
    mapa.je_inicializovana = false;
    VIETOR vietor;

    SIMULACIA_THREAD_DATA simulacia_thread_data;
    MENU_THREAD_DATA menu_thread_data;
    pthread_mutex_t mapa_mutex;
    pthread_cond_t bezi;
    pthread_cond_t pozastavena;
    bool je_pozastavena = false;
    bool ukonci = true;
    bool zaciatok = true;
    bool nova_mapa = true;


    //==========init cast=========

    //Vytvorenie socketu pre klient
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Nastavenie IP adresy
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    //inet_pton premeni stringový format IP adresy na binarny format
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("Error configuring server address");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Pripojenie na server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to server");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Zaslanie príkazu na server
    const char* command = "do_something1";
    ssize_t bytesSent = send(clientSocket, command, strlen(command), 0);
    if (bytesSent == -1) {
        perror("Error sending data");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    const char* command1 = "do_something2";
    ssize_t bytesSent1 = send(clientSocket, command1, strlen(command1), 0);
    if (bytesSent1 == -1) {
        perror("Error sending data");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    vietor.smer = 0;
    vietor.trvanie = 0;

    //mapa_init(&mapa, 10, 6, &vietor);

    pthread_mutex_init(&mapa_mutex, NULL);
    pthread_cond_init(&bezi, NULL);
    pthread_cond_init(&pozastavena, NULL);

    menu_thread_data.je_pozastavena = &je_pozastavena;
    menu_thread_data.mapa = &mapa;
    menu_thread_data.mapa_mutex = &mapa_mutex;
    menu_thread_data.bezi = &bezi;
    menu_thread_data.vietor = &vietor;
    menu_thread_data.zaciatok = &zaciatok;
    menu_thread_data.pozastavena = &pozastavena;
    menu_thread_data.ukonci = &ukonci;
    menu_thread_data.nova_mapa = &nova_mapa;

    simulacia_thread_data.je_pozastavena = &je_pozastavena;
    simulacia_thread_data.mapa = &mapa;
    simulacia_thread_data.mapa_mutex = &mapa_mutex;
    simulacia_thread_data.bezi = &bezi;
    simulacia_thread_data.vietor = &vietor;
    simulacia_thread_data.pozastavena = &pozastavena;
    simulacia_thread_data.ukonci = &ukonci;
    simulacia_thread_data.nova_mapa = &nova_mapa;

    pthread_create(&thread_menu, NULL, menu, &menu_thread_data);
    pthread_create(&thread_simulacia, NULL, simulacia, &simulacia_thread_data);

    /*========Logika cast========
    mapa_vykresli(mapa);
    for (int i = 0; i < 5; ++i) {
        int zapal2 = rand() % 10;
        int zapal1 = rand() % 6;
        if (mapa.mapa[zapal1][zapal2].horlavy) {
            mapa_rozsir_ohen(&mapa, zapal1, zapal2, 0);
            printf("Zapalene policko: [%d, %d]\n", zapal1, zapal2);
        }
    }
    mapa_vykresli(mapa);
    simulacia(&simulacia_thread_data);
    mapa_vykresli(mapa);
    */

    //=========Destroy casti==========
    pthread_join(thread_simulacia, NULL);
    pthread_join(thread_menu, NULL);

    if (mapa.je_inicializovana) {
        mapa_destroy(&mapa);
    }
    pthread_mutex_destroy(&mapa_mutex);

    pthread_cond_destroy(&bezi);
    pthread_cond_destroy(&pozastavena);

    //Ukoncenie servera pomocou príkazu
    ssize_t prikaz_vypnutia = send(clientSocket, "Vypni", strlen("Vypni"), 0);
    if (prikaz_vypnutia == -1) {
        perror("Error sending data");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    close(clientSocket);
    return 0;

}