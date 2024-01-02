
#include <stdio.h>
#include "mapa.h"

int main() {
    MAPA mapa;
    mapa_init(&mapa,5, 8);
    mapa_vykresli(mapa);
    return 0;
}
