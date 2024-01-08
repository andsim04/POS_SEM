//
// Created by andre on 2. 1. 2024.
//
#include "bunka.h"
#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>

void vykonaj_prikaz(int prikaz, bool* server_zapnuty, int clientSocket) {
    if (prikaz == 0) {
        printf("Vypinam\n");
        *server_zapnuty = false;

    } else if (prikaz == 1) {
        printf("Uloz\n");
        MAPA mapa;
        int sirka, vyska;

        ssize_t sirka_ = recv(clientSocket, &sirka, sizeof(sirka), 0);
        if (sirka_ == -1) {
            perror("Chyba pri prijímaní dát");
            close(clientSocket);
        }
        printf("Prijimam sirku: %d\n", sirka);
        ssize_t vyska_ = recv(clientSocket, &vyska, sizeof(vyska), 0);
        if (vyska_ == -1) {
            perror("Chyba pri prijímaní dát");
            close(clientSocket);
        }
        printf("Prijimam vyska: %d\n", vyska);

        mapa_init2(&mapa, sirka, vyska, NULL);

        // Receive the serialized data
        char receivedMapa[vyska * sirka * sizeof(BUNKA)];
        ssize_t mapa_ = recv(clientSocket, receivedMapa, sizeof(receivedMapa), 0);
        if (mapa_ == -1) {
            perror("Error receiving data");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }

        // Deserializácia dát buniek
        int k = 0;
        for (int i = 0; i < vyska; ++i) {
            for (int j = 0; j < sirka; ++j) {
                memcpy(&mapa.mapa[i][j], &receivedMapa[k], sizeof(BUNKA));
                k += sizeof(BUNKA);
            }
        }
        mapa_vykresli(mapa);
        ulozenie_mapy(mapa, "./UlozeneMapy/ServerMapy.txt");

        mapa_destroy(&mapa);

    } else if (prikaz == 2) { //nacitanie mozno by to slo ze iba chary sa nacitaju a spracuju v klientovi nemusela by sa robit serializacia a bolo by to mensie
        MAPA nacitane_mapy[20];
        int pocet_map = 0;
        int cislo_mapy = 0;
        FILE *f = fopen("./UlozeneMapy/ServerMapy.txt", "r");
        char znak = ' ';
        int sirka = 0;
        int vyska = 0;
        const unsigned int odpad = 1;
        char prazdnyRiadok[odpad];
        int sirka_mapy = 0;
        int vyska_mapy = 0;
        BUNKA bunky[10][10];
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

        ssize_t pocet_map_ = send(clientSocket, &pocet_map, sizeof(int), 0);
        if (pocet_map_== -1) {
            perror("Error sending data");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }
        cislo_mapy = 0;
        while (pocet_map > cislo_mapy) {

            ssize_t sirka_ = send(clientSocket, &nacitane_mapy[cislo_mapy].sirka, sizeof(int), 0);
            if (sirka_ == -1) {
                perror("Error sending data");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }

            ssize_t vyska_ = send(clientSocket, &nacitane_mapy[cislo_mapy].vyska, sizeof(int), 0);
            if (vyska_ == -1) {
                perror("Error sending data");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }

            BUNKA mapa[nacitane_mapy[cislo_mapy].vyska][nacitane_mapy[cislo_mapy].sirka];

            for (int i = 0; i < nacitane_mapy[cislo_mapy].vyska; ++i) {
                for (int j = 0; j < nacitane_mapy[cislo_mapy].sirka; ++j) {
                    mapa[i][j] = nacitane_mapy[cislo_mapy].mapa[i][j];
                }
            }

            // Serializacia dát buniek
            char serializedMapa[nacitane_mapy[cislo_mapy].vyska * nacitane_mapy[cislo_mapy].sirka * sizeof(BUNKA)];
            int l = 0;

            for (int j = 0; j < nacitane_mapy[cislo_mapy].vyska; ++j) {
                for (int k = 0; k < nacitane_mapy[cislo_mapy].sirka; ++k) {
                    memcpy(&serializedMapa[l], &nacitane_mapy[cislo_mapy].mapa[j][k], sizeof(BUNKA));
                    l += sizeof(BUNKA);
                }
            }

            // Send the serialized data
            ssize_t mapa_ = send(clientSocket, serializedMapa, sizeof(serializedMapa), 0);
            if (mapa_ == -1) {
                perror("Error sending data");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }
            nacitane_mapy[cislo_mapy].je_inicializovana = true;
            cislo_mapy++;

        }
        for (int i = 0; i < pocet_map; ++i) {
            mapa_destroy(&nacitane_mapy[i]);
        }

    }
    printf("Server vykonal prikaz: %d\n", prikaz);
}

int main() {
    int PORT = 10123;
    int BUFFER_SIZE = 1024;

    bool je_pripojeny = false;
    bool server_zapnuty = true;
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    int buffer;

    //Vytvorenie socketu
    //TCP socket komunikacia
    //AF_INET = Adress family internet (oznacenie ze budeme vyuzivat IPv4 adresy)
    //SOCK_STREAM = sposob komunikacie, vyuziva sa aj v TCP na zarucenie poradie prijimania informacii
    //0 = defaultny protocol pre specificku rodinu adries (v nasom pripade IPv4)
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Chyba pri vytváraní socketu");
        exit(EXIT_FAILURE);
    }

    //Nastavenie adresovej štruktúry
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; //server počúva na všetkych sietových rozhraniach
    serverAddr.sin_port = htons(PORT); //"host to network short" zabezpecuje rovnaku interpetaciu bytoveho poradia

    //Socket binding
    //Pokusime sa priradit socketu IP adresu a port, v pripade ze to nevyjde tak uzavrie socket a ukončí program
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Chyba priradenia server socketu");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Čakanie na prichádzajúce pripojenie
    //listen() dáva vedieť operačnému systému, že nami vytvorený socket by mal prijímať prichádzajúce pripojenia
    // parameter 0 udáva maximalny pocet v rade cakajúcich pripojení (0 == default OS)
    //v pripade zlyhania sa socket uzavrie a program sa ukončí
    if (listen(serverSocket, 0) == -1) {
        perror("Chyba pri čakaní na pripojenie");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
    printf("Server počúva na porte: %d\n", PORT);

    while (server_zapnuty) {

        if(!je_pripojeny) {
            //Prijatie pripojenia
            //na accept() server ostane stáť a čaká kým sa nenaviaže spojenie s klientom,
            //v momente ako sa naviaže spojenie server začne pokračovať.
            //Ak sa naviazalo chybné spojenie alebo došlo k chybe tak sa socket uzavrie a program sa ukončí
            clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrSize);
            if (clientSocket == -1) {
                perror("Chyba pri prijatí spojenia");
                close(serverSocket);
                exit(EXIT_FAILURE);
            }
            printf("Klienta sa napojil z adresy %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            je_pripojeny = true;
        }

        //Prijímanie dát od klienta
        ssize_t prikaz_ = recv(clientSocket, &buffer, sizeof(buffer), 0);
        if (prikaz_ == -1) {
            perror("Chyba pri prijímaní dát");
            close(clientSocket);
            continue;
        } else if (prikaz_ == 0) {
            printf("Klient sa odpojil!\n");
            printf("Cakam na nového klienta.\n");
            je_pripojeny = false;
            continue;
        }
        printf("Prijaté dáta od klienta: %d\n", buffer);
        vykonaj_prikaz(buffer, &server_zapnuty, clientSocket);

    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}
