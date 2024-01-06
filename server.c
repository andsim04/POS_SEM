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
        ulozenie_mapy(mapa, "../UlozeneMapy/ServerMapy.txt");

        mapa_destroy(&mapa);

    } else if (prikaz == 2) {

    }
    printf("Server vykonal prikaz: %d\n", prikaz);
}

int main() {
    int PORT = 99887;
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
    // parameter 5 udáva maximalny pocet v rade cakajúcich pripojení
    //v pripade zlyhania sa socket uzavrie a program sa ukončí
    if (listen(serverSocket, 5) == -1) {
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
