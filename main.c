
#include <stdio.h>
#include <time.h>
#include "mapa.h"

int main() {
    srand(time(NULL));
    MAPA mapa;
    mapa_init(&mapa, 6, 8);
    mapa_vykresli(mapa);
    mapa_destroy(&mapa);
    return 0;
}
