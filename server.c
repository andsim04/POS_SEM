//
// Created by andre on 2. 1. 2024.
//

#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>

void vykonaj_prikaz(char* prikaz, bool* server_zapnuty) {
    if (strcmp(prikaz, "Vypni") == 0) {
        *server_zapnuty = false;
    }
    printf("Server vykonal prikaz: %s\n", prikaz);
}

int main() {
    int PORT = 99883;
    int BUFFER_SIZE = 1024;

    bool server_zapnuty = true;
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];

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

    while (server_zapnuty) {
        //Prijímanie dát od klienta
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            perror("Chyba pri prijímaní dát");
            close(clientSocket);
            continue;
        }
        printf("Prijaté dáta od klienta: %s\n", buffer);
        buffer[bytesRead] = '\0';
        vykonaj_prikaz(buffer, &server_zapnuty);

    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}
