//
// Created by andre on 2. 1. 2024.
//
#include "mapa.h"
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#ifndef POS_SEM_KLIENT_H
#define POS_SEM_KLIENT_H
// mozno spojit
typedef struct simulacia_thread_data {
    MAPA* mapa;
    VIETOR * vietor;
    bool * je_pozastavena;
    bool * ukonci;
    bool * nova_mapa;
    pthread_mutex_t* mapa_mutex;
    pthread_cond_t * pozastavena;
    pthread_cond_t * bezi;
} SIMULACIA_THREAD_DATA;

typedef struct menu_thread_data {
    MAPA* mapa;
    VIETOR * vietor;
    bool * je_pozastavena;
    bool * zaciatok;
    bool * ukonci;
    bool * nova_mapa;
    pthread_mutex_t* mapa_mutex;
    pthread_cond_t * pozastavena;
    pthread_cond_t * bezi;
} MENU_THREAD_DATA;

void* simulacia(void* thr_data);
void* menu(void* thr_data);
int main();
void zaciatocne_menu(char akcia, MENU_THREAD_DATA* data);

#endif //POS_SEM_KLIENT_H
