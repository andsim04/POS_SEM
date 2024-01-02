
#include <stdio.h>
#include <time.h>
#include "mapa.h"
#include <pthread.h>

int main() {
    srand(time(NULL));
    MAPA mapa;
    mapa_init(&mapa, 6, 8);
    mapa_vykresli(mapa);

    mapa_rozsir_ohen(&mapa, 1, 1, 2);

    mapa_vykresli(mapa);

    mapa_destroy(&mapa);
    return 0;
}
