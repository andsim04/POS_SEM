cmake_minimum_required(VERSION 3.18.4)
project(POS_SEM C)

set(CMAKE_C_STANDARD 11)

add_executable(POS_SEM
        bunka.h
        klient.c
        klient.h
        server.c
        server.h
        mapa.c
        mapa.h
        bunka.c)

target_link_libraries(POS_SEM pthread)
