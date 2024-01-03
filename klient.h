//
// Created by andre on 2. 1. 2024.
//
#include "mapa.h"
#include <time.h>

#ifndef POS_SEM_KLIENT_H
#define POS_SEM_KLIENT_H

typedef struct vietor {
    int trvanie;
    int smer;
} VIETOR;

typedef struct simulacia_thread_data {
    MAPA* mapa;
} SIMULACIA_THREAD_DATA;

void* simulacia(void* thr_data);
int main();

#endif //POS_SEM_KLIENT_H
