//
// Created by andre on 2. 1. 2024.
//

#include "klient.h"


void *simulacia(void *thr_data) {
    SIMULACIA_THREAD_DATA *data = (SIMULACIA_THREAD_DATA *) thr_data;
    int kolocount = 0;
    while (*data->ukonci) {
        kolocount++;
        pthread_mutex_lock(data->mapa_mutex);
        while (*data->je_pozastavena) {
            pthread_cond_signal(data->pozastavena);
            pthread_cond_wait(data->bezi, data->mapa_mutex);
        }
        if (*data->nova_mapa) {
            kolocount = 1;
            *data->nova_mapa = false;
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
        scanf(" %c", &akcia);
    }
    switch (akcia) {
        case 'Z':
            printf("Zadajte akciu:\n");
            printf("\t1: Vytvorit vlastnu mapu.\n");
            printf("\t2: Vygenerovat mapu.\n");
            while (akcia != '1' && akcia != '2') {
                scanf(" %c", &akcia);
            }
            if (akcia == '1') {
                mapa_rucne(data->mapa);
                mapa_vykresli(*data->mapa);
            } else {
                printf("Zadajte rozmery mapy:\n"); //TODO opravit vstupy
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

            printf("Chcete načítať mapu z lokálneho súboru alebo serveru ? J/S\n");
            while (akcia != 'J' && akcia != 'S') {
                scanf(" %c", &akcia);
            }
            if (akcia == 'J') {
                *data->je_pozastavena = true;
                *data->nova_mapa = true;
                nacitanie_mapy(data->mapa, "../UlozeneMapy/UlozeneMapy.txt");
                pthread_mutex_unlock(data->mapa_mutex);
            } else {
                // TODO ide to ale sirku aj vysku nejako dogabalo
                //pthread_mutex_lock(data->mapa_mutex);
                if (data->je_pripojeny) {
                    int prikaz = 2;
                    int pocet_map = 0;
                    ssize_t prikaz_ = send(data->clientSocket, &prikaz, sizeof(int), 0);
                    if (prikaz_ == -1) {
                        perror("Error sending data");
                        close(data->clientSocket);
                        exit(EXIT_FAILURE);
                    }

                    ssize_t pocet_map_ = recv(data->clientSocket, &pocet_map, sizeof(int), 0);
                    if (pocet_map_ == -1) {
                        perror("Chyba pri prijímaní dát");
                        close(data->clientSocket);
                    }
                    MAPA mapy[pocet_map];
                    for (int i = 0; i < pocet_map; ++i) {
                        ssize_t sirka_ = recv(data->clientSocket, &mapy[i].sirka, sizeof(int), 0);
                        if (sirka_ == -1) {
                            perror("Error sending data");
                            close(data->clientSocket);
                            exit(EXIT_FAILURE);
                        }


                        ssize_t vyska_ = recv(data->clientSocket, &mapy[i].vyska, sizeof(int), 0);
                        if (vyska_ == -1) {
                            perror("Error sending data");
                            close(data->clientSocket);
                            exit(EXIT_FAILURE);
                        }

                        mapa_init2(&mapy[i],mapy[i].sirka, mapy[i].vyska, NULL);


                        char receivedMapa[mapy[i].vyska * mapy[i].sirka * sizeof(BUNKA)];
                        ssize_t mapa_ = recv(data->clientSocket, receivedMapa, sizeof(receivedMapa), 0);
                        if (mapa_ == -1) {
                            perror("Error receiving data");
                            close(data->clientSocket);
                            exit(EXIT_FAILURE);
                        }

                        // Deserializácia dát buniek
                        int l = 0;
                        for (int j = 0; j < mapy[i].vyska; ++j) {
                            for (int k = 0; k < mapy[i].sirka; ++k) {
                                memcpy(&mapy[i].mapa[j][k], &receivedMapa[l], sizeof(BUNKA));
                                l += sizeof(BUNKA);
                            }
                        }
                    }
                    if (data->mapa->je_inicializovana) {
                        mapa_destroy(data->mapa);
                    }
                    mapa_init2(data->mapa, mapy[0].sirka, mapy[0].vyska, NULL);
                    for (int i = 0; i < data->mapa->vyska; ++i) {
                        for (int j = 0; j < data->mapa->sirka; ++j) {
                            data->mapa->mapa[i][j] = mapy[0].mapa[i][j];
                        }

                    }
                    *data->je_pozastavena = false;
                    *data->nova_mapa = true;
                    pthread_mutex_unlock(data->mapa_mutex);
                    //pthread_cond_signal(data->bezi);
                }

            }

            break;
        case 'C':
            //TODO: spravit aby to nepadlo ked nie je zapnuty server
            if (!*data->je_pripojeny) {
                if (connect(data->clientSocket, (struct sockaddr *) data->serverAddr, sizeof(*data->serverAddr)) ==
                    -1) {
                    perror("Chyba pripojenia na server.\n");
                    close(data->clientSocket);
                    exit(EXIT_FAILURE);
                } else {
                    printf("Uspesne pripojenie na server!\n");
                    *data->je_pripojeny = true;
                }
            } else printf("Klient uz je pripojeny na server!\n");
            *data->je_pozastavena = true;
            zaciatocne_menu(' ', data);
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
    bool hotovo = false;
    while (!hotovo) {
        printf("Zadajte riadok: \n");
        scanf("%d", &riadok);
        printf("Zadajte stlpec: \n");
        scanf("%d", &stlpec);
        if ((riadok >= 0 && riadok < data->mapa->vyska) && (stlpec >= 0 && stlpec < data->mapa->sirka)) {
            if(data->mapa->mapa[riadok][stlpec].horlavy) {
                data->mapa->mapa[riadok][stlpec].biotop = 'O';
                data->mapa->mapa[riadok][stlpec].ohen = true;
                data->mapa->mapa[riadok][stlpec].horlavy = false;
                printf("Policko na suradniciach [%d,%d]\n", riadok, stlpec);
                char volba = ' ';
                while (true) {
                    printf("Chcete zadat dalsie policko A/N?\n");
                    scanf(" %c", &volba);
                    if (volba == 'A') {
                        break;
                    } else if (volba == 'N') {
                        hotovo = true;
                        break;
                    } else {
                        printf("Nespravny vstup!\n");
                    }
                }

            } else {
                printf("Policko je nehorlave!\n");
            }
        } else {
            printf("Nespravne suradnice!\n");
        }
    }

    mapa_vykresli(*data->mapa);
}

void *menu(void *thr_data) {
    MENU_THREAD_DATA*data = (MENU_THREAD_DATA *) thr_data;
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
        if (!*data->menu_prerusenie) {

            while (akcia != 'H') {
                scanf(" %c", &akcia);
            }
            printf("Simulácia bola pozastavená.\n");
        }
        if (!*data->menu_prerusenie) {
            pthread_mutex_lock(data->mapa_mutex);
            *data->je_pozastavena = true;
            pthread_cond_wait(data->pozastavena, data->mapa_mutex);
            pthread_mutex_unlock(data->mapa_mutex);
        } else {
            *data->menu_prerusenie = false;
        }

        printf("\tZadaj akciu:\n");
        printf("\t\tP: Pokračuj v simulácii\n");
        printf("\t\tZ: Zapal bunku\n");
        printf("\t\tN: Nová simulacia\n");
        printf("\t\tU: Ulož aktuálnu simulaciu\n");
        printf("\t\tL: Načítaj novú simulaciu\n");
        printf("\t\tC: Pripojit sa na server\n");
        printf("\t\tX: Ukonči program\n");

        while (akcia != 'P' && akcia != 'Z' && akcia != 'N' && akcia != 'U' && akcia != 'L' && akcia != 'C' && akcia != 'X') {
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
                zapal_bunky(data);
                *data->je_pozastavena = true;
                *data->menu_prerusenie = true;
                pthread_mutex_unlock(data->mapa_mutex);
                break;
            case 'N':
                zaciatocne_menu('Z', data);
                *data->nova_mapa = true;
                break;
            case 'U':
                if (data->je_pripojeny) {
                    int prikaz = 1;
                    ssize_t prikaz_ = send(data->clientSocket, &prikaz, sizeof(int), 0);
                    if (prikaz_ == -1) {
                        perror("Error sending data");
                        close(data->clientSocket);
                        exit(EXIT_FAILURE);
                    }

                    ssize_t sirka_ = send(data->clientSocket, &data->mapa->sirka, sizeof(int), 0);
                    if (sirka_ == -1) {
                        perror("Error sending data");
                        close(data->clientSocket);
                        exit(EXIT_FAILURE);
                    }

                    ssize_t vyska_ = send(data->clientSocket, &data->mapa->vyska, sizeof(int), 0);
                    if (vyska_ == -1) {
                        perror("Error sending data");
                        close(data->clientSocket);
                        exit(EXIT_FAILURE);
                    }

                    BUNKA mapa[data->mapa->vyska][data->mapa->sirka];

                    for (int i = 0; i < data->mapa->vyska; ++i) {
                        for (int j = 0; j < data->mapa->sirka; ++j) {
                            mapa[i][j] = data->mapa->mapa[i][j];
                        }
                    }

                    // Serializacia dát buniek
                    char serializedMapa[data->mapa->vyska * data->mapa->sirka * sizeof(BUNKA)];
                    int k = 0;

                    for (int i = 0; i < data->mapa->vyska; ++i) {
                        for (int j = 0; j < data->mapa->sirka; ++j) {
                            memcpy(&serializedMapa[k], &data->mapa->mapa[i][j], sizeof(BUNKA));
                            k += sizeof(BUNKA);
                        }
                    }

                    // Send the serialized data
                    ssize_t mapa_ = send(data->clientSocket, serializedMapa, sizeof(serializedMapa), 0);
                    if (mapa_ == -1) {
                        perror("Error sending data");
                        close(data->clientSocket);
                        exit(EXIT_FAILURE);
                    }

                } else printf("Neexistuje pripojenie na server\n");

                ulozenie_mapy(*data->mapa, "../UlozeneMapy/UlozeneMapy.txt");
                pthread_mutex_unlock(data->mapa_mutex);
                *data->menu_prerusenie = true;
                break;
            case 'L':
                *data->je_pozastavena = true;
                *data->nova_mapa = true;
                nacitanie_mapy(data->mapa, "../UlozeneMapy/UlozeneMapy.txt" );
                *data->menu_prerusenie = true;
                pthread_mutex_unlock(data->mapa_mutex);
                break;
            case 'C':
                //TODO: spravit aby to nepadlo ked nie je zapnuty server
                if (!*data->je_pripojeny) {
                    if (connect(data->clientSocket, (struct sockaddr *) data->serverAddr, sizeof(*data->serverAddr)) ==
                        -1) {
                        perror("Chyba pripojenia na server.\n");
                        close(data->clientSocket);
                        exit(EXIT_FAILURE);
                    } else {
                        printf("Uspesne pripojenie na server!\n");
                        *data->je_pripojeny = true;
                    }
                } else printf("Klient uz je pripojeny na server!\n");
                *data->je_pozastavena = true;
                *data->menu_prerusenie = true;
                pthread_mutex_unlock(data->mapa_mutex);
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
    int PORT = 99883;
    int clientSocket;
    struct sockaddr_in serverAddr;

    pthread_t thread_menu, thread_simulacia;
    MAPA mapa;
    mapa.je_inicializovana = false;
    VIETOR vietor;

    SIMULACIA_THREAD_DATA simulacia_thread_data;
    MENU_THREAD_DATA menu_thread_data;
    pthread_mutex_t mapa_mutex;
    pthread_cond_t bezi;
    pthread_cond_t pozastavena;
    bool je_pozastavena = true;
    bool ukonci = true;
    bool zaciatok = true;
    bool nova_mapa = true;
    bool je_pripojeny = false;
    bool menu_prerusenie = false;

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
    //if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
    //    perror("Error connecting to server");
    //    close(clientSocket);
    //    exit(EXIT_FAILURE);
    //}

    menu_thread_data.clientSocket = clientSocket;
    menu_thread_data.serverAddr = &serverAddr;

    vietor.smer = 0;
    vietor.trvanie = 0;

    pthread_mutex_init(&mapa_mutex, NULL);
    pthread_cond_init(&bezi, NULL);
    pthread_cond_init(&pozastavena, NULL);

    menu_thread_data.je_pozastavena = &je_pozastavena;
    menu_thread_data.je_pripojeny = &je_pripojeny;
    menu_thread_data.mapa = &mapa;
    menu_thread_data.mapa_mutex = &mapa_mutex;
    menu_thread_data.bezi = &bezi;
    menu_thread_data.vietor = &vietor;
    menu_thread_data.zaciatok = &zaciatok;
    menu_thread_data.pozastavena = &pozastavena;
    menu_thread_data.ukonci = &ukonci;
    menu_thread_data.nova_mapa = &nova_mapa;
    menu_thread_data.menu_prerusenie = &menu_prerusenie;

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


    //=========Destroy cast==========
    pthread_join(thread_simulacia, NULL);
    pthread_join(thread_menu, NULL);

    if (mapa.je_inicializovana) {
        mapa_destroy(&mapa);
    }
    pthread_mutex_destroy(&mapa_mutex);

    pthread_cond_destroy(&bezi);
    pthread_cond_destroy(&pozastavena);


    ////Ukoncenie servera pomocou príkazu
    //    ssize_t prikaz_vypnutia = send(clientSocket, "Vypni", strlen("Vypni"), 0);
    //    if (prikaz_vypnutia == -1) {
    //        perror("Error sending data");
    //        close(clientSocket);
    //        exit(EXIT_FAILURE);
    //    }
    //}
    close(clientSocket);
    return 0;

}